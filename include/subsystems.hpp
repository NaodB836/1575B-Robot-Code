#pragma once

#include "api.h"

// Your motors, sensors, etc. should go here.  Below are examples

inline pros::Motor intakeMotor(9);
inline pros::MotorGroup wallStake({-3,6});
inline pros::adi::DigitalOut Clamper('A');
inline pros::adi::DigitalOut intake('B');
inline pros::adi::DigitalOut doinker('C'); 
inline pros::adi::DigitalOut ringRush('H');                                                                                                          
inline pros::Optical OP(11);

/*extern void Anti_Jam();
extern void BlueColorSensor_task();
inline pros::Task antiJammer(Anti_Jam);
inline pros::Task colorTask(BlueColorSensor_task);*/
// inline pros::Motor intake(1);
// inline pros::adi::DigitalIn limit_switch('A');