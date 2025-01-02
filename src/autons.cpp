#include "main.h"

/////

/////

// These are out of 127
const int DRIVE_SPEED = 120;
const int TURN_SPEED = 120;
const int SWING_SPEED = 120;

///
// Constants
///
void default_constants() {
  chassis.pid_heading_constants_set(11, 0, 20);
  chassis.pid_drive_constants_set(10, 0.03, 6);
  chassis.pid_turn_constants_set(3, 0.05, 20, 15);
  
  chassis.pid_swing_constants_set(6, 0, 65);

  chassis.pid_turn_exit_condition_set(80_ms, 3_deg, 250_ms, 7_deg, 500_ms, 500_ms);
  chassis.pid_swing_exit_condition_set(80_ms, 3_deg, 250_ms, 7_deg, 500_ms, 500_ms);
  chassis.pid_drive_exit_condition_set(80_ms, 1_in, 250_ms, 3_in, 500_ms, 500_ms);

  chassis.pid_turn_chain_constant_set(3_deg);
  chassis.pid_swing_chain_constant_set(5_deg);
  chassis.pid_drive_chain_constant_set(3_in);

  chassis.slew_drive_constants_set(7_in, 80);
}

/*void antiJam(){
    if(intakeMotor.get_actual_velocity() == 0){
            intakeMotor.move(-127);
            pros::delay(500);
    }
    pros::delay(20);
    }*/
  

int Test = 0;
void Anti_Jam(){
 while (true) {
        // Check if the R2 button is pressed
        if (Test == 1) {
            // Move the intake
            intakeMotor.move(127);
            pros::delay(50);
            // Check if the voltage of the intake equals 0
            if (intakeMotor.get_actual_velocity() == 0) {
                // Reverse the intake for 200 ms
                intakeMotor.move(-127);
                pros::delay(220);
                // Resume normal intake operation
                intakeMotor.move(127);
            }
        }
        else if(Test ==2){
          intakeMotor.move(-127);
        }
        else if(Test == 0) {
            // Stop the intake if R2 is not pressed
            intakeMotor.move(0);
        }
        pros::delay(20); // Delay to prevent excessive CPU usage
  }
}
inline pros::Task antiJammer(Anti_Jam);
int IntakeToggleV = -1;         // Tracks the forward toggle state (1 for on, -1 for off)
int IntakeReverseToggleV = -1;   // Tracks the reverse toggle state (1 for reverse on, -1 for off)
void IntakeToggle() {
// If `IntakeToggleV` is 1, the intake motors will run at full speed
// If `IntakeToggleV` is -1, the intake motors will stop
  // Toggle the `IntakeToggleV` state each time this function is called
  IntakeToggleV = -1 * IntakeToggleV;
  // If IntakeToggleV is 1, move both intake motors forward at full speed
  if (IntakeToggleV == 1) {
    intakeMotor.move(127);      // Move the first intake motor at full speed
    Test = 1;
  }
  // This condition attempts to check if both toggles (forward and reverse) are active,
  // but `IntakeToggleV == 1` is redundant here since it was already checked in the previous `if` condition.
  // else if (IntakeReverseToggleV == 1 && IntakeToggleV == 1) {
  //   intakeMotor.move(127);      // Move the first intake motor at full speed
  // }
  // If IntakeToggleV is -1, stop both intake motors
  else {
    intakeMotor.move(0);        // Stop the first intake motor
    Test = 0;
  }
}
void BlueColorSensor_task(){
  while (true) { // Infinite loop to continuously check the color sensor
      OP.set_led_pwm(100);
    // Get the detected color hue from the color sensor
    int hue = OP.get_hue();

    // Check if the detected color is red (typical red hue is around 8 degrees)
    if (hue >= 5 && hue <= 15){
      
      intakeMotor.move(0);// Activates the color sorter piston for red
      pros::delay(1000);
      intakeMotor.move(127);
    }  
    else{
        if (master.get_digital(DIGITAL_R1)) { // Check if button R1 is pressed
            intakeMotor.move(127);            
        }    
        else if (master.get_digital(DIGITAL_R2)) { // Check if button R2 is pressed

          intakeMotor.move(-127); // Set intake motor to full speed backward

        } 
        else{
          intakeMotor.move(0);      
        }
    } 

    // Small delay to prevent overwhelming the CPU with constant checks
    pros::delay(20);
    }
  }

///
// Drive Example
///
void drive_example() {
  // The first parameter is target inches
  // The second parameter is max speed the robot will drive at
  // The third parameter is a boolean (true or false) for enabling/disabling a slew at the start of drive motions
  // for slew, only enable it when the drive distance is greater than the slew distance + a few inches

  chassis.pid_drive_set(24_in, 55);
  chassis.pid_wait();
}

///I understand it now
// Turn Example
///
void turn_example() {
  // The first parameter is the target in degrees
  // The second parameter is max speed the robot will drive at

  chassis.pid_turn_set(90_deg, TURN_SPEED);
  chassis.pid_wait();

  chassis.pid_turn_set(45_deg, TURN_SPEED);
  chassis.pid_wait();

  chassis.pid_turn_set(0_deg, TURN_SPEED);
  chassis.pid_wait();
}

///
// Combining Turn + Drive
///
void drive_and_turn() {
  chassis.pid_drive_set(24_in, DRIVE_SPEED, true);
  chassis.pid_wait();

  chassis.pid_turn_set(45_deg, TURN_SPEED);
  chassis.pid_wait();

  chassis.pid_turn_set(-45_deg, TURN_SPEED);
  chassis.pid_wait();

  chassis.pid_turn_set(0_deg, TURN_SPEED);
  chassis.pid_wait();

  chassis.pid_drive_set(-24_in, DRIVE_SPEED, true);
  chassis.pid_wait();
}

///
// Wait Until and Changing Max Speed
///
void wait_until_change_speed() {
  // pid_wait_until will wait until the robot gets to a desired position

  // When the robot gets to 6 inches slowly, the robot will travel the remaining distance at full speed
  chassis.pid_drive_set(24_in, 30, true);
  chassis.pid_wait_until(6_in);
  chassis.pid_speed_max_set(DRIVE_SPEED);  // After driving 6 inches at 30 speed, the robot will go the remaining distance at DRIVE_SPEED
  chassis.pid_wait();

  chassis.pid_turn_set(45_deg, TURN_SPEED);
  chassis.pid_wait();

  chassis.pid_turn_set(-45_deg, TURN_SPEED);
  chassis.pid_wait();

  chassis.pid_turn_set(0_deg, TURN_SPEED);
  chassis.pid_wait();

  // When the robot gets to -6 inches slowly, the robot will travel the remaining distance at full speed
  chassis.pid_drive_set(-24_in, 30, true);
  chassis.pid_wait_until(-6_in);
  chassis.pid_speed_max_set(DRIVE_SPEED);  // After driving 6 inches at 30 speed, the robot will go the remaining distance at DRIVE_SPEED
  chassis.pid_wait();
}

///
// Swing Example
///
void swing_example() {
  // The first parameter is ez::LEFT_SWING or ez::RIGHT_SWING
  // The second parameter is the target in degrees
  // The third parameter is the speed of the moving side of the drive
  // The fourth parameter is the speed of the still side of the drive, this allows for wider arcs

  chassis.pid_swing_set(ez::LEFT_SWING, 45_deg, SWING_SPEED, 45);
  chassis.pid_wait();

  chassis.pid_swing_set(ez::RIGHT_SWING, 0_deg, SWING_SPEED, 45);
  chassis.pid_wait();

  chassis.pid_swing_set(ez::RIGHT_SWING, 45_deg, SWING_SPEED, 45);
  chassis.pid_wait();

  chassis.pid_swing_set(ez::LEFT_SWING, 0_deg, SWING_SPEED, 45);
  chassis.pid_wait();
}

///
// Motion Chaining
///
void motion_chaining() {
  // Motion chaining is where motions all try to blend together instead of individual movements.
  // This works by exiting while the robot is still moving a little bit.
  // To use this, replace pid_wait with pid_wait_quick_chain.
  chassis.pid_drive_set(24_in, DRIVE_SPEED, true);
  chassis.pid_wait();

  chassis.pid_turn_set(45_deg, TURN_SPEED);
  chassis.pid_wait_quick_chain();

  chassis.pid_turn_set(-45_deg, TURN_SPEED);
  chassis.pid_wait_quick_chain();

  chassis.pid_turn_set(0_deg, TURN_SPEED);
  chassis.pid_wait();

  // Your final motion should still be a normal pid_wait
  chassis.pid_drive_set(-24_in, DRIVE_SPEED, true);
  chassis.pid_wait();
}

///
// Auto that tests everything
///
void combining_movements() {
  chassis.pid_drive_set(24_in, DRIVE_SPEED, true);
  chassis.pid_wait();

  chassis.pid_turn_set(45_deg, TURN_SPEED);
  chassis.pid_wait();

  chassis.pid_swing_set(ez::RIGHT_SWING, -45_deg, SWING_SPEED, 45);
  chassis.pid_wait();

  chassis.pid_turn_set(0_deg, TURN_SPEED);
  chassis.pid_wait();

  chassis.pid_drive_set(-24_in, DRIVE_SPEED, true);
  chassis.pid_wait();
}

///
// Interference example
///
void tug(int attempts) {
  for (int i = 0; i < attempts - 1; i++) {
    // Attempt to drive backward
    printf("i - %i", i);
    chassis.pid_drive_set(-12_in, 127);
    chassis.pid_wait();

    // If failsafed...
    if (chassis.interfered) {
      chassis.drive_sensor_reset();
      chassis.pid_drive_set(-2_in, 20);
      pros::delay(1000);
    }
    // If the robot successfully drove back, return
    else {
      return;
    }
  }
}

// If there is no interference, the robot will drive forward and turn 90 degrees.
// If interfered, the robot will drive forward and then attempt to drive backward.
void interfered_example() {
  chassis.pid_drive_set(24_in, DRIVE_SPEED, true);
  chassis.pid_wait();

  if (chassis.interfered) {
    tug(3);
    return;
  }

  chassis.pid_turn_set(90_deg, TURN_SPEED);
  chassis.pid_wait();
}

// . . .
// Make your own autonomous functions here!
// . . .
void elimsAutoRedLeft() {

  wallStake.move_absolute(1000,3000);
  chassis.pid_wait();
  chassis.pid_drive_set(-41_in, 60);
  chassis.pid_wait();
  pros::delay(500);
  Clamper.set_value(1);
  pros::delay(500);
  chassis.pid_turn_set(115_deg, 75);
  chassis.pid_wait();
  intakeMotor.move_velocity(6000);
  chassis.pid_drive_set(22_in, 75);
  chassis.pid_wait();
  intakeMotor.move_velocity(-6000);
  chassis.pid_turn_set(70_deg, TURN_SPEED);
  chassis.pid_wait();
  intakeMotor.move_velocity(6000);
  chassis.pid_drive_set(5.5_in, DRIVE_SPEED);
  pros::delay(3400);
  intakeMotor.move_velocity(0);
  chassis.pid_drive_set(-15_in, DRIVE_SPEED);
  chassis.pid_wait();
  chassis.pid_turn_set(30_deg, 75);
  chassis.pid_wait();
  intakeMotor.move_velocity(6000);
  chassis.pid_drive_set(23_in, 75);
  chassis.pid_wait();
  chassis.pid_turn_set(105_deg, 75);
  chassis.pid_wait();
  chassis.pid_drive_set(22_in, 75);
  chassis.pid_wait();
  intakeMotor.move_velocity(0);
  wallStake.move_absolute(1700, 3000);
  chassis.pid_wait();
  pros::delay(2000);
  /*chassis.pid_drive_set(-25_in, 75);
  wallStake.move_absolute(20,3000);
  chassis.pid_wait();*/
  
}

void redAutoRightElims(){

  chassis.pid_drive_set(12_in, DRIVE_SPEED);
  chassis.pid_wait();
  wallStake.move_velocity(2300);
  pros::delay(500);
  wallStake.move_velocity(-2300);
  pros::delay(500);
  wallStake.move_velocity(0);
  chassis.pid_drive_set(-45_in, 60);
  chassis.pid_wait();
  pros::delay(500);
  Clamper.set_value(1);
  pros::delay(500);
  chassis.pid_wait();
  chassis.pid_turn_set(-115_deg, 90);
  chassis.pid_wait();
  chassis.pid_drive_set(23_in, 100);
  intakeMotor.move_velocity(3000);
  pros::delay(2100);
  intakeMotor.move_velocity(0);
  intakeMotor.move_velocity(-3000);
  pros::delay(200);
  intakeMotor.move_velocity(0);
  chassis.pid_drive_set(-25_in, DRIVE_SPEED);
  chassis.pid_wait();
  Clamper.set_value(0);
  chassis.pid_drive_set(15_in, DRIVE_SPEED);
  chassis.pid_wait();
  chassis.pid_turn_set(-10_deg, TURN_SPEED);
  chassis.pid_wait();
  /*
  intake.set_value(1);
  chassis.pid_drive_set(48_in, DRIVE_SPEED);
  chassis.pid_wait();
  intake.set_value(0);
  intakeMotor.move_velocity(3000);
  pros::delay(900);
  chassis.pid_drive_set(-25_in, DRIVE_SPEED);
  pros::delay(50);
  intakeMotor.move_velocity(0);
  intakeMotor.move_velocity(-3000);
  pros::delay(300);
  intakeMotor.move_velocity(0);
  chassis.pid_turn_set(120_deg, TURN_SPEED);
  chassis.pid_wait();
  chassis.pid_drive_set(24.5_in, DRIVE_SPEED);
  chassis.pid_wait();
  wallStake.move_absolute(400,100);*/
}

void soloSigAWPBlueRight(){

  intake.set_value(1);
  ringRush.set_value(1);
  chassis.pid_drive_set(44_in, 90);
  chassis.pid_wait();
  pros::delay(100);
  chassis.pid_drive_set(-20_in, 127);
  chassis.pid_wait();
  ringRush.set_value(0);
  chassis.pid_drive_set(-14_in, 127);
  chassis.pid_wait();
  chassis.pid_turn_set(133_deg, TURN_SPEED);
  chassis.pid_wait();
  chassis.pid_drive_set(-27_in, 100);
  chassis.pid_wait();
  Clamper.set_value(1);
  pros::delay(500);
  chassis.pid_turn_set(65_deg, TURN_SPEED);
  chassis.pid_wait();
  IntakeToggle();
  chassis.pid_drive_set(15_in, 70);
  chassis.pid_wait();
  pros::delay(1000);
  IntakeToggle();
  intakeMotor.move(-127);
  pros::delay(500);
  IntakeToggle();
  chassis.pid_drive_set(15_in, 70);
  chassis.pid_wait();
  pros::delay(1000);
  chassis.pid_drive_set(-15_in, 120);
  chassis.pid_wait();
  chassis.pid_turn_set(125_deg, TURN_SPEED);
  chassis.pid_wait();
  chassis.pid_drive_set(47_in, 120);
  chassis.pid_wait();
  intake.set_value(0);
  chassis.pid_wait();
  intake.set_value(1);
  pros::delay(500);
  chassis.pid_drive_set(-5_in, 120);
  chassis.pid_wait();
  chassis.pid_drive_set(5_in, 120);
  chassis.pid_wait();
  pros::delay(500);
  chassis.pid_drive_set(-15_in, 120);
  chassis.pid_wait();
  chassis.pid_turn_set(285_deg, TURN_SPEED);
  chassis.pid_wait();
  /*chassis.pid_drive_set(14.5_in, DRIVE_SPEED);
  chassis.pid_wait();
  wallStake.move_velocity(2300);
  pros::delay(550);
  wallStake.move_velocity(-2300);
  pros::delay(550);
  chassis.pid_drive_set(-43_in, 70);
  wallStake.move_velocity(0);
  chassis.pid_wait();
  pros::delay(100);
  Clamper.set_value(1);
  pros::delay(100);
  chassis.pid_wait();
  chassis.pid_turn_set(-160_deg, TURN_SPEED);
  chassis.pid_wait();
  intakeMotor.move(127);
  chassis.pid_drive_set(19_in, DRIVE_SPEED);
  chassis.pid_wait();
  pros::delay(700);
  chassis.pid_drive_set(-9_in, DRIVE_SPEED);
  chassis.pid_wait();
  chassis.pid_turn_set(-65_deg, TURN_SPEED);
  chassis.pid_wait();
  chassis.pid_drive_set(15_in, 75);
  chassis.pid_wait();
  chassis.pid_turn_set(-45_deg, TURN_SPEED);
  chassis.pid_wait();
  intakeMotor.move(0);*/



}

void elimsAutoBlueRight() {

  wallStake.move_absolute(600,3000);
  chassis.pid_wait();
  chassis.pid_drive_set(-41_in, 60);
  chassis.pid_wait();
  pros::delay(500);
  Clamper.set_value(1);
  pros::delay(500);
  chassis.pid_turn_set(-115_deg, 75);
  chassis.pid_wait();
  intakeMotor.move_velocity(6000);
  chassis.pid_drive_set(22_in, 75);
  chassis.pid_wait();
  chassis.pid_turn_set(-70_deg, TURN_SPEED);
  chassis.pid_wait();
  chassis.pid_drive_set(5.5_in, DRIVE_SPEED);
  pros::delay(3400);
  intakeMotor.move_velocity(0);
  chassis.pid_drive_set(-15_in, DRIVE_SPEED);
  chassis.pid_wait();
  chassis.pid_turn_set(-30_deg, 75);
  chassis.pid_wait();
  intakeMotor.move_velocity(6000);
  chassis.pid_drive_set(23_in, 75);
  chassis.pid_wait();
  chassis.pid_turn_set(-107_deg, 75);
  chassis.pid_wait();
  chassis.pid_drive_set(21_in, 75);
  chassis.pid_wait();
  intakeMotor.move_velocity(0);
  wallStake.move_absolute(1625,3000);
  chassis.pid_wait();
  pros::delay(1500);
  chassis.pid_drive_set(-25_in, 75);
  pros::delay(2500);
  wallStake.move_absolute(20,3000);
  chassis.pid_wait();
  
}
void blue_Left_Side() {
  
  intake.set_value(1);
  chassis.pid_drive_set(12_in, DRIVE_SPEED);
  chassis.pid_wait();
  wallStake.move_velocity(2300);
  pros::delay(500);
  wallStake.move_velocity(-2300);
  pros::delay(500);
  wallStake.move_velocity(0);
  chassis.pid_drive_set(-45_in, 60);
  chassis.pid_wait();
  pros::delay(500);
  Clamper.set_value(1);
  pros::delay(500);
  chassis.pid_wait();
  chassis.pid_turn_set(115_deg, 90);
  chassis.pid_wait();
  chassis.pid_drive_set(23_in, 100);
  intakeMotor.move_velocity(3000);
  pros::delay(2100);
  intakeMotor.move_velocity(0);
  intakeMotor.move_velocity(-3000);
  pros::delay(200);
  intakeMotor.move_velocity(0);
  chassis.pid_wait();
  chassis.pid_turn_set(-35_deg, TURN_SPEED);
  chassis.pid_wait();
  intake.set_value(0);
  chassis.pid_drive_set(48_in, DRIVE_SPEED);
  chassis.pid_wait();
  intake.set_value(1);
  intakeMotor.move_velocity(3000);
  pros::delay(900);
  chassis.pid_drive_set(-25_in, DRIVE_SPEED);
  pros::delay(950);
  intakeMotor.move_velocity(0);
  intakeMotor.move_velocity(-3000);
  pros::delay(300);
  intakeMotor.move_velocity(0);
  chassis.pid_turn_set(-120_deg, TURN_SPEED);
  chassis.pid_wait();
  chassis.pid_drive_set(24.5_in, DRIVE_SPEED);
  chassis.pid_wait();
  wallStake.move_absolute(400,100);
}

void red_Right_Side() {

  chassis.pid_drive_set(12_in, DRIVE_SPEED);
  chassis.pid_wait();
  wallStake.move_velocity(2300);
  pros::delay(500);
  wallStake.move_velocity(-2300);
  pros::delay(500);
  wallStake.move_velocity(0);
  intake.set_value(1);
  chassis.pid_drive_set(-25_in, 127);
  chassis.pid_wait();
  pros::delay(100);
  Clamper.set_value(1);
  pros::delay(100);
  chassis.pid_wait();
  chassis.pid_turn_set(-115_deg, 90);
  chassis.pid_wait();
  intakeMotor.move_velocity(3000);
  chassis.pid_drive_set(23_in, 100);
  pros::delay(2100);
  intakeMotor.move_velocity(0);
  intakeMotor.move_velocity(-3000);
  pros::delay(200);
  intakeMotor.move_velocity(0);
  chassis.pid_wait();
  chassis.pid_turn_set(35_deg, TURN_SPEED);
  chassis.pid_wait();
  intake.set_value(0);  
  chassis.pid_drive_set(40_in, DRIVE_SPEED);
  chassis.pid_wait();
  intake.set_value(1);
  intakeMotor.move_velocity(0);
  pros::delay(900);
  chassis.pid_drive_set(-17_in, DRIVE_SPEED);
  pros::delay(50);
  intakeMotor.move_velocity(0);
  intakeMotor.move_velocity(-3000);
  pros::delay(300);
  intakeMotor.move_velocity(0);
  chassis.pid_turn_set(120_deg, TURN_SPEED);
  chassis.pid_wait();
  chassis.pid_drive_set(24.5_in, DRIVE_SPEED);
  chassis.pid_wait();
  wallStake.move_absolute(400,100);
}

void skillsAuto(){

  antiJammer.resume();
  wallStake.move_velocity(2300);
  pros::delay(600);
  wallStake.move_velocity(-2300);
  pros::delay(600);
  wallStake.move_velocity(0);
  chassis.pid_drive_set(-15_in, DRIVE_SPEED);
  chassis.pid_wait();
  intake.set_value(1);
  chassis.pid_turn_set(90_deg, TURN_SPEED);
  chassis.pid_wait();
  chassis.pid_drive_set(-25_in, 80);
  chassis.pid_wait();
  IntakeToggle();
  pros::delay(200);
  Clamper.set_value(1);
  pros::delay(200);
  chassis.pid_turn_set(267_deg, 127);
  chassis.pid_wait();
  chassis.pid_drive_set(25_in, 70);
  chassis.pid_wait();
  pros::delay(750);
  chassis.pid_turn_set(272_deg, 127);
  chassis.pid_wait();
  chassis.pid_drive_set(7_in, 70);
  chassis.pid_wait();
  pros::delay(750);
  chassis.pid_turn_set(150_deg, 127);
  chassis.pid_wait();
  chassis.pid_drive_set(-13_in, DRIVE_SPEED);
  chassis.pid_wait();
  pros::delay(500);
  intakeMotor.move(0);
  Clamper.set_value(0);
  chassis.pid_drive_set(10_in, DRIVE_SPEED);
  chassis.pid_wait();
  IntakeToggle();
  chassis.pid_turn_set(270, 127);
  chassis.pid_wait();
  chassis.pid_drive_set(-80_in, 100);
  chassis.pid_wait();
  pros::delay(100);
  Clamper.set_value(1);
  pros::delay(100);
  chassis.pid_wait();
  IntakeToggle();
  chassis.pid_turn_set(90_deg, 127);
  chassis.pid_wait();
  chassis.pid_drive_set(18_in, 65);
  chassis.pid_wait();
  pros::delay(750);
  chassis.pid_turn_set(88_deg, 127);
  chassis.pid_wait();
  chassis.pid_drive_set(8_in, 65);
  chassis.pid_wait();
  pros::delay(750);
  chassis.pid_turn_set(-150_deg, 127);
  chassis.pid_wait();
  chassis.pid_drive_set(-15_in, DRIVE_SPEED);
  chassis.pid_wait();
  pros::delay(500);
  IntakeToggle();
  Clamper.set_value(0);
  chassis.pid_drive_set(25_in, DRIVE_SPEED);
  chassis.pid_wait();
  chassis.pid_turn_set(-180_deg, 127);
  chassis.pid_wait();
  IntakeToggle();
  chassis.pid_drive_set(35_in, DRIVE_SPEED);
  chassis.pid_wait();
  IntakeToggle();
  chassis.pid_turn_set(27_deg, 127);
  chassis.pid_wait();
  chassis.pid_drive_set(-70_in, DRIVE_SPEED);
  chassis.pid_wait();
  pros::delay(100);
  Clamper.set_value(1);
  pros::delay(100);
  chassis.pid_turn_set(285_deg, 127);
  chassis.pid_wait();
  chassis.pid_drive_set(-35_in, DRIVE_SPEED);
  chassis.pid_wait();
  Clamper.set_value(0);
  chassis.pid_drive_set(30_in, DRIVE_SPEED);
  chassis.pid_wait();
  chassis.pid_turn_set(230_deg, 127);
  chassis.pid_wait();
  chassis.pid_drive_set(20_in, DRIVE_SPEED);
  chassis.pid_wait();
  chassis.pid_turn_set(270_deg, 127);
  chassis.pid_wait();
  


}