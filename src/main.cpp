#include "main.h"

// Chassis constructor
ez::Drive chassis(
    // These are your drive motors, the first motor is used for sensing!
    {11, -12, -13},     // Left Chassis Ports (negative port will reverse it!)
    {-1, 14, 2},  // Right Chassis Ports (negative port will reverse it!)

    10,      // IMU Port
    2.75,  // Wheel Diameter (Remember, 4" wheels without screw holes are actually 4.125!)
    480);   // Wheel RPM
  
void BlueColorSensor_task(){
  while (true) { // Infinite loop to continuously check the color sensor
    OP.set_led_pwm(100);
    // Get the detected color hue from the color sensor
    int hue = OP.get_hue();

    // Check if the detected color is red (typical red hue is around 8 degrees)
    if (hue >= 0 && hue <= 8){
      pros::delay(50); // Small delay before activating piston
      intakeMotor.move(0);// Activates the color sorter piston for red
      pros::delay(800); // Delay to keep sorter active for a longer duration
      intakeMotor.move_velocity(3000);// Activates the color sorter piston for red
      
    }
    else{ // If the color is neither red nor blue, deactivate the color sorter piston
      Color_sorter.set_value(0);
    }

    // Small delay to prevent overwhelming the CPU with constant checks
    pros::delay(20);
    }
}
/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
  pros::delay(100);  // Stop the user from doing anything while legacy ports configure

  // Configure your chassis controls
  chassis.opcontrol_curve_buttons_toggle(true);  // Enables modifying the controller curve with buttons on the joysticks
  chassis.opcontrol_drive_activebrake_set(0);    // Sets the active brake kP. We recommend ~2.  0 will disable.
  chassis.opcontrol_curve_default_set(0, 0);     // Defaults for curve. If using tank, only the first parameter is used. (Comment this line out if you have an SD card!)

  // Set the drive to your own constants from autons.cpp!
  default_constants();

  // Autonomous Selector using LLEMU
  ez::as::auton_selector.autons_add({
      Auton(" ", blue_Right_Side),
      /*Auton("Example Turn\n\nTurn 3 times.", turn_example),
      Auton("Drive and Turn\n\nDrive forward, turn, come back. ", drive_and_turn),
      Auton("Drive and Turn\n\nSlow down during drive.", wait_until_change_speed),
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

bool rachetToggleEnabled = false; // two-choice toggle, so we use bool for rachet
bool buttonPressed3 = false; // IGNORE, logic variable

void opcontrol() {
  // This is preference to what you like to drive on
  pros::motor_brake_mode_e_t driver_preference_brake = MOTOR_BRAKE_COAST;
  chassis.drive_brake_set(driver_preference_brake);

  pros::Task colorTask(BlueColorSensor_task);

  while (true) {
    bool buttonX = master.get_digital(DIGITAL_X);
    //Toggle Logic
    if (buttonX && !buttonPressed){
      buttonPressed = true; 
      clampToggleEnabled = !clampToggleEnabled;
    }
    else if (!buttonX) buttonPressed = false;

    bool buttonY = master.get_digital(DIGITAL_L1);
    //Toggle Logic
    if (buttonY && !buttonPressed2){
      buttonPressed2 = true; 
      wallStakeToggleEnabled = !wallStakeToggleEnabled;
    }
    else if (!buttonY) buttonPressed2 = false;

    bool buttonB = master.get_digital(DIGITAL_B);
    //Toggle Logic
    if (buttonB && !buttonPressed3){
      buttonPressed3 = true; 
      rachetToggleEnabled = !rachetToggleEnabled;
    }
    else if (!buttonB) buttonPressed3 = false;
    // PID Tuner
    // After you find values that you're happy with, you'll have to set them in auton.cpp
    if (!pros::competition::is_connected()) {
      // Enable / Disable PID Tuner
      //  When enabled:
      //  * use A and Y to increment / decrement the constants
      //  * use the arrow keys to navigate the constants
      /*if (master.get_digital_new_press(DIGITAL_X))
        chassis.pid_tuner_toggle();

      // Trigger the selected autonomous routine
      if (master.get_digital(DIGITAL_B) && master.get_digital(DIGITAL_DOWN)) {
        autonomous();
        chassis.drive_brake_set(driver_preference_brake);
      }

      chassis.pid_tuner_iterate();  // Allow PID Tuner to iterate
    }*/
    }
    

    if (master.get_digital(DIGITAL_R1)) { // Check if button R1 is pressed

      intakeMotor.move_velocity(3000); // Set intake motor to full speed forward

    } 
    else if (master.get_digital(DIGITAL_R2)) { // Check if button R2 is pressed

      intakeMotor.move_velocity(-3000); // Set intake motor to full speed backward

    } 
    
    else
    {
      intakeMotor.move(0);
    }

    if (master.get_digital_new_press(DIGITAL_L1)) { // Check if button R1 is pressed
      if(wallStakeToggleEnabled){
        // Do another thing
        wallStake.move_absolute(1500,2000);
      }
      else{
        
        wallStake.move_absolute(400,2000);

      }

    } 
    else if (master.get_digital_new_press(DIGITAL_L2)) { // Check if button R2 is pressed

        wallStake.set_brake_mode(MOTOR_BRAKE_COAST);
        wallStake.move_absolute(5,-1000);

    } 
    

    if(clampToggleEnabled){
      // Do another thing
      Clamper.set_value(1);
    }
    else{
      // Do initial thing
      Clamper.set_value(0);
    }

    if(rachetToggleEnabled){
      // Do another thing
      rachet.set_value(1);
    }
    else{
      // Do initial thing
      rachet.set_value(0);
    }


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
