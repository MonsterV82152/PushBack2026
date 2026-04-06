#ifndef GLOBALS_HPP
#define GLOBALS_HPP

#include "main.h"

// Constants
constexpr double PI = 3.141592;

/*---Controller---*/
inline pros::Controller master(pros::E_CONTROLLER_MASTER);
inline pros::Controller slave(pros::E_CONTROLLER_PARTNER);

/*------------------Global Variables------------------*/

namespace buttons
{
    constexpr auto R1 = pros::E_CONTROLLER_DIGITAL_R1;
    constexpr auto R2 = pros::E_CONTROLLER_DIGITAL_R2;
    constexpr auto L1 = pros::E_CONTROLLER_DIGITAL_L1;
    constexpr auto L2 = pros::E_CONTROLLER_DIGITAL_L2;
    constexpr auto A = pros::E_CONTROLLER_DIGITAL_A;
    constexpr auto B = pros::E_CONTROLLER_DIGITAL_B;
    constexpr auto Y = pros::E_CONTROLLER_DIGITAL_Y;
    constexpr auto X = pros::E_CONTROLLER_DIGITAL_X;
    constexpr auto UP = pros::E_CONTROLLER_DIGITAL_UP;
    constexpr auto DOWN = pros::E_CONTROLLER_DIGITAL_DOWN;
    constexpr auto LEFT = pros::E_CONTROLLER_DIGITAL_LEFT;
    constexpr auto RIGHT = pros::E_CONTROLLER_DIGITAL_RIGHT;
}

const pros::motor_brake_mode_e_t brake = pros::E_MOTOR_BRAKE_BRAKE;
const pros::motor_brake_mode_e_t coast = pros::E_MOTOR_BRAKE_COAST;
const pros::motor_brake_mode_e_t hold = pros::E_MOTOR_BRAKE_HOLD;

#define A_NEW_PRESS (master.get_digital_new_press(buttons::A))
#define B_NEW_PRESS (master.get_digital_new_press(buttons::B))
#define Y_NEW_PRESS (master.get_digital_new_press(buttons::Y))
#define X_NEW_PRESS (master.get_digital_new_press(buttons::X))
#define UP_NEW_PRESS (master.get_digital_new_press(buttons::UP))
#define DOWN_NEW_PRESS (master.get_digital_new_press(buttons::DOWN))
#define LEFT_NEW_PRESS (master.get_digital_new_press(buttons::LEFT))
#define RIGHT_NEW_PRESS (master.get_digital_new_press(buttons::RIGHT))
#define R1_NEW_PRESS (master.get_digital_new_press(buttons::R1))
#define R2_NEW_PRESS (master.get_digital_new_press(buttons::R2))
#define L1_NEW_PRESS (master.get_digital_new_press(buttons::L1))
#define L2_NEW_PRESS (master.get_digital_new_press(buttons::L2))

#define A_HELD (master.get_digital(buttons::A))
#define B_HELD (master.get_digital(buttons::B))
#define Y_HELD (master.get_digital(buttons::Y))
#define X_HELD (master.get_digital(buttons::X))
#define UP_HELD (master.get_digital(buttons::UP))
#define DOWN_HELD (master.get_digital(buttons::DOWN))
#define LEFT_HELD (master.get_digital(buttons::LEFT))
#define RIGHT_HELD (master.get_digital(buttons::RIGHT))
#define R1_HELD (master.get_digital(buttons::R1))
#define R2_HELD (master.get_digital(buttons::R2))
#define L1_HELD (master.get_digital(buttons::L1))
#define L2_HELD (master.get_digital(buttons::L2))

#define LEFT_Y master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y)
#define LEFT_X master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_X)
#define RIGHT_Y master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y)
#define RIGHT_X master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X)

#define A_RELEASED (master.get_digital_new_release(buttons::A))
#define B_RELEASED (master.get_digital_new_release(buttons::B))
#define Y_RELEASED (master.get_digital_new_release(buttons::Y))
#define X_RELEASED (master.get_digital_new_release(buttons::X))
#define UP_RELEASED (master.get_digital_new_release(buttons::UP))
#define DOWN_RELEASED (master.get_digital_new_release(buttons::DOWN))
#define LEFT_RELEASED (master.get_digital_new_release(buttons::LEFT))
#define RIGHT_RELEASED (master.get_digital_new_release(buttons::RIGHT))
#define R1_RELEASED (master.get_digital_new_release(buttons::R1))
#define R2_RELEASED (master.get_digital_new_release(buttons::R2))
#define L1_RELEASED (master.get_digital_new_release(buttons::L1))
#define L2_RELEASED (master.get_digital_new_release(buttons::L2))

#endif