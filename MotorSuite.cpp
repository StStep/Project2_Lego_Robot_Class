#include "MotorSuite.h"

extern "C" {

void MotorStep(int LeftPWM, int RightPWM, int TimeStep)
{
	int _LeftPWM = LeftPWM;
	int _RightPWM = RightPWM;

	if(_LeftPWM > FULLSPEED)
		_LeftPWM = FULLSPEED;
	else if(_LeftPWM < -FULLSPEED)
		_LeftPWM = -FULLSPEED;
	if(_RightPWM > FULLSPEED)
		_RightPWM = FULLSPEED;
	else if(_RightPWM < -FULLSPEED)
		_RightPWM = -FULLSPEED;
	
	leftMotor.setPWM(_LeftPWM); // Left motor forward
	rightMotor.setPWM(_RightPWM); // Right motor forward
	clock.wait(TimeStep); // Perform for duration of .1 seconds
}

} //End Extern C