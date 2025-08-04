#ifndef LOCALIZATION_HPP
#define LOCALIZATION_HPP

#include "limelib/types.hpp"
#include "limelib/pose2d.hpp"
#include "limelib/trackingWheel.hpp"
#include "limelib/field2d.hpp"
#include "pros/imu.hpp"

namespace limelib
{
    class Localization
    {
    public:
        virtual void initalize();
        virtual void update();
        virtual void calibrate();
        virtual Pose2D getPose() const = 0;
    };

    class Odometry : public Localization
    {
    public:
        Odometry(TrackingWheel *verticalTW, TrackingWheel *horizontalTW, pros::IMU &imu);
        void initalize() override;
        void update() override;
        void calibrate() override;
        Pose2D getPose() const override;
        void setPose(Pose2D pose);
        void setPose(real_t x, real_t y, real_t theta);

    private:
        TrackingWheel *verticalTW;
        TrackingWheel *horizontalTW;
        pros::IMU &imu;
        Pose2D currentPose;
    };
    class MCL : public Localization
    {
    public:
        MCL(TrackingWheel *verticalTW, TrackingWheel *horizontalTW, pros::Imu &imu, Field2D &field, int num_particles, int rotationNoise, int translationNoise);
        void initalize() override;
        void calibrate() override;
        void update() override;
        Pose2D getPose() const override;

    private:
        TrackingWheel &verticalTW;
        TrackingWheel &horizontalTW;
        pros::Imu &imu;
        Field2D &field;
        Pose2D currentPose;
        int NUM_PARTICLES;
        int ROTATION_NOISE;
        int TRANSLATION_NOISE;
    };
}

#endif