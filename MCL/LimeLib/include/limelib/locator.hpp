#ifndef LOCALIZATION_HPP
#define LOCALIZATION_HPP

#include "limelib/types.hpp"
#include "limelib/geometry/pose2d.hpp"
#include "limelib/trackingWheel.hpp"
#include "limelib/geometry/field2d.hpp"
#include "limelib/geometry/ray2d.hpp"
#include "limelib/mclDistance.hpp"
#include "pros/imu.hpp"
#include "pros/rtos.hpp"
#include "pros/screen.hpp"

namespace limelib
{
    struct MCLParticle
    {
        Pose2D point;
        real_t weight;

        MCLParticle(Pose2D point = Pose2D(), real_t weight = 0) : point(point), weight(weight) {}
    };
    real_t getRayCastDistance(const std::vector<LineSegment2D> &edges, Ray2D ray);

    class Locator
    {
    public:
        // Add virtual destructor
        virtual ~Locator() = default;
        virtual Pose2D update();
        virtual void calibrate();
        virtual Pose2D getPose(bool radians = false) const = 0;
        virtual void setPose(Pose2D pose);
        virtual void setPose(real_t x, real_t y, real_t theta);
    };

    class Odometry : public Locator
    {
    public:
        Odometry(TrackingWheel *verticalTW, TrackingWheel *horizontalTW, pros::IMU &imu, bool shouldTaskRun = true);
        Pose2D update() override;
        void calibrate() override;
        Pose2D getPose(bool radians = false) const override;
        void setPose(Pose2D pose) override;
        void setPose(real_t x, real_t y, real_t theta) override;

    private:
        TrackingWheel *verticalTW;
        TrackingWheel *horizontalTW;
        pros::IMU &imu;
        Pose2D currentPose;
        real_t headingOffset;
        bool shouldTaskRun;
    };

    class MCL : public Locator
    {
    public:
        /**
         * MCL Localization Constructor
         * @param verticalTW Pointer to the vertical tracking wheel
         * @param horizontalTW Pointer to the horizontal tracking wheel
         * @param imu Reference to the IMU sensor
         * @param sensors Vector of MCLDistance sensors for distance measurements
         * @param field Reference to the Field2D object representing the environment
         * @param num_particles Number of particles to use in the MCL algorithm
         * @param rotationNoise Amount of rotation noise to add during prediction step
         * @param translationNoise Amount of translation noise to add during prediction step
         * @param debug Whether to enable debug display on the screen
         * @param intensitivity How often to update MCL (lower is more frequent)
         * @param shouldTaskRun Whether to run the MCL update in a separate task
         * @return MCL object
         *
         * @example
         * limelib::MCL mcl(&verticalTW, &horizontalTW, imu, sensors, field, 100, 0.1, 0.1, true, 10, true);
         *
         * This creates an MCL localization object using the specified tracking wheels, IMU, distance sensors, and field.
         * It uses 100 particles with 0.1 rotation and translation noise, enables debug display, updates every 10 cycles, and runs in a separate task.
         */
        MCL(TrackingWheel *verticalTW, TrackingWheel *horizontalTW, pros::Imu &imu, std::vector<MCLDistance> &sensors, Field2D &field, int num_particles, int rotationNoise, int translationNoise, bool debug = false, int intensitivity = 10, bool shouldTaskRun = true);
        void calibrate() override;
        Pose2D update() override;
        Pose2D getPose(bool radians = false) const override;
        void setPose(Pose2D pose) override;
        void setPose(real_t x, real_t y, real_t theta) override;

    private:
        Odometry odomHelper;
        std::vector<MCLDistance> &sensors;
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
        bool shouldTaskRun;
        bool debug;
        std::vector<MCLParticle> particles;
        void updateMCL();
        void debugDisplay();
    };

}

#endif