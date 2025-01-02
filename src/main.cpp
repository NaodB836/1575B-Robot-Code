#include "main.h"
// Chassis constructor
ez::Drive chassis(
    // These are your drive motors, the first motor is used for sensing!
    {10, -20, -7},     // Left Chassis Ports (negative port will reverse it!)
    {-8, 4, 5},  // Right Chassis Ports (negative port will reverse it!)

    2,      // IMU Port
    2.75,  // Wheel Diameter (Remember, 4" wheels without screw holes are actually 4.125!)
    450);   // Wheel RPM
/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */



void initialize() {
  pros::delay(500);  // Stop the user from doing anything while legacy ports configure

  // Configure your chassis controls
  chassis.opcontrol_curve_buttons_toggle(true);  // Enables modifying the controller curve with buttons on the joysticks
  chassis.opcontrol_drive_activebrake_set(2);    // Sets the active brake kP. We recommend ~2.  0 will disable.
  chassis.opcontrol_curve_default_set(0, 0);     // Defaults for curve. If using tank, only the first parameter is used. (Comment this line out if you have an SD card!)

  // Set the drive to your own constants from autons.cpp!
  default_constants();

  // Autonomous Selector using LLEMU
  ez::as::auton_selector.autons_add({
      Auton("Skills Auto", skillsAuto),
      Auton(" Right Side Auton AWP", red_Right_Side),
      Auton(" Left Side Auton AWP", blue_Left_Side),
     /* Auton("Drive and Turn\n\nSlow down during drive.", wait_until_change_speed),
      Auton("Swing Example\n\nSwing in an 'S' curve", swing_example),
      Auton("Motion Chaining\n\nDrive forward, turn, and come back, but blend everything together :D", motion_chaining),
      Auton("Combine all 3 movements", combining_movements),
      Auton("Interference\n\nAfter driving forward, robot performs differently if interfered or not.", interfered_example),
  */
    });
  
  // Initialize chassis and auton selector
  chassis.initialize();
  ez::as::initialize(); 
  master.rumble(".");

}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {
  // . . .
}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {
  // . . .
}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */


void autonomous() {
  chassis.pid_targets_reset();                // Resets PID targets to 0
  chassis.drive_imu_reset();                  // Reset gyro position to 0
  chassis.drive_sensor_reset();               // Reset drive sensors to 0
  chassis.drive_brake_set(MOTOR_BRAKE_HOLD);
  //Set motors to hold.  This helps autonomous consistency
  // pros::Task colorTask(BlueColorSensor_task);
  ez::as::auton_selector.selected_auton_call();  // Calls selected auton from autonomous selector
}


/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */

bool clampToggleEnabled = false; // two-choice toggle, so we use bool for clamp
bool buttonPressed = false; // IGNORE, logic variable

bool wallStakeToggleEnabled = false; // two-choice toggle, so we use bool for wall stake pistons
bool buttonPressed2 = false; // IGNORE, logic variable

bool doinkerToggleEnabled = false; // two-choice toggle, so we use bool for rachet
bool buttonPressed3 = false; // IGNORE, logic variable

bool intakeToggleEnabled = false; // two-choice toggle, so we use bool for rachet
bool buttonPressed4 = false; // IGNORE, logic variable

void opcontrol() {
  intake.set_value(1);
  pros::motor_brake_mode_e_t driver_preference_brake = MOTOR_BRAKE_COAST;
  chassis.drive_brake_set(driver_preference_brake);

  while (true) {
    bool buttonY = master.get_digital(DIGITAL_Y);
    //Toggle Logic
    if (buttonY && !buttonPressed){
      buttonPressed = true; 
      clampToggleEnabled = !clampToggleEnabled;
    }
    else if (!buttonY) buttonPressed = false;

    bool buttonL1 = master.get_digital(DIGITAL_L1);
    //Toggle Logic
    if (buttonL1 && !buttonPressed2){
      buttonPressed2 = true; 
      wallStakeToggleEnabled = !wallStakeToggleEnabled;
    }
    else if (!buttonL1) buttonPressed2 = false;

    bool buttonRight = master.get_digital(DIGITAL_RIGHT);
    //Toggle Logic
    if (buttonRight && !buttonPressed3){
      buttonPressed3 = true; 
      doinkerToggleEnabled = !doinkerToggleEnabled;
    }
    else if (!buttonRight) buttonPressed3 = false;

    bool buttonX = master.get_digital(DIGITAL_Y);
    //Toggle Logic
    if (buttonX && !buttonPressed4){
      buttonPressed4 = true; 
      intakeToggleEnabled = !intakeToggleEnabled;
    }
    else if (!buttonX) buttonPressed4 = false;
    // PID Tuner
    // After you find values that you're happy with, you'll have to set them in auton.cpp
    /*if (!pros::competition::is_connected()) {
      // Enable / Disable PID Tuner
      //  When enabled:
      //  * use A and Y to increment / decrement the constants
      //  * use the arrow keys to navigate the constants
      if (master.get_digital_new_press(DIGITAL_X))
        chassis.pid_tuner_toggle();

      // Trigger the selected autonomous routine
      if (master.get_digital(DIGITAL_B) && master.get_digital(DIGITAL_DOWN)) {
        autonomous();
        chassis.drive_brake_set(driver_preference_brake);
      }

      chassis.pid_tuner_iterate();  // Allow PID Tuner to iterate
    }*/
    
    if (master.get_digital(DIGITAL_B) && master.get_digital(DIGITAL_DOWN)) {
        autonomous();
        chassis.drive_brake_set(driver_preference_brake);
      }
    
    wallStake.set_brake_mode(MOTOR_BRAKE_COAST);
    if (master.get_digital_new_press(DIGITAL_L1)) { // Check if button R1 is pressed
      if(wallStakeToggleEnabled){
        // Do another thing
        wallStake.move_absolute(1900,3000);
        wallStake.set_brake_mode(MOTOR_BRAKE_COAST);
      }
      else{
        
        wallStake.move_absolute(347,2000);
        wallStake.set_brake_mode(MOTOR_BRAKE_COAST);
      }

    } 
    else if (master.get_digital_new_press(DIGITAL_L2)) { // Check if button R2 is pressed
        wallStake.move_absolute(100,-1000);
        wallStake.set_brake_mode(MOTOR_BRAKE_COAST);

    } 
    if (master.get_digital(DIGITAL_R1)) { // Check if button R1 is pressed
            intakeMotor.move(127);            
        }    
        else if (master.get_digital(DIGITAL_R2)) { // Check if button R2 is pressed

          intakeMotor.move(-127); // Set intake motor to full speed backward

        } 
        else{
          intakeMotor.move(0);      
        }
    


    if(clampToggleEnabled){
      // Do another thing
      Clamper.set_value(1);
    }
    else{
      // Do initial thing
      Clamper.set_value(0);
    }

    if(doinkerToggleEnabled){
      // Do another thing
      doinker.set_value(1);
    }
    else{
      // Do initial thing
      doinker.set_value(0);
    }

    /*if(intakeToggleEnabled){
      // Do another thing
      intake.set_value(1);
    }
    else{
      // Do initial thing
      intake.set_value(0);
    
    }*/


    //chassis.opcontrol_tank();  // Tank control
     chassis.opcontrol_arcade_standard(ez::SPLIT);   // Standard split arcade
    //chassis.opcontrol_arcade_standard(ez::SINGLE);  // Standard single arcade
     //chassis.opcontrol_arcade_flipped(ez::SPLIT);    // Flipped split arcade
    // chassis.opcontrol_arcade_flipped(ez::SINGLE);   // Flipped single arcade

    // . . .
    // Put more user control code here!
    // . . .

    pros::delay(ez::util::DELAY_TIME);  // This is used for timer calculations!  Keep this ez::util::DELAY_TIME
  }
}
