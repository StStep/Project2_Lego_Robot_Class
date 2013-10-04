#include "MotorSuite.h"

extern "C" {

void MotorStep(int LeftPWM, int RightPWN, int TimeStep)
{
	int _LeftPWN = LeftPWM;
	int _RightPWN = RightPWN;

	if(_LeftPWM > FULLSPEED)
		_LeftPWM = FULLSPEED;
	else if(_LeftPWM < -FULLSPEED)
		_LeftPWM = -FULLSPEED;
	if(_RightPWN > FULLSPEED)
		_RightPWN = FULLSPEED;
	else if(_RightPWN < -FULLSPEED)
		_RightPWN = -FULLSPEED;
	
	leftMotor.setPWM(_LeftPWM); // Left motor forward
	rightMotor.setPWM(_RightPWN); // Right motor forward
	clock.wait(TimeStep); // Perform for duration of .1 seconds
}

} //End Extern C