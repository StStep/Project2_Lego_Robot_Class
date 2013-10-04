#ifndef _MotorSuite_
#define _MotorSuite_

#include "Motor.h"
#include "Clock.h"
#include "Nxt.h"

using namespace ecrobot;

// Value for each motor to move full speed ahead
#define FULLSPEED 100
#define BASESPEED 30
#define HALFSPEED 15
#define QUARTSPEED 15
#define NOSPEED 0

//Values for SPeed Multipliers
#define MAX_SP_MULT 2.0
#define DFLT_SP_MULT 1.0


#define MOTORTIMESTEP 50

extern "C" {

// Initialize Clock
Clock clock;

// Initialize Actuators
Motor steering(PORT_A);
Motor rightMotor(PORT_B);
Motor leftMotor(PORT_C);

//Function Declarations
void MotorStep(int LeftPWM, int RightPWN, int TimeStep);

} //End Extern C
#endif 
 
