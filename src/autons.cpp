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

int num = 0;
void antiJam(){
    if(intakeMotor.get_actual_velocity() == 0 && num == 1){
            intakeMotor.move_velocity(-600);
            pros::delay(250);
            intakeMotor.move_velocity(600);
    }
    pros::delay(20);
    }
  

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
    // Small delay to prevent overwhelming the CPU with constant checks
    pros::delay(20);
    }
  }
  
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
  chassis.pid_drive_set(-42_in, 60);
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
  wallStake.move_absolute(400,100);
}

void soloSigAWPBlueRight(){
  ringRush.set_value(1);
  chassis.pid_drive_set(44_in, 90);
  chassis.pid_wait();
  pros::delay(100);
  chassis.pid_drive_set(-20_in, 127);
  chassis.pid_wait();
  ringRush.set_value(0);
  pros::delay(200);
  chassis.pid_drive_set(-14_in, 127);
  chassis.pid_wait();
  chassis.pid_turn_set(133_deg, TURN_SPEED);
  chassis.pid_wait();
  chassis.pid_drive_set(-27_in, 80);
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
  
  chassis.pid_drive_set(5.5_in, 100);
  chassis.pid_wait();
  wallStake.move_velocity(500);
  pros::delay(750);
  wallStake.move_velocity(0);
  chassis.pid_drive_set(-23_in, 75);
  chassis.pid_wait();
  chassis.pid_drive_set(-7_in, 75);
  chassis.pid_wait();
  wallStake.move_velocity(-600);
  pros::delay(800);
  wallStake.move_velocity(0);
  pros::delay(200);
  Clamper.set_value(1);
  pros::delay(200);
  chassis.pid_wait();
  chassis.pid_turn_set(121_deg, 90);
  chassis.pid_wait();  
  intakeMotor.move_velocity(600);
  chassis.pid_drive_set(33_in, 127);
  pros::delay(1500);
  chassis.pid_wait();
  intakeMotor.move_velocity(-600);
  pros::delay(200);
  intakeMotor.move_velocity(600);
  chassis.pid_drive_set(-9_in, 127);
  chassis.pid_wait();
  chassis.pid_turn_set(55_deg, TURN_SPEED);
  chassis.pid_wait();
  chassis.pid_drive_set(38_in, 90);
  chassis.pid_wait();
  pros::delay(1050);
  chassis.pid_turn_set(-100_deg, TURN_SPEED);
  chassis.pid_wait();
  intakeMotor.move_velocity(-600);
  pros::delay(200);
  intakeMotor.move_velocity(600);
  // intakeMotor.move_velocity(600);
  chassis.pid_drive_set(25_in, DRIVE_SPEED);
  // chassis.pid_wait();
  // chassis.pid_turn_set(-120_deg, TURN_SPEED);
  // chassis.pid_wait();
  // pros::delay(200);
  // wallStake.move_absolute(500, -600);
  // chassis.pid_drive_set(35_in, DRIVE_SPEED);
  // chassis.pid_wait();
  // wallStake.move_absolute(1250, 600);
}

void red_Right_Side() {
  chassis.pid_drive_set(5.5_in, 100);
  chassis.pid_wait();
  wallStake.move_velocity(500);
  pros::delay(750);
  wallStake.move_velocity(0);
  chassis.pid_drive_set(-23_in, 75);
  chassis.pid_wait();
  chassis.pid_drive_set(-7_in, 75);
  chassis.pid_wait();
  wallStake.move_velocity(-600);
  pros::delay(800);
  wallStake.move_velocity(0);
  pros::delay(200);
  Clamper.set_value(1);
  pros::delay(200);
  chassis.pid_wait();
  chassis.pid_turn_set(-121_deg, 90);
  chassis.pid_wait();  
  intakeMotor.move_velocity(600);
  chassis.pid_drive_set(33_in, 127);
  pros::delay(1500);
  chassis.pid_wait();
  intakeMotor.move_velocity(-600);
  pros::delay(200);
  intakeMotor.move_velocity(600);
  chassis.pid_drive_set(-9_in, 127);
  chassis.pid_wait();
  chassis.pid_turn_set(-55_deg, TURN_SPEED);
  chassis.pid_wait();
  chassis.pid_drive_set(38_in, 90);
  chassis.pid_wait();
  pros::delay(1050);
  chassis.pid_turn_set(100_deg, TURN_SPEED);
  chassis.pid_wait();
  intakeMotor.move_velocity(-600);
  pros::delay(200);
  intakeMotor.move_velocity(600);
  // intakeMotor.move_velocity(600);
  chassis.pid_drive_set(25_in, DRIVE_SPEED);
  // chassis.pid_wait();
  // chassis.pid_turn_set(-120_deg, TURN_SPEED);
  // chassis.pid_wait();
  // pros::delay(200);
  // wallStake.move_absolute(500, -600);
  // chassis.pid_drive_set(35_in, DRIVE_SPEED);
  // chassis.pid_wait();
  // wallStake.move_absolute(1250, 600);
  }

void skillsAuto(){
  wallStake.move_velocity(600);
  pros::delay(400);
  wallStake.move_velocity(-600);
  pros::delay(300);
  wallStake.move_velocity(0);
  chassis.pid_drive_set(-13_in, DRIVE_SPEED);
  chassis.pid_wait();
  chassis.pid_turn_set(90_deg, TURN_SPEED);
  chassis.pid_wait();
  chassis.pid_drive_set(-24_in, 80);
  chassis.pid_wait();
  pros::delay(100);
  Clamper.set_value(1);
  pros::delay(100);
  chassis.pid_turn_set(180_deg, 127);
  chassis.pid_wait();
  //intakeMotor.move_velocity(600);
  chassis.pid_drive_set(22.5_in, DRIVE_SPEED);
  chassis.pid_wait();
  num = 1;
  pros::delay(300);
  chassis.pid_turn_set(205_deg, 127);
  chassis.pid_wait();
  chassis.pid_drive_set(58_in, 120);
  chassis.pid_wait();
  pros::delay(300);
  chassis.pid_turn_set(17_deg, 127);
  chassis.pid_wait();
  chassis.pid_drive_set(28_in, 120);
  chassis.pid_wait();
  chassis.pid_turn_set(-90_deg, 127);
  chassis.pid_wait();
  num = 0;
  wallStake.move_absolute(320,600);
  chassis.pid_drive_set(17_in, 120);
  chassis.pid_wait();
  pros::delay(700);
  intakeMotor.move_velocity(0);
  wallStake.move_absolute(1800,600);
  pros::delay(500);
  wallStake.move_absolute(10,-600);
  chassis.pid_drive_set(-11_in, 120);
  chassis.pid_wait();
  chassis.pid_turn_set(2_deg, 127);
  chassis.pid_wait();
  //intakeMotor.move_velocity(600);
  num = 1;
  chassis.pid_drive_set(32_in, 127);
  chassis.pid_wait();
  pros::delay(400);
  chassis.pid_drive_set(16_in, 127);
  chassis.pid_wait();
  pros::delay(400);
  chassis.pid_drive_set(12_in, 127);
  chassis.pid_wait();
  pros::delay(400);
  chassis.pid_turn_set(-125_deg, 127);
  chassis.pid_wait();
  chassis.pid_drive_set(18_in, 120);
  chassis.pid_wait();
  pros::delay(600);
  chassis.pid_turn_set(-220_deg, 127);
  chassis.pid_wait();
  chassis.pid_drive_set(-13_in, 120);
  chassis.pid_wait();
  pros::delay(100);
  Clamper.set_value(0);
  pros::delay(100);
  chassis.pid_drive_set(14_in, 120);
  chassis.pid_wait();
  chassis.pid_turn_set(-240_deg, 127);
  chassis.pid_wait();
  chassis.pid_drive_set(-73_in, 70);
  chassis.pid_wait();
  pros::delay(100);
  Clamper.set_value(1);
  pros::delay(100);
  chassis.pid_turn_set(178_deg, 127);
  chassis.pid_wait();
  intakeMotor.move_velocity(0);
}