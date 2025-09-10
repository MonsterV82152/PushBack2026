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
    if (shouldTaskRun) {
        pros::Task odomTask([this]() {
            while (true)
            {
                update();
                pros::delay(10);
            }
        });
    }
}

void limelib::Odometry::setPose(limelib::Pose2D pose)
{
    headingOffset = (-imu.get_heading() + pose.theta) * M_PI / 180;
    currentPose = pose;
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

limelib::Pose2D limelib::Odometry::getPose() const
{
    return currentPose.toDegrees();
}

limelib::MCL::MCL(TrackingWheel *verticalTW, TrackingWheel *horizontalTW,
                  pros::Imu &imu, std::vector<MCLDistance> &sensors, Field2D &field, int num_particles, int rotationNoise, int translationNoise,
                  int intensity, bool shouldTaskRun)
    : odomHelper(verticalTW, horizontalTW, imu, false), sensors(sensors), field(field), NUM_PARTICLES(num_particles),
      ROTATION_NOISE(rotationNoise), TRANSLATION_NOISE(translationNoise), INTENSITY(intensity), last_mcl_update(intensity), randomParticleCount(num_particles / 50), shouldTaskRun(shouldTaskRun)
{
    for (int i = 0; i < NUM_PARTICLES; i++)
    {
        particles.push_back(MCLParticle(Pose2D(getRandomReal_t(-field.getWidth(), field.getWidth()), getRandomReal_t(-field.getHeight(), field.getHeight()), getRandomReal_t(0, M_PI * 2)), 0));
    }
}

void limelib::MCL::calibrate()
{
    odomHelper.calibrate();
    if (shouldTaskRun) {
        pros::Task mclTask([this]() {
            while (true)
            {
                update();
                pros::delay(10);
            }
        });
    }
}

void limelib::MCL::setPose(limelib::Pose2D pose)
{
    odomHelper.setPose(pose);
    particles.clear();
    for (int i = 0; i < NUM_PARTICLES; i++)
    {
        particles.push_back(MCLParticle(Pose2D(pose.x + getRandomReal_t(-5, 5), pose.y + getRandomReal_t(-5, 5), getRandomReal_t(0, M_PI * 2)), 0));
    }
}

limelib::Pose2D limelib::MCL::update()
{
    if (last_mcl_update > INTENSITY)
    {
        updateMCL();
        last_mcl_update = 0;
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

    for (MCLParticle &particle : particles)
    {
        // Apply motion model with additive noise (not multiplicative)
        particle.point.x += odomDelta.x + getRandomReal_t(-TRANSLATION_NOISE, TRANSLATION_NOISE);
        particle.point.y += odomDelta.y + getRandomReal_t(-TRANSLATION_NOISE, TRANSLATION_NOISE);
        particle.point.theta += odomDelta.theta + getRandomReal_t(-ROTATION_NOISE, ROTATION_NOISE);

        // Normalize theta to [0, 2π)
        while (particle.point.theta < 0)
            particle.point.theta += 2 * M_PI;
        while (particle.point.theta >= 2 * M_PI)
            particle.point.theta -= 2 * M_PI;
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

    // Add random particles for kidnapping resistance
    for (int i = 0; i < numRandom; i++)
    {
        real_t randomX = getRandomReal_t(-field.getWidth() / 2, field.getWidth() / 2);
        real_t randomY = getRandomReal_t(-field.getHeight() / 2, field.getHeight() / 2);
        real_t randomTheta = getRandomReal_t(0, 2 * M_PI);
        newParticles.push_back(MCLParticle(Pose2D(randomX, randomY, randomTheta), 0.0));
    }

    particles = std::move(newParticles);

    // 4. UPDATE POSE ESTIMATE - Calculate weighted average and confidence
    real_t sumX = 0, sumY = 0, sumSinTheta = 0, sumCosTheta = 0, sumWeights = 0;
    real_t maxWeight = 0;

    for (const MCLParticle &particle : particles)
    {
        real_t weight = (particle.weight > 0) ? particle.weight : 1.0 / NUM_PARTICLES;

        sumX += particle.point.x * weight;
        sumY += particle.point.y * weight;
        sumSinTheta += sin(particle.point.theta) * weight;
        sumCosTheta += cos(particle.point.theta) * weight;
        sumWeights += weight;
        maxWeight = std::max(maxWeight, weight);
    }
    if (sumWeights > 0)
    {
        estimatedPose.point.x = sumX / sumWeights;
        estimatedPose.point.y = sumY / sumWeights;
        estimatedPose.point.theta = atan2(sumSinTheta / sumWeights, sumCosTheta / sumWeights);

        // Normalize theta to [0, 2π) to match your convention
        while (estimatedPose.point.theta < 0)
            estimatedPose.point.theta += 2 * M_PI;
        while (estimatedPose.point.theta >= 2 * M_PI)
            estimatedPose.point.theta -= 2 * M_PI;

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
        odomBasedPose.theta = actualPose.theta + odomDelta.theta;

        // Normalize odometry theta
        while (odomBasedPose.theta < 0)
            odomBasedPose.theta += 2 * M_PI;
        while (odomBasedPose.theta >= 2 * M_PI)
            odomBasedPose.theta -= 2 * M_PI;

        // Blend poses based on confidence
        actualPose.x = confidence * estimatedPose.point.x + (1.0 - confidence) * odomBasedPose.x;
        actualPose.y = confidence * estimatedPose.point.y + (1.0 - confidence) * odomBasedPose.y;

        // Handle angle blending carefully (circular interpolation)
        real_t mclSin = sin(estimatedPose.point.theta);
        real_t mclCos = cos(estimatedPose.point.theta);
        real_t odomSin = sin(odomBasedPose.theta);
        real_t odomCos = cos(odomBasedPose.theta);

        real_t blendedSin = confidence * mclSin + (1.0 - confidence) * odomSin;
        real_t blendedCos = confidence * mclCos + (1.0 - confidence) * odomCos;
        actualPose.theta = atan2(blendedSin, blendedCos);

        // Normalize final theta
        while (actualPose.theta < 0)
            actualPose.theta += 2 * M_PI;
        while (actualPose.theta >= 2 * M_PI)
            actualPose.theta -= 2 * M_PI;
    }
    odomDelta = Pose2D(0, 0, 0); // Reset odomDelta after update
}


limelib::Pose2D limelib::MCL::getPose() const
{
    // If we've never run MCL or have no valid estimate, fall back to pure odometry
    if (estimatedPose.weight <= 0) {
        return odomHelper.getPose();
    }
    
    // Calculate age factor - confidence decreases with time since last MCL update
    real_t ageFactor = exp(-static_cast<real_t>(last_mcl_update) / (INTENSITY * 0.5));
    ageFactor = std::max(ageFactor, real_t(0.1)); // Don't let age factor go below 0.1
    
    // Calculate effective confidence considering both MCL confidence and age
    real_t baseConfidence = std::clamp(estimatedPose.weight, real_t(0.1), real_t(0.9));
    real_t effectiveConfidence = baseConfidence * ageFactor;
    effectiveConfidence = std::clamp(effectiveConfidence, real_t(0.1), real_t(0.9));
    
    // Calculate current odometry-based pose from last MCL estimate + accumulated delta
    Pose2D currentOdomPose;
    currentOdomPose.x = actualPose.x + odomDelta.x;
    currentOdomPose.y = actualPose.y + odomDelta.y;
    currentOdomPose.theta = actualPose.theta + odomDelta.theta;
    
    // Normalize odometry theta
    while (currentOdomPose.theta < 0) currentOdomPose.theta += 2 * M_PI;
    while (currentOdomPose.theta >= 2 * M_PI) currentOdomPose.theta -= 2 * M_PI;
    
    // If confidence is very low (old or unreliable MCL), use mostly odometry
    if (effectiveConfidence < 0.2) {
        return currentOdomPose;
    }
    
    // Blend MCL estimate with current odometry interpolation
    Pose2D blendedPose;
    blendedPose.x = effectiveConfidence * estimatedPose.point.x + (1.0 - effectiveConfidence) * currentOdomPose.x;
    blendedPose.y = effectiveConfidence * estimatedPose.point.y + (1.0 - effectiveConfidence) * currentOdomPose.y;
    
    // Handle angle blending with circular interpolation
    real_t mclSin = sin(estimatedPose.point.theta);
    real_t mclCos = cos(estimatedPose.point.theta);
    real_t odomSin = sin(currentOdomPose.theta);
    real_t odomCos = cos(currentOdomPose.theta);
    
    real_t blendedSin = effectiveConfidence * mclSin + (1.0 - effectiveConfidence) * odomSin;
    real_t blendedCos = effectiveConfidence * mclCos + (1.0 - effectiveConfidence) * odomCos;
    blendedPose.theta = atan2(blendedSin, blendedCos);
    
    // Normalize final theta
    while (blendedPose.theta < 0) blendedPose.theta += 2 * M_PI;
    while (blendedPose.theta >= 2 * M_PI) blendedPose.theta -= 2 * M_PI;
    
    return blendedPose.toDegrees();
}

// Add these implementations for the Locator base class
limelib::Pose2D limelib::Locator::update() {
    return Pose2D();  // Default implementation
}

void limelib::Locator::calibrate() {
    // Default implementation
}

void limelib::Locator::setPose(Pose2D pose) {
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
