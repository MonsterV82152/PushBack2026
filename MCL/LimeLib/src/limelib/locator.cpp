#include "limelib/locator.hpp"

void limelib::Locator::setPose(real_t x, real_t y, real_t theta)
{
    this->setPose(limelib::Pose2D(x, y, theta));
}

limelib::Odometry::Odometry(TrackingWheel *verticalTW, TrackingWheel *horizontalTW, pros::IMU &imu, bool shouldTaskRun)
    : verticalTW(verticalTW), horizontalTW(horizontalTW), imu(imu), currentPose(0, 0, 0), prevPose(0, 0, 0), headingOffset(0), shouldTaskRun(shouldTaskRun), lastUpdateTime(pros::millis()), currentVelocity(), odomTask(nullptr)
{
}

void limelib::Odometry::calibrate()
{
    if (verticalTW != nullptr)
        verticalTW->calibrate();
    if (horizontalTW != nullptr)
        horizontalTW->calibrate();
    imu.reset();
    while (imu.is_calibrating())
    {
        pros::delay(20);
    }

    headingOffset = -imu.get_heading() * M_PI / 180;
    if (shouldTaskRun)
    {
        odomTask = std::make_unique<pros::Task>([this]()
                                                {
            while (true)
            {
                update();
                pros::delay(10);
            } });
    }
}

void limelib::Odometry::setPose(limelib::Pose2D pose)
{
    headingOffset = (-imu.get_heading() + pose.theta) * M_PI / 180;
    currentPose = pose;
}

void limelib::Odometry::setPose(real_t x, real_t y, real_t theta)
{
    setPose(limelib::Pose2D(x, y, theta));
}

limelib::Pose2D limelib::Odometry::update()
{
    // Calculate time delta in seconds
    uint32_t currentTime = pros::millis();
    real_t dt = (currentTime - lastUpdateTime) / 1000.0f;
    lastUpdateTime = currentTime;

    real_t heading = imu.get_heading() * M_PI / 180 + headingOffset;
    real_t headingDiff = heading - currentPose.theta;
    real_t vertDist = verticalTW != nullptr ? verticalTW->getDistanceTravelled() + verticalTW->getOffset() * headingDiff / 2 : 0;
    real_t horDist = horizontalTW != nullptr ? horizontalTW->getDistanceTravelled() + horizontalTW->getOffset() * headingDiff / 2 : 0;

    real_t xChange = vertDist * sin(heading) + horDist * cos(heading);
    real_t yChange = vertDist * cos(heading) - horDist * sin(heading);

    currentPose.x += xChange;
    currentPose.y += yChange;
    currentPose.theta = heading;

    // Calculate velocity (inches per second)
    if (dt > 0.001f) // Avoid division by zero
    {
        currentVelocity.x = xChange / dt;
        currentVelocity.y = yChange / dt;
        currentVelocity.linear = std::sqrt(xChange * xChange + yChange * yChange) / dt;
        currentVelocity.angular = headingDiff / dt;
    }

    return Pose2D(xChange, yChange, headingDiff);
}

limelib::Pose2D limelib::Odometry::getPose(bool radians) const
{
    return radians ? currentPose : currentPose.toDegrees();
}

limelib::Velocity limelib::Odometry::getVelocity() const
{
    return currentVelocity;
}

limelib::MCL::MCL(TrackingWheel *verticalTW, TrackingWheel *horizontalTW,
                  pros::Imu &imu, std::vector<MCLDistance> &sensors, Field2D &field, int num_particles, real_t rotationNoise, real_t translationNoise, bool debug,
                  int intensity, bool shouldTaskRun)
    : odomHelper(verticalTW, horizontalTW, imu, false), sensors(sensors), field(field), NUM_PARTICLES(num_particles),
      ROTATION_NOISE(rotationNoise), TRANSLATION_NOISE(translationNoise), debug(debug), INTENSITY(intensity), last_mcl_update(intensity), randomParticleCount(0), shouldTaskRun(shouldTaskRun), prevPose(0, 0, 0), lastUpdateTime(pros::millis()), currentVelocity(), debugCounter(0),
      currentMode(LocalizationMode::ODOMETRY_PRIMARY), sensorConfidence(0.5), motionConfidence(1.0), overallConfidence(0.5),
      consecutiveLowConfidence(0), consecutiveHighConfidence(0), master(pros::E_CONTROLLER_MASTER)
{
    // Initialize particles with random positions but heading will be set from odometry later
    for (int i = 0; i < NUM_PARTICLES; i++)
    {
        // Start with 0 heading, will be updated to odometry heading in first update
        // Initialize with uniform weight
        particles.push_back(MCLParticle(Pose2D(getRandomReal_t(-field.getWidth() / 2, field.getWidth() / 2), getRandomReal_t(-field.getHeight() / 2, field.getHeight() / 2), 0), 1.0 / NUM_PARTICLES));
    }
}

void limelib::MCL::calibrate()
{
    odomHelper.calibrate();
    std::cout << "Field Edges: " << field.getEdges().size() << std::endl;
    std::cout << "MCL::calibrate - shouldTaskRun=" << shouldTaskRun << std::endl;
    if (shouldTaskRun)
    {
        std::cout << "MCL::calibrate - Creating MCL task..." << std::endl;
        // Create task and store in member variable so it persists
        mclTask = std::make_unique<pros::Task>([this]()
                                               {
            std::cout << "MCL task started!" << std::endl;
            while (true)
            {
                update();
                pros::delay(10);
            } });
        std::cout << "MCL::calibrate - MCL task created, ptr=" << mclTask.get() << std::endl;
    }
    else
    {
        std::cout << "MCL::calibrate - Task NOT created (shouldTaskRun=false)" << std::endl;
    }
}

void limelib::MCL::setPose(limelib::Pose2D pose)
{
    odomHelper.setPose(pose);

    // Update actualPose to match the set pose
    real_t odomHeading = pose.theta * M_PI / 180; // Convert to radians
    actualPose.x = pose.x;
    actualPose.y = pose.y;
    actualPose.theta = odomHeading;

    // Reset odomDelta since we're setting a new pose
    odomDelta = Pose2D(0, 0, 0);

    // Initialize estimated pose
    estimatedPose.point = actualPose;
    estimatedPose.weight = 1.0; // Full confidence initially

    particles.clear();
    // Initialize particles with position spread but all using the same odometry heading
    for (int i = 0; i < NUM_PARTICLES; i++)
    {
        particles.push_back(MCLParticle(Pose2D(pose.x + getRandomReal_t(-5, 5), pose.y + getRandomReal_t(-5, 5), odomHeading), 1.0 / NUM_PARTICLES));
    }
}

void limelib::MCL::setPose(real_t x, real_t y, real_t theta)
{
    setPose(limelib::Pose2D(x, y, theta));
}

limelib::Pose2D limelib::MCL::update()
{
    Pose2D currentDelta = odomHelper.update();
    odomDelta.x += currentDelta.x;
    odomDelta.y += currentDelta.y;
    odomDelta.theta += currentDelta.theta;
    // Normalize theta to [0, 2π)
    while (odomDelta.theta < 0)
        odomDelta.theta += 2 * M_PI;
    while (odomDelta.theta >= 2 * M_PI)
        odomDelta.theta -= 2 * M_PI;
    if (last_mcl_update > INTENSITY)
    {
        debugCounter++;
        updateMCL();
        if (debug)
            debugDisplay();
        // if (master.get_digital(pros::E_CONTROLLER_DIGITAL_B))
        // {
        //     Pose2D pose = getPose();
        //     std::cout << "MCL Pose: (" << pose.x << ", " << pose.y << ", " << pose.theta << "deg)" << std::endl;
        // }
        last_mcl_update = 0;
    }
    else
    {
        last_mcl_update++;
    }

    currentVelocity = odomHelper.getVelocity();

    return odomDelta;
}

void limelib::MCL::updateMCL()
{
    // Get current odometry heading for all particles
    real_t odomHeading = odomHelper.getPose(true).theta;

    // Adaptive noise: increase noise when confidence is low to maintain diversity
    real_t adaptiveTranslationNoise = TRANSLATION_NOISE;
    if (currentMode == LocalizationMode::RECOVERY_MODE)
    {
        adaptiveTranslationNoise *= 3.0; // Triple noise in recovery mode
    }
    else if (overallConfidence < 0.4)
    {
        adaptiveTranslationNoise *= 1.5; // Increase noise when struggling
    }

    for (MCLParticle &particle : particles)
    {
        // Apply translation motion with adaptive noise
        particle.point.x += odomDelta.x + getRandomReal_t(-adaptiveTranslationNoise, adaptiveTranslationNoise);
        particle.point.y += odomDelta.y + getRandomReal_t(-adaptiveTranslationNoise, adaptiveTranslationNoise);
        // Set all particles to use the same odometry heading (no rotation correction)
        particle.point.theta = odomHeading;
    }
    // 2. CORRECTION STEP - Update particle weights based on sensor observations
    real_t totalWeight = 0.0;
    int validSensorCount = 0;
    if (debug && debugCounter >= 50)
    {
        std::cout << std::endl
                  << std::endl
                  << "---- MCL Update ----" << std::endl;
        std::cout << "Valid sensor readings: " << std::endl;
        std::cout << "Current Localization Mode: " << (currentMode == LocalizationMode::ODOMETRY_PRIMARY ? "ODOMETRY_PRIMARY" : (currentMode == LocalizationMode::MCL_PRIMARY ? "MCL_PRIMARY" : "BALANCED")) << std::endl;
    }
    for (MCLDistance &sensor : sensors)
    {
        // Get raw int32_t value first to check for errors
        int32_t raw = sensor.sensor.get_distance();
        if (debug && debugCounter >= 50)
        {
            std::cout << "Sensor at angle " << sensor.pose.theta << "deg: ";
            std::cout << raw << " ";
        }
        // Valid readings are positive and less than 9999 (no object detected)
        // PROS_ERR is typically negative, which becomes huge positive when cast to float
        if (raw > 0 && raw < 2000)
        {
            if (debug && debugCounter >= 50)
                std::cout << "(valid) " << std::endl;
            sensor.reading = static_cast<real_t>(raw);
            validSensorCount++;
        }
        else
        {
            if (debug && debugCounter >= 50)
                std::cout << "(invalid) " << std::endl;
            sensor.reading = -1; // Mark as invalid
        }
    }
    if (debug && debugCounter >= 50)
        std::cout << std::endl;

    // Debug: Print sensor readings
    if ((debug && debugCounter >= 50) /* || master.get_digital(pros::E_CONTROLLER_DIGITAL_A)*/)
    {
        std::cout << "Valid sensors: " << validSensorCount << "/" << sensors.size() << " - Readings: ";
        for (const MCLDistance &sensor : sensors)
        {
            std::cout << sensor.reading << "mm ";
        }
        std::cout << std::endl;
    }

    // If no valid sensors, skip correction step - just apply odometry
    if (validSensorCount == 0)
    {
        // Still update actualPose with odometry delta
        actualPose.x += odomDelta.x;
        actualPose.y += odomDelta.y;
        actualPose.theta = odomHelper.getPose(true).theta;
        odomDelta = Pose2D(0, 0, 0);
        return;
    }

    for (MCLParticle &particle : particles)
    {
        real_t likelihood = 1.0;
        bool isFirstParticle = (&particle == &particles[0]); // Debug only first particle
        real_t particleAvgError = 0.0;
        int particleSensorCount = 0;

        // For each sensor, compare actual vs expected readings
        for (const MCLDistance &sensor : sensors)
        {
            // Skip invalid readings (marked as -1 during preprocessing)
            if (sensor.reading <= 0)
                continue;

            real_t actualDistance = sensor.reading * MM_TO_IN;

            // Calculate expected sensor reading from this particle's position
            // Transform sensor LOCAL position to GLOBAL position
            // The sensor position is relative to robot center, rotated by ROBOT heading only
            // (sensor.pose.theta only affects ray direction, not sensor physical position)
            real_t cosTheta = cos(particle.point.theta);
            real_t sinTheta = sin(particle.point.theta);

            // For 0°=+Y, 90°=+X coordinate system:
            // global_x = robot_x + local_x * cos(θ) + local_y * sin(θ)
            // global_y = robot_y + local_y * cos(θ) - local_x * sin(θ)
            real_t sensorGlobalX = particle.point.x + sensor.pose.x * cosTheta + sensor.pose.y * sinTheta;
            real_t sensorGlobalY = particle.point.y + sensor.pose.y * cosTheta - sensor.pose.x * sinTheta;

            // Ray direction = robot heading + sensor facing angle
            real_t sensorGlobalTheta = particle.point.theta + sensor.pose.theta * M_PI / 180;

            // Cast ray from sensor position in sensor direction
            Ray2D ray(Point2D(sensorGlobalX, sensorGlobalY), sensorGlobalTheta);
            real_t expectedDistance = getRayCastDistance(field.getEdges(), ray);

            // Debug output for first particle
            if (debug && isFirstParticle && debugCounter >= 50)
            {
                std::cout << "Particle at (" << particle.point.x << ", " << particle.point.y << ", " << particle.point.theta * 180 / M_PI << "deg)" << std::endl;
                std::cout << "Sensor at angle " << sensor.pose.theta << "deg, globalX: " << sensorGlobalX << ", globalY: " << sensorGlobalY
                          << ", actual=" << actualDistance << "in, expected=" << expectedDistance << "in" << std::endl;
            }

            // Calculate likelihood based on how well actual matches expected
            real_t error = std::abs(actualDistance - expectedDistance);

            // Track error for confidence calculation
            particleAvgError += error;
            particleSensorCount++;

            // Gaussian likelihood with σ=4 inches (variance=16)
            real_t sensorLikelihood = exp(-error * error / (2.0 * 16.0));
            // Apply minimum likelihood floor to prevent complete particle starvation
            sensorLikelihood = std::max(sensorLikelihood, real_t(0.001));

            likelihood *= sensorLikelihood;
        }

        particle.weight = likelihood;
        totalWeight += likelihood;
    }
    // Normalize weights
    if (totalWeight > 0)
    {
        for (MCLParticle &particle : particles)
        {
            particle.weight /= totalWeight;
        }
    }
    else
    {
        // If all weights are zero, reset to uniform distribution
        for (MCLParticle &particle : particles)
        {
            particle.weight = 1.0 / NUM_PARTICLES;
        }
    }

    // 3. UPDATE POSE ESTIMATE - Calculate weighted average for position, use odometry for heading
    // (Must happen BEFORE resampling to use meaningful weights from correction step)
    real_t sumX = 0, sumY = 0, sumWeights = 0;
    real_t maxWeight = 0;
    real_t bestParticleAvgError = 1000.0; // Track error of best particle

    for (const MCLParticle &particle : particles)
    {
        real_t weight = particle.weight;

        sumX += particle.point.x * weight;
        sumY += particle.point.y * weight;
        sumWeights += weight;

        if (weight > maxWeight)
        {
            maxWeight = weight;
            // Recalculate error for the best particle
            real_t errorSum = 0.0;
            int errorCount = 0;
            for (const MCLDistance &sensor : sensors)
            {
                if (sensor.reading <= 0)
                    continue;

                real_t actualDistance = sensor.reading * MM_TO_IN;
                real_t cosTheta = cos(particle.point.theta);
                real_t sinTheta = sin(particle.point.theta);
                real_t sensorGlobalX = particle.point.x + sensor.pose.x * cosTheta + sensor.pose.y * sinTheta;
                real_t sensorGlobalY = particle.point.y + sensor.pose.y * cosTheta - sensor.pose.x * sinTheta;
                real_t sensorGlobalTheta = particle.point.theta + sensor.pose.theta * M_PI / 180;
                Ray2D ray(Point2D(sensorGlobalX, sensorGlobalY), sensorGlobalTheta);
                real_t expectedDistance = getRayCastDistance(field.getEdges(), ray);
                errorSum += std::abs(actualDistance - expectedDistance);
                errorCount++;
            }
            bestParticleAvgError = errorCount > 0 ? errorSum / errorCount : 1000.0;
        }
    }
    // std::cout << "Max Particle Weight: " << maxWeight << std::endl;
    if (sumWeights > 0)
    {
        estimatedPose.point.x = sumX / sumWeights;
        estimatedPose.point.y = sumY / sumWeights;
        // Use odometry heading directly instead of particle-based rotation
        estimatedPose.point.theta = odomHeading;

        // Mixed MCL - Calculate multi-factor confidence
        sensorConfidence = calculateSensorConfidence(validSensorCount, maxWeight, sensors.size(), bestParticleAvgError);
        motionConfidence = calculateMotionConfidence();
        overallConfidence = combinedConfidence(sensorConfidence, motionConfidence);

        // Update localization mode based on confidence trends
        updateLocalizationMode();
        if (debug && debugCounter >= 50)
        {
            std::cout << "Best Particle Avg Error: " << bestParticleAvgError << " inches" << std::endl;
            std::cout << "Sensor Confidence: " << sensorConfidence << ", Motion Confidence: " << motionConfidence
                      << ", Overall Confidence: " << overallConfidence << std::endl;
        }

        // Get adaptive blend factor based on current mode
        real_t confidence = getAdaptiveBlendFactor();

        // Store simple weight for backwards compatibility
        estimatedPose.weight = overallConfidence;

        // Calculate odometry-based pose from last actualPose + accumulated delta
        Pose2D odomBasedPose;
        odomBasedPose.x = actualPose.x + odomDelta.x;
        odomBasedPose.y = actualPose.y + odomDelta.y;
        odomBasedPose.theta = odomHeading; // Use current odometry heading

        // Adaptive blending based on localization mode
        actualPose.x = confidence * estimatedPose.point.x + (1.0 - confidence) * odomBasedPose.x;
        actualPose.y = confidence * estimatedPose.point.y + (1.0 - confidence) * odomBasedPose.y;
        // Always use odometry heading (no rotation correction)
        actualPose.theta = odomHeading;

        // Debug output for mode and confidence
        if (debug && debugCounter >= 50)
        {
            debugCounter = 0;
            const char *modeStr = currentMode == LocalizationMode::MCL_PRIMARY ? "MCL_PRIMARY" : currentMode == LocalizationMode::ODOMETRY_PRIMARY ? "ODOM_PRIMARY"
                                                                                                                                                   : "RECOVERY";
            std::cout << "Mode: " << modeStr << ", Confidence: " << overallConfidence
                      << " (sensor=" << sensorConfidence << ", motion=" << motionConfidence
                      << "), Blend=" << confidence << std::endl;
        }
    }
    else
    {
        // If sumWeights is 0, just apply odometry
        actualPose.x += odomDelta.x;
        actualPose.y += odomDelta.y;
        actualPose.theta = odomHeading;
    }

    // 4. RESAMPLING STEP - Resample particles based on weights
    std::vector<MCLParticle> newParticles;
    newParticles.reserve(NUM_PARTICLES);

    // Calculate how many random particles to inject based on mode
    int numRandom;
    if (currentMode == LocalizationMode::RECOVERY_MODE)
    {
        // RECOVERY: Very aggressive - inject 50% random particles to search entire field
        numRandom = NUM_PARTICLES / 2; // 50% random particles
        std::cout << "RECOVERY MODE: Injecting " << numRandom << " random particles (50%)" << std::endl;
    }
    else if (overallConfidence < 0.4)
    {
        // Even in normal mode, if confidence is low, inject more random particles
        numRandom = NUM_PARTICLES / 10; // 10% random when struggling
    }
    else
    {
        // Normal operation: small number for kidnapping resistance
        numRandom = randomParticleCount;
    }

    int numResampled = NUM_PARTICLES - numRandom;

    // Guard against edge cases
    if (numResampled < 0)
        numResampled = 0;
    if (numRandom > NUM_PARTICLES)
        numRandom = NUM_PARTICLES;

    // Only do systematic resampling if we have particles to resample
    if (numResampled > 0)
    {
        // Resample particles using systematic resampling
        real_t step = 1.0 / numResampled;
        real_t r = getRandomReal_t(0, step);
        real_t c = particles[0].weight;
        int i = 0;

        for (int m = 0; m < numResampled; m++)
        {
            real_t u = r + m * step;
            while (u > c && i < NUM_PARTICLES - 1)
            {
                i++;
                c += particles[i].weight;
            }
            MCLParticle resampled = particles[i];
            resampled.weight = 1.0 / NUM_PARTICLES; // Reset weight after resampling
            newParticles.push_back(resampled);
        }
    }

    // Add random particles for kidnapping resistance (using current odometry heading)
    for (int j = 0; j < numRandom; j++)
    {
        real_t randomX = getRandomReal_t(-field.getWidth() / 2, field.getWidth() / 2);
        real_t randomY = getRandomReal_t(-field.getHeight() / 2, field.getHeight() / 2);
        // Use current odometry heading instead of random rotation
        newParticles.push_back(MCLParticle(Pose2D(randomX, randomY, odomHeading), 1.0 / NUM_PARTICLES));
    }

    particles = std::move(newParticles);
    odomDelta = Pose2D(0, 0, 0); // Reset odomDelta after update
}

limelib::Pose2D limelib::MCL::getPose(bool radians) const
{
    // If we've never run MCL or have no valid estimate, fall back to pure odometry
    if (estimatedPose.weight <= 0)
    {
        return odomHelper.getPose(radians);
    }

    Pose2D blendedPose;
    blendedPose.x = actualPose.x + odomDelta.x;
    blendedPose.y = actualPose.y + odomDelta.y;
    blendedPose.theta = odomHelper.getPose(radians).theta;
    return blendedPose;
}

limelib::Velocity limelib::MCL::getVelocity() const
{
    return currentVelocity;
}

void limelib::MCL::debugDisplay()
{
    pros::screen::erase();
    pros::screen::set_pen(pros::Color::white);
    int height = field.getHeight();
    int width = field.getWidth();
    pros::screen::draw_rect(10, 10, width + 10, height + 10);
    for (const auto &edge : field.getEdges())
    {
        int x0 = static_cast<int>(edge.start.x) + 10 + width / 2;
        int y0 = static_cast<int>(edge.start.y) + 10 + height / 2;
        int x1 = static_cast<int>(edge.end.x) + 10 + width / 2;
        int y1 = static_cast<int>(edge.end.y) + 10 + height / 2;
        pros::screen::draw_line(x0, y0, x1, y1);
    }
    pros::screen::set_pen(pros::Color::blue);
    for (const auto &particle : particles)
    {
        int px = static_cast<int>(particle.point.x) + 10 + width / 2;
        int py = static_cast<int>(particle.point.y) + 10 + height / 2;
        pros::screen::draw_circle(px, py, 1);
    }
    int ax = static_cast<int>(actualPose.x) + 10 + width / 2;
    int ay = static_cast<int>(actualPose.y) + 10 + height / 2;
    pros::screen::set_pen(pros::Color::red);
    pros::screen::draw_circle(ax, ay, 2);
}

// Add these implementations for the Locator base class
limelib::Pose2D limelib::Locator::update()
{
    return Pose2D(); // Default implementation
}

void limelib::Locator::calibrate()
{
    // Default implementation
}

void limelib::Locator::setPose(Pose2D pose)
{
    // Default implementation
}

limelib::real_t limelib::getRayCastDistance(const std::vector<LineSegment2D> &edges, Ray2D ray)
{
    real_t min_distance = 1000;

    // Pre-calculate ray direction for 0°=+Y, 90°=+X coordinate system
    // direction = (sin(θ), cos(θ))
    real_t ray_dx = std::sin(ray.radians); // X component of direction
    real_t ray_dy = std::cos(ray.radians); // Y component of direction

    for (const auto &edge : edges)
    {
        // Get line segment vectors
        real_t dx = edge.end.x - edge.start.x;
        real_t dy = edge.end.y - edge.start.y;

        // Vector from ray start to line segment start
        real_t px = edge.start.x - ray.start.x;
        real_t py = edge.start.y - ray.start.y;

        // Calculate determinant for intersection
        real_t det = ray_dx * dy - ray_dy * dx;

        // Skip if ray is parallel to line segment (det ≈ 0)
        if (std::abs(det) < 1e-9)
        {
            continue;
        }

        // Calculate intersection parameters
        real_t u = (ray_dy * px - ray_dx * py) / det; // Parameter along line segment
        real_t t = (dy * px - dx * py) / det;         // Parameter along ray

        // Check if intersection is valid:
        // - t > 0: intersection is in front of ray (not behind)
        // - 0 <= u <= 1: intersection is within line segment bounds
        if (t > 1e-9 && u >= 0.0 && u <= 1.0)
        {
            min_distance = std::min(min_distance, t);
        }
    }

    return min_distance;
}

// Mixed MCL - Confidence Calculation Helpers

limelib::real_t limelib::MCL::calculateSensorConfidence(int validSensors, real_t maxWeight, int totalSensors, real_t avgError)
{
    // Component 1: Sensor availability (more valid sensors = higher confidence)
    real_t sensorAvailability = static_cast<real_t>(validSensors) / totalSensors;

    // Component 2: Particle agreement (higher max weight = particles agree more)
    // maxWeight is already normalized, multiply by NUM_PARTICLES for scaling
    real_t particleAgreement = maxWeight * NUM_PARTICLES;
    particleAgreement = std::clamp(particleAgreement, 0.0f, 1.0f);

    // Component 3: Error magnitude (NEW - addresses the issue!)
    // Low error = high confidence, high error = low confidence
    // Use exponential decay: good at 2in, poor at 10in
    real_t errorConfidence = std::exp(-avgError / 5.0); // e^(-error/5)
    // At 2 inches: e^(-0.4) ≈ 0.67
    // At 5 inches: e^(-1.0) ≈ 0.37
    // At 10 inches: e^(-2.0) ≈ 0.14
    // At 40 inches: e^(-8.0) ≈ 0.0003 (nearly zero!)

    // Combine all three factors (geometric mean ensures all contribute)
    real_t sensorConf = std::pow(sensorAvailability * particleAgreement * errorConfidence, 1.0 / 3.0);
    if (validSensors < 2)
    {
        sensorConf *= 0.5; // Heavily penalize low sensor count
    }

    return std::clamp(sensorConf, 0.0f, 1.0f);
}

limelib::real_t limelib::MCL::calculateMotionConfidence()
{
    // Calculate recent motion magnitude
    real_t deltaDistance = std::sqrt(odomDelta.x * odomDelta.x + odomDelta.y * odomDelta.y);

    // Motion confidence decreases with speed (odometry drift increases)
    // At low speeds: high confidence in odometry
    // At high speeds: lower confidence due to wheel slip, etc.
    real_t maxTrustedSpeed = 5.0; // inches per update cycle (adjust based on your robot)
    real_t speedRatio = deltaDistance / maxTrustedSpeed;

    // Exponential decay: slow = 1.0, fast = approaches 0
    real_t motionConf = std::exp(-speedRatio);

    return std::clamp(motionConf, 0.2f, 1.0f); // Never go below 0.2
}

limelib::real_t limelib::MCL::combinedConfidence(real_t sensorConf, real_t motionConf)
{
    // Weight sensor confidence more heavily (70/30 split)
    // Sensor data is more reliable for absolute position
    real_t combined = 0.7 * sensorConf + 0.3 * motionConf;

    return std::clamp(combined, 0.1f, 0.9f);
}

void limelib::MCL::updateLocalizationMode()
{
    // Track confidence trends
    if (overallConfidence > 0.6)
    {
        consecutiveHighConfidence++;
        consecutiveLowConfidence = 0;
    }
    else if (overallConfidence < 0.3)
    {
        consecutiveLowConfidence++;
        consecutiveHighConfidence = 0;
    }
    else
    {
        // Medium confidence - decay counters
        consecutiveHighConfidence = std::max(0, consecutiveHighConfidence - 1);
        consecutiveLowConfidence = std::max(0, consecutiveLowConfidence - 1);
    }

    // State machine for mode transitions
    switch (currentMode)
    {
    case LocalizationMode::ODOMETRY_PRIMARY:
        // Transition to MCL_PRIMARY if we have sustained high confidence
        if (consecutiveHighConfidence >= 5)
        {
            currentMode = LocalizationMode::MCL_PRIMARY;
            consecutiveLowConfidence = 0; // Reset counter
            std::cout << "Mode: ODOMETRY_PRIMARY -> MCL_PRIMARY" << std::endl;
        }
        // Transition to RECOVERY if we're stuck with low confidence
        // Reduced threshold from 20 to 10 for faster recovery
        else if (consecutiveLowConfidence >= 10)
        {
            currentMode = LocalizationMode::RECOVERY_MODE;
            std::cout << "Mode: ODOMETRY_PRIMARY -> RECOVERY_MODE (stuck at low confidence)" << std::endl;
        }
        break;

    case LocalizationMode::MCL_PRIMARY:
        // Transition to ODOMETRY_PRIMARY if confidence drops
        if (consecutiveLowConfidence >= 10)
        {
            currentMode = LocalizationMode::ODOMETRY_PRIMARY;
            std::cout << "Mode: MCL_PRIMARY -> ODOMETRY_PRIMARY" << std::endl;
        }
        // Transition to RECOVERY if we're completely lost
        else if (overallConfidence < 0.15 && consecutiveLowConfidence >= 5)
        {
            currentMode = LocalizationMode::RECOVERY_MODE;
            std::cout << "Mode: MCL_PRIMARY -> RECOVERY_MODE" << std::endl;
        }
        break;

    case LocalizationMode::RECOVERY_MODE:
        // Exit recovery when we find good localization
        if (consecutiveHighConfidence >= 3)
        {
            currentMode = LocalizationMode::ODOMETRY_PRIMARY;
            consecutiveLowConfidence = 0; // Reset for fresh start
            std::cout << "Mode: RECOVERY_MODE -> ODOMETRY_PRIMARY (recovered!)" << std::endl;
        }
        // If we're stuck in recovery for too long without improving, reset more aggressively
        else if (consecutiveLowConfidence >= 30)
        {
            std::cout << "RECOVERY: Still stuck after 30 cycles, re-dispersing all particles!" << std::endl;
            // Force complete particle reset (will happen in next resampling)
            consecutiveLowConfidence = 0; // Reset to stay in recovery mode
        }
        break;
    }
}

limelib::real_t limelib::MCL::getAdaptiveBlendFactor()
{
    // Return blend factor for MCL vs odometry
    // Higher value = trust MCL more
    // Lower value = trust odometry more

    switch (currentMode)
    {
    case LocalizationMode::MCL_PRIMARY:
        // High confidence: trust MCL heavily (70-90%)
        return std::clamp(0.7f + overallConfidence * 0.2f, 0.7f, 0.9f);

    case LocalizationMode::ODOMETRY_PRIMARY:
        // Low confidence: trust odometry more (10-50%)
        return std::clamp(overallConfidence * 0.5f, 0.1f, 0.5f);

    case LocalizationMode::RECOVERY_MODE:
        // Recovery: very aggressive MCL (90-95%)
        return std::clamp(0.9f + overallConfidence * 0.05f, 0.9f, 0.95f);

    default:
        return 0.5f;
    }
}
