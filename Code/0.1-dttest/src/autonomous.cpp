#include "autonomous.h"
#include "lemlib/pose.hpp"
#include "setup.h" 

lemlib::Pose offsetPose(lemlib::Pose targetPose, float offset) {
    if (offset != 0) {
        targetPose.x -= offset * sin(lemlib::degToRad(targetPose.theta));
        targetPose.y -= offset * cos(lemlib::degToRad(targetPose.theta));
    }
    return {targetPose};
}

lemlib::Pose offsetPoint(lemlib::Pose targetPoint, lemlib::Pose prevPoint, float offset) {
    if (offset != 0) {
        targetPoint.theta = atan2(targetPoint.x - prevPoint.x, targetPoint.y - prevPoint.y);
        targetPoint.x -= offset * sin(lemlib::degToRad(targetPoint.theta));
        targetPoint.y -= offset * cos(lemlib::degToRad(targetPoint.theta));
    }
    return {targetPoint};
}

lemlib::Pose mirrorPose(lemlib::Pose pose) {
    return {-pose.x, pose.y, -pose.theta};
}

void moveStraight(float length, int timeout, lemlib::MoveToPointParams params) {
    if (chassis.isInMotion()) chassis.waitUntilDone();
    params.forwards = length > 0;
    lemlib::Pose pose = chassis.getPose();
    chassis.moveToPoint(pose.x + length * sin(lemlib::degToRad(pose.theta)),
                        pose.y + length * cos(lemlib::degToRad(pose.theta)), timeout, params);
}

//======================= pid tuning =======================

void pidtune() {
    // disable all other settings other than kd and kp
    // set position to x:0, y:0, heading:0
    chassis.setPose(0, 0, 0);
    lemlib::Pose pose = offsetPoint({0, 48, NAN},{0,0,0},8);
    std::printf("%.f %.f %.f\n", pose.x, pose.y, pose.theta);
    // turn to face heading 90 with a very long timeout
    //chassis.swingToHeading(45, DriveSide::LEFT, 100000);
    //chassis.turnToHeading(45, 100000);
    // increase kp until the robot oscillates then add kd until it stops
    //chassis.moveToPoint(0, 24, 10000);
    // chassis.moveToPoint(-48, 0, 10000);
    //chassis.turnToHeading(270, 100000);
    // chassis.moveToPoint(-48, 0, 10000);

    //chassis.turnToPoint(elims_rush_pos_red_7.x, elims_rush_pos_red_7.y, 1000, {.forwards=true}, true); // turn to stake
    //intake_solenoid.retract();
    //chassis.moveToPoint(elims_rush_pos_red_7.x, elims_rush_pos_red_7.y, 1000, {.forwards = true}, false);
    //chassis.turnToHeading(elims_rush_pos_red_7.theta, 500);
    pros::delay(2000);
    std::printf("%.1f %.1f %.1f\n", pose.x, pose.y, pose.theta);
    pros::delay(50000);

    //chassisPrintPose();
    //chassis.turnToHeading(95, 100000);
    //robot::chassisPrintPose();
}

//======================= qualis pos 6p safe rush auton =======================

void qual_safe_pos_blue() {}

//======================= qualis pos 9p goal rush AWP auton =======================

void qual_rush_pos_red() {}

void qual_rush_pos_blue() {}

//======================= elims pos 9p goal rush auton =======================

void elims_rush_pos_red() {}

void elims_rush_pos_blue() {}

//======================= skills autons =======================

void skills() {}
