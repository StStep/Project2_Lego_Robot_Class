#ifndef _MotorSuite_
#define _MotorSuite_

#include "Motor.h"
#include "Nxt.h"

using namespace ecrobot;

// Value for each motor to move full speed ahead
#define FULLSPEED 100

extern "C" {

// Initialize Actuators
Motor steering(PORT_A);
Motor rightMotor(PORT_C);
Motor leftMotor(PORT_B);

} //End Extern C
#endif 
 
