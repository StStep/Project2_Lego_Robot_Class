#ifndef _MotorSuite_
#define _MotorSuite_

#include "Motor.h"
#include "Nxt.h"

using namespace ecrobot;

// Value for each motor to move full speed ahead
#define FULLSPEED 100
#define BASESPEED 33
#define NOSPEED 0

#define MOTORTIMESTEP 100

extern "C" {

// Initialize Actuators
Motor steering(PORT_A);
Motor rightMotor(PORT_B);
Motor leftMotor(PORT_C);

} //End Extern C
#endif 
 
