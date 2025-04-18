#pragma once

#include "api.h" // IWYU pragma: keep

enum color { red, blue, unknown };

std::string colorToString(color color);

extern color sideColor; // what side we on

//controllers
extern pros::Controller master;
// extern pros::Controller partner;

// drivetrain
#define RIGHT_MOTOR_PORTS { 3, 4, 5 }
#define LEFT_MOTOR_PORTS { -8, -9, -10 }

//#define HORIZONTAL_ENCODER_PORT 10
#define VERTICAL_ENCODER_PORT -6
//extern pros::Rotation horizontal_encoder;
extern pros::Rotation vertical_encoder;

#define INTAKE_SOLENOID_PORT 'C'
extern pros::adi::Pneumatics intake_solenoid;

extern pros::MotorGroup dt_left;
extern pros::MotorGroup dt_right;
extern lemlib::Chassis chassis;

#define INERTIAL_SENSOR_PORT 18
extern pros::IMU inertial_sensor;
