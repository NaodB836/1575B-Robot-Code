#pragma once

#include "api.h"

// Your motors, sensors, etc. should go here.  Below are examples

inline pros::Motor intakeMotor(15);
inline pros::Motor wallStakeMotor(21);
inline pros::adi::DigitalOut Clamper('A');
inline pros::adi::DigitalOut Color_sorter('B');
inline pros::adi::DigitalOut wallStakePiston('C');
inline pros::adi::DigitalOut wallStakePiston('D');
inline pros::Optical OP(20);

// inline pros::Motor intake(1);
// inline pros::adi::DigitalIn limit_switch('A');