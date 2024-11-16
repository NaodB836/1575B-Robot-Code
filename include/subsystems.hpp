#pragma once

#include "api.h"

// Your motors, sensors, etc. should go here.  Below are examples

inline pros::Motor intakeMotor(15);
inline pros::Motor wallStakeMotor(6);
inline pros::adi::DigitalOut Clamper('A');
inline pros::adi::DigitalOut Color_sorter('B');
inline pros::adi::DigitalOut rachet('C');
inline pros::adi::DigitalOut wallStakePiston2('D');
inline pros::adi::DigitalOut wallStakePiston1('E');
inline pros::Optical OP(14);

// inline pros::Motor intake(1);
// inline pros::adi::DigitalIn limit_switch('A');