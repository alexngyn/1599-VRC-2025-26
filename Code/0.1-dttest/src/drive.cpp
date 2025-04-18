/**
 * Drivetrain stuff
 */
#include "main.h"


#define DRIVE_DEADBAND 2

int scaleValue = 100; // for practice only

static float expDriveCurve(float input, float scale, int type) {
    if (scale != 0) {
        if (type==0) {return int((powf(2.718, -(scale / 10)) + powf(2.718, (fabs(input) - 127) / 10) * (1 - powf(2.718, -(scale / 10)))) * input);}
		if (type==1) {return int((powf(2.718, (((fabs(input)-127)*scale)/1000))) * input);}
		//https://www.desmos.com/calculator/ntf6aqvcqf
		// blue is 1 red is 0 purple is 2
		if (type==2) { //sinusoidal curve (twice)
			double denominator = sin(M_PI / 2 * scale);
			double firstRemapIteration = sin(M_PI / 2 * scale * input/127) / denominator;
			return int(sin(M_PI / 2 * scale * firstRemapIteration) / denominator * 127);
		}
    }
    return input;
}

double prevTurn = 0.0;
double prevThrottle = 0.0;

std::pair<float, float> arcadeDrive(float throttle, float turn, float curveGainY = 0, float curveGainX = 0,  float curveGainX2 = 0, int curveTypeY = 0, int curveTypeX = 0) {

    bool turnInPlace = false;
	if (fabs(throttle) < DRIVE_DEADBAND && fabs(turn) > DRIVE_DEADBAND) {
		// The controller joysticks can output values near zero when they are
		// not actually pressed. In the case of small inputs like this, we
		// override the throttle value to 0.
		turnInPlace = true;
	} 

	double left, right;
	if (turnInPlace) {
		float angularCmd = expDriveCurve(turn, curveGainX2, curveTypeX);
		left = angularCmd;
		right = -angularCmd;

	} else {

		float angularCmd = expDriveCurve(turn, curveGainX, curveTypeX);; 
		float linearCmd = expDriveCurve(throttle, curveGainY, curveTypeY);

		right = left = linearCmd;
		left += angularCmd;
		right -= angularCmd;
	}

    return std::make_pair(left, right);

}

std::pair<float, float> arcade(int throttle, int turn, float curveGainY = 0, float curveGainX = 0, int curveTypeY = 0, int curveTypeX = 0) {
    throttle = expDriveCurve(throttle, curveGainY, curveTypeY);
    turn = expDriveCurve(turn, curveGainX, curveTypeX);

    float leftPower = throttle + turn;
    float rightPower = throttle - turn;
    return std::make_pair(leftPower, rightPower);
}

//drive function

std::pair<float, float> scale(float left, float right, int s = 100) {
	left *= s/100.0;
    right *= s/100.0;
    return std::make_pair(left, right);
}

void drive() {
	while (true) {
        if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_X)) {
            scaleValue += 10;
        } else if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_B)) {
            scaleValue -= 10;
        }

        double power = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
		double turn = master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);

		auto [left, right] = arcadeDrive(power, turn, 
													6.2, 4.2, 5.8,
													1, 0); //14.6

		// auto [left, right] = arcade(power, turn, 6.2,4.2, 1, 0); //14.6
        auto [sleft, sright] = scale(left, right, scaleValue);

	    dt_left.move(sleft);
	    dt_right.move(sright);

		pros::delay(20);
	}
}