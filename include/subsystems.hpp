#pragma once

#include "api.h"

// Your motors, sensors, etc. should go here.  Below are examples

inline pros::Motor intakeMotor(15);
inline pros::MotorGroup wallStake({20,-6});
inline pros::adi::DigitalOut Clamper('A');
inline pros::adi::DigitalOut intake('B');
inline pros::adi::DigitalOut doinker('H');
inline pros::Optical OP(9);

// inline pros::Motor intake(1);
// inline pros::adi::DigitalIn limit_switch('A');