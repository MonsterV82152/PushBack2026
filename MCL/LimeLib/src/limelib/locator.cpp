#include "limelib/locator.hpp"

void limelib::Locator::setPose(real_t x, real_t y, real_t theta)
{
    this->setPose(limelib::Pose2D(x, y, theta));
}

limelib::Odometry::Odometry(TrackingWheel *verticalTW, TrackingWheel *horizontalTW, pros::IMU &imu, bool shouldTaskRun)
    : verticalTW(verticalTW), horizontalTW(horizontalTW), imu(imu), currentPose(0, 0, 0), headingOffset(0), shouldTaskRun(shouldTaskRun)
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
    pros::delay(1000);
    if (shouldTaskRun)
    {
        pros::Task odomTask([this]()
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
    real_t heading = imu.get_heading() * M_PI / 180 + headingOffset;
    real_t headingDiff = heading - currentPose.theta;
    real_t vertDist = verticalTW != nullptr ? verticalTW->getDistanceTravelled() + verticalTW->getOffset() * headingDiff / 2 : 0;
    real_t horDist = horizontalTW != nullptr ? horizontalTW->getDistanceTravelled() + horizontalTW->getOffset() * headingDiff / 2 : 0;

    real_t xChange = vertDist * sin(heading) + horDist * cos(heading);
    real_t yChange = vertDist * cos(heading) - horDist * sin(heading);

    currentPose.x += xChange;
    currentPose.y += yChange;
    currentPose.theta = heading;

    return Pose2D(xChange, yChange, headingDiff);
}

limelib::Pose2D limelib::Odometry::getPose(bool radians) const
{
    return radians ? currentPose : currentPose.toDegrees();
}

limelib::MCL::MCL(TrackingWheel *verticalTW, TrackingWheel *horizontalTW,
                  pros::Imu &imu, std::vector<MCLDistance> &sensors, Field2D &field, int num_particles, int rotationNoise, int translationNoise, bool debug,
                  int intensity, bool shouldTaskRun)
    : odomHelper(verticalTW, horizontalTW, imu, false), sensors(sensors), field(field), NUM_PARTICLES(num_particles),
      ROTATION_NOISE(rotationNoise), TRANSLATION_NOISE(translationNoise), debug(debug), INTENSITY(intensity), last_mcl_update(intensity), randomParticleCount(0), shouldTaskRun(shouldTaskRun)
{
    // Initialize particles with random positions but heading will be set from odometry later
    for (int i = 0; i < NUM_PARTICLES; i++)
    {
        // Start with 0 heading, will be updated to odometry heading in first update
        particles.push_back(MCLParticle(Pose2D(getRandomReal_t(-field.getWidth() / 2, field.getWidth() / 2), getRandomReal_t(-field.getHeight() / 2, field.getHeight() / 2), 0), 0));
    }
}

void limelib::MCL::calibrate()
{
    odomHelper.calibrate();
    std::cout << "Field Edges: " << field.getEdges().size() << std::endl;
    if (shouldTaskRun)
    {
        pros::Task mclTask([this]()
                           {
            while (true)
            {
                update();
                pros::delay(10);
            } });
    }
}

void limelib::MCL::setPose(limelib::Pose2D pose)
{
    odomHelper.setPose(pose);
    particles.clear();
    // Initialize particles with position spread but all using the same odometry heading
    real_t odomHeading = pose.theta * M_PI / 180; // Convert to radians
    for (int i = 0; i < NUM_PARTICLES; i++)
    {
        particles.push_back(MCLParticle(Pose2D(pose.x + getRandomReal_t(-5, 5), pose.y + getRandomReal_t(-5, 5), odomHeading), 0));
    }
}

void limelib::MCL::setPose(real_t x, real_t y, real_t theta)
{
    setPose(limelib::Pose2D(x, y, theta));
}

limelib::Pose2D limelib::MCL::update()
{
    if (last_mcl_update > INTENSITY)
    {
        updateMCL();
        if (debug)
            debugDisplay();
        last_mcl_update = 0;
        Pose2D pose = getPose();
        std::cout << "Estimated Pose: X=" << pose.x << ", Y=" << pose.y << ", Theta=" << pose.theta * 180 / M_PI << ", Confidence=" << estimatedPose.weight << std::endl;
    }
    else
    {
        last_mcl_update++;
        Pose2D currentDelta = odomHelper.update();
        odomDelta.x += currentDelta.x;
        odomDelta.y += currentDelta.y;
        odomDelta.theta += currentDelta.theta;
        // Normalize theta to [0, 2π)
        while (odomDelta.theta < 0)
            odomDelta.theta += 2 * M_PI;
        while (odomDelta.theta >= 2 * M_PI)
            odomDelta.theta -= 2 * M_PI;
    }
    return odomDelta;
}

void limelib::MCL::updateMCL()
{
    // 1. PREDICTION STEP - Apply odometry motion to all particles
    Pose2D currentDelta = odomHelper.update();
    odomDelta.x += currentDelta.x;
    odomDelta.y += currentDelta.y;
    odomDelta.theta += currentDelta.theta;

    // Get current odometry heading for all particles
    real_t odomHeading = odomHelper.getPose(true).theta;

    for (MCLParticle &particle : particles)
    {
        // Apply only translation motion with noise, use odometry heading for all particles
        particle.point.x += odomDelta.x + getRandomReal_t(-TRANSLATION_NOISE, TRANSLATION_NOISE);
        particle.point.y += odomDelta.y + getRandomReal_t(-TRANSLATION_NOISE, TRANSLATION_NOISE);
        // Set all particles to use the same odometry heading (no rotation correction)
        particle.point.theta = odomHeading;
    }
    // 2. CORRECTION STEP - Update particle weights based on sensor observations
    real_t totalWeight = 0.0;
    for (MCLDistance &sensor : sensors)
    {
        sensor.reading = sensor.sensor.get_distance();
    }
    for (MCLParticle &particle : particles)
    {
        real_t likelihood = 1.0;

        // For each sensor, compare actual vs expected readings
        for (const MCLDistance &sensor : sensors)
        {
            // Get actual sensor reading
            real_t actualDistance = sensor.reading * MM_TO_IN;
            if (sensor.reading < 0 || sensor.reading >= 9999)
                continue; // Skip invalid readings

            // Calculate expected sensor reading from this particle's position
            // Transform sensor position to global coordinates
            real_t sensorGlobalX = particle.point.x + sensor.pose.x * cos(particle.point.theta) + sensor.pose.y * sin(particle.point.theta);
            real_t sensorGlobalY = particle.point.y - sensor.pose.x * sin(particle.point.theta) + sensor.pose.y * cos(particle.point.theta);
            real_t sensorGlobalTheta = particle.point.theta + sensor.pose.theta * M_PI / 180;

            // Cast ray from sensor position in sensor direction
            Ray2D ray(Point2D(sensorGlobalX, sensorGlobalY), sensorGlobalTheta);
            real_t expectedDistance = getRayCastDistance(field.getEdges(), ray);

            // Calculate likelihood based on how well actual matches expected
            real_t error = std::abs(actualDistance - expectedDistance);
            real_t sensorLikelihood = exp(-error * error / (2.0 * 100.0)); // Gaussian with σ=10

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

    // 3. RESAMPLING STEP - Resample particles based on weights
    std::vector<MCLParticle> newParticles;
    newParticles.reserve(NUM_PARTICLES);

    // Calculate how many random particles to inject (for kidnapping resistance)
    int numRandom = randomParticleCount;
    int numResampled = NUM_PARTICLES - numRandom;

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
        newParticles.push_back(particles[i]);

        // Reset weight for next iteration
        newParticles.back().weight = 0.0;
    }

    // Add random particles for kidnapping resistance (using current odometry heading)
    for (int i = 0; i < numRandom; i++)
    {
        real_t randomX = getRandomReal_t(-field.getWidth() / 2, field.getWidth() / 2);
        real_t randomY = getRandomReal_t(-field.getHeight() / 2, field.getHeight() / 2);
        // Use current odometry heading instead of random rotation
        newParticles.push_back(MCLParticle(Pose2D(randomX, randomY, odomHeading), 0.0));
    }

    particles = std::move(newParticles);

    // 4. UPDATE POSE ESTIMATE - Calculate weighted average for position, use odometry for heading
    real_t sumX = 0, sumY = 0, sumWeights = 0;
    real_t maxWeight = 0;

    for (const MCLParticle &particle : particles)
    {
        real_t weight = (particle.weight > 0) ? particle.weight : 1.0 / NUM_PARTICLES;

        sumX += particle.point.x * weight;
        sumY += particle.point.y * weight;
        sumWeights += weight;
        maxWeight = std::max(maxWeight, weight);
    }
    if (sumWeights > 0)
    {
        estimatedPose.point.x = sumX / sumWeights;
        estimatedPose.point.y = sumY / sumWeights;
        // Use odometry heading directly instead of particle-based rotation
        estimatedPose.point.theta = odomHeading;

        // Calculate confidence based on particle concentration
        // Higher max weight = more particles agree = higher confidence
        estimatedPose.weight = maxWeight * NUM_PARTICLES; // Normalized confidence [0, 1]

        // Alternative confidence calculation based on effective sample size
        // real_t ess = 1.0 / std::accumulate(particles.begin(), particles.end(), 0.0,
        //     [](real_t sum, const MCLParticle& p) { return sum + p.weight * p.weight; });
        // estimatedPose.weight = ess / NUM_PARTICLES;

        // Use confidence to blend MCL estimate with odometry
        real_t confidence = estimatedPose.weight;
        confidence = std::clamp(confidence, 0.1f, 0.9f); // Prevent full reliance on either source

        // Calculate odometry-based pose from last actualPose + accumulated delta
        Pose2D odomBasedPose;
        odomBasedPose.x = actualPose.x + odomDelta.x;
        odomBasedPose.y = actualPose.y + odomDelta.y;
        odomBasedPose.theta = odomHeading; // Use current odometry heading

        // Blend only X and Y positions based on confidence, use odometry heading directly
        actualPose.x = confidence * estimatedPose.point.x + (1.0 - confidence) * odomBasedPose.x;
        actualPose.y = confidence * estimatedPose.point.y + (1.0 - confidence) * odomBasedPose.y;
        // Always use odometry heading (no rotation correction)
        actualPose.theta = odomHeading;
    }
    odomDelta = Pose2D(0, 0, 0); // Reset odomDelta after update
}

limelib::Pose2D limelib::MCL::getPose(bool radians) const
{
    // If we've never run MCL or have no valid estimate, fall back to pure odometry
    if (estimatedPose.weight <= 0)
    {
        return odomHelper.getPose(radians);
    }

    // Calculate age factor - confidence decreases with time since last MCL update
    real_t ageFactor = exp(-static_cast<real_t>(last_mcl_update) / (INTENSITY * 0.5));
    ageFactor = std::max(ageFactor, real_t(0.1)); // Don't let age factor go below 0.1

    // Calculate effective confidence considering both MCL confidence and age
    real_t baseConfidence = std::clamp(estimatedPose.weight, real_t(0.1), real_t(0.9));
    real_t effectiveConfidence = baseConfidence * ageFactor;
    effectiveConfidence = std::clamp(effectiveConfidence, real_t(0.1), real_t(0.9));

    // Get current odometry heading
    real_t currentOdomHeading = odomHelper.getPose(true).theta; // Already in radians

    // Calculate current odometry-based pose from last MCL estimate + accumulated delta
    Pose2D currentOdomPose;
    currentOdomPose.x = actualPose.x + odomDelta.x;
    currentOdomPose.y = actualPose.y + odomDelta.y;
    currentOdomPose.theta = currentOdomHeading; // Convert to radians for internal calculations

    // If confidence is very low (old or unreliable MCL), use odometry position with odometry heading
    if (effectiveConfidence < 0.2)
    {
        return Pose2D(currentOdomPose.x, currentOdomPose.y, radians ? currentOdomHeading : currentOdomHeading * 180 / M_PI); // Return with heading in degrees
    }

    // Blend only X and Y positions based on MCL confidence, always use odometry heading
    Pose2D blendedPose;
    blendedPose.x = effectiveConfidence * estimatedPose.point.x + (1.0 - effectiveConfidence) * currentOdomPose.x;
    blendedPose.y = effectiveConfidence * estimatedPose.point.y + (1.0 - effectiveConfidence) * currentOdomPose.y;
    // Always use current odometry heading (no rotation correction from MCL)
    blendedPose.theta = radians ? currentOdomHeading : currentOdomHeading * 180 / M_PI; // In degrees for return

    return blendedPose;
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

    // Pre-calculate ray direction for efficiency
    real_t ray_cos = std::sin(ray.radians);
    real_t ray_sin = std::cos(ray.radians);

    for (const auto &edge : edges)
    {
        // Get line segment vectors
        real_t dx = edge.end.x - edge.start.x;
        real_t dy = edge.end.y - edge.start.y;

        // Vector from ray start to line segment start
        real_t px = edge.start.x - ray.start.x;
        real_t py = edge.start.y - ray.start.y;

        // Calculate determinant for intersection
        real_t det = ray_cos * dy - ray_sin * dx;

        // Skip if ray is parallel to line segment (det ≈ 0)
        if (std::abs(det) < 1e-9)
        {
            continue;
        }

        // Calculate intersection parameters
        real_t u = (ray_sin * px - ray_cos * py) / det; // Parameter along line segment
        real_t t = (dy * px - dx * py) / det;           // Parameter along ray

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
