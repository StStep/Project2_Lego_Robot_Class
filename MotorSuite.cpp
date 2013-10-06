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

int GetLeftMotCnt()
{
	return leftMotor.getCount();
}

int GetRightMotCnt()
{
	return rightMotor.getCount();
}

bool align(bool isLeftTrue, bool isRightTrue, float Mult)
{
	//Defualt return state
	bool ret =  false;

	//Both on line, change states
	if(isLeftTrue && isRightTrue)
	{
		MotorStep(NOSPEED, NOSPEED, 0);
		ret = true;
	}
	else if(isRightTrue) 
	{
		MotorStep((int) Mult*BASESPEED, -(int) Mult*HALFSPEED, MOTORTIMESTEP);
	}
	else if(isLeftTrue) 
	{
		MotorStep(-(int) Mult*HALFSPEED, (int) Mult*BASESPEED, MOTORTIMESTEP);
	}
	else //while neither of the sensors have detected black tape, move forward
	{
		MotorStep((int) Mult*BASESPEED, (int) Mult*BASESPEED, MOTORTIMESTEP);
	}

	return ret;

}

} //End Extern C