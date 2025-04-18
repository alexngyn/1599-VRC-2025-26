#include "setup.h"

pros::Controller master (pros::E_CONTROLLER_MASTER);
//pros::Controller partner (pros::E_CONTROLLER_PARTNER);

pros::MotorGroup dt_left (LEFT_MOTOR_PORTS, pros::MotorGearset::blue, pros::MotorEncoderUnits::degrees);
pros::MotorGroup dt_right (RIGHT_MOTOR_PORTS, pros::MotorGearset::blue, pros::MotorEncoderUnits::degrees);

pros::IMU inertial_sensor (INERTIAL_SENSOR_PORT);

color sideColor = color::unknown;

// drivetrain settings
lemlib::Drivetrain drivetrain {
    &dt_left, // left drivetrain motors
    &dt_right, // right drivetrain motors
    11.85, // track width 11.83
    lemlib::Omniwheel::NEW_275, // using new 2.75" omnis
    600, // drivetrain rpm is 450
    4 // prevents drifting
};

// lateral motion controller

lemlib::ControllerSettings linearController ( // 26,0,150      ,7
    10, // proportional gain (kP)
    0, // integral gain (kI)
    55, // derivative gain (kD)
    3, // anti windup
    1, // small error range, in inches
    100, // small error range timeout, in milliseconds
    3, // large error range, in inches
    500, // large error range timeout, in milliseconds
    20 // maximum acceleration (slew)
);

// angular motion controller
lemlib::ControllerSettings angularController ( // 5.2,0,45
    3, // proportional gain (kP)
    0, // integral gain (kI)
    20, // derivative gain (kD)
    3, // anti windup
    2, // small error range, in degrees
    100, // small error range timeout, in milliseconds
    5, // large error range, in degrees
    500, // large error range timeout, in milliseconds
    8 // maximum acceleration (slew) prevent wheel cuz we dont have encoder
);

// sensors for odometry
lemlib::OdomSensors sensors {
    nullptr, // vertical tracking wheel 1, &vertical_tracking_wheel
    nullptr, // vertical tracking wheel 2, set to nullptr as we don't have one
    nullptr, // horizontal tracking wheel 1,  set to nullptr as we don't have one
    nullptr, // horizontal tracking wheel 2, set to nullptr as we don't have a second one
    &inertial_sensor // inertial sensor
};

// create the chassis
lemlib::Chassis chassis(drivetrain, linearController, angularController, sensors);