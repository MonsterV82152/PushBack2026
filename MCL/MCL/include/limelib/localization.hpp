#ifndef LOCALIZATION_HPP
#define LOCALIZATION_HPP

#include "limelib/types.hpp"
#include "limelib/pose2d.hpp"
#include "limelib/trackingWheel.hpp"
#include "pros/imu.hpp"

namespace limelib
{
    class Localization
    {
    public:
        virtual void update();
        virtual Pose2D getPose() const = 0;
    };

    class Odometry : public Localization
    {
    public:
        Odometry(TrackingWheel verticalTW, TrackingWheel horizontalTW, pros::IMU *imu);
        void update() override;
        Pose2D getPose() const override;
    private:
        TrackingWheel verticalTW;
        TrackingWheel horizontalTW;
        pros::IMU *imu;
        Pose2D currentPose;
    };
    class MCL : public Localization
    {
    public:
        MCL(TrackingWheel verticalTW, TrackingWheel horizontalTW, pros::IMU *imu, int num_particles, int rotationNoise, int translationNoise);
        void update() override;
        Pose2D getPose() const override;
    private:
        TrackingWheel verticalTW;
        TrackingWheel horizontalTW;
        pros::IMU *imu;
        Pose2D currentPose;
        int NUM_PARTICLES;
        int ROTATION_NOISE;
        int TRANSLATION_NOISE;

    };
}

#endif