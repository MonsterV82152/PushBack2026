#ifndef LOCALIZATION_HPP
#define LOCALIZATION_HPP

#include "limelib/types.hpp"
#include "limelib/pose2d.hpp"
#include "limelib/trackingWheel.hpp"
#include "limelib/field2d.hpp"
#include "limelib/ray2d.hpp"
#include "limelib/mclDistance.hpp"
#include "pros/imu.hpp"
#include "pros/rtos.hpp"

namespace limelib
{
    class Locator
    {
    public:
        virtual Pose2D update();
        virtual void calibrate();
        virtual Pose2D getPose() const = 0;
        virtual void setPose(Pose2D pose);
        void setPose(real_t x, real_t y, real_t theta);
    };

    class Odometry : public Locator
    {
    public:
        Odometry(TrackingWheel *verticalTW, TrackingWheel *horizontalTW, pros::IMU &imu);
        Pose2D update() override;
        void calibrate() override;
        Pose2D getPose() const override;
        void setPose(Pose2D pose) override;

    private:
        TrackingWheel *verticalTW;
        TrackingWheel *horizontalTW;
        pros::IMU &imu;
        Pose2D currentPose; 
        real_t headingOffset;
        void task(void *params);
    };

    class MCL : public Locator
    {
    public:
        MCL(TrackingWheel *verticalTW, TrackingWheel *horizontalTW, pros::Imu &imu, std::vector<MCLDistance>& sensors, Field2D &field, int num_particles, int rotationNoise, int translationNoise, int intensitivity = 10);
        void calibrate() override;
        Pose2D update() override;
        Pose2D getPose() const override;
        void setPose(Pose2D pose) override;

    private:
        Odometry odomHelper;
        std::vector<MCLDistance>& sensors;
        Field2D &field;
        Pose2D odomDelta;
        Pose2D poseOffset;
        Pose2D actualPose;
        MCLParticle estimatedPose;
        int NUM_PARTICLES;
        int ROTATION_NOISE;
        int TRANSLATION_NOISE;
        int INTENSITY;
        int last_mcl_update;
        int randomParticleCount;
        std::vector<MCLParticle> particles;
        void updateMCL();
        
    };
    real_t getRayCastDistance(const std::vector<LineSegment2D> &edges, Ray2D ray);

    struct MCLParticle
    {
        Pose2D point;
        real_t weight;

        MCLParticle(Pose2D point = Pose2D(), real_t weight = 0) : point(point), weight(weight) {}
    };
}

#endif