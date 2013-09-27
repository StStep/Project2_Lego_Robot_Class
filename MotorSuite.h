#ifndef _MotorSuite_
#define _MotorSuite_

#include "Motor.h"
#include "Nxt.h"

using namespace ecrobot;

extern "C" {

// Initialize Actuators
Motor steering(PORT_A);
Motor rightMotor(PORT_C);
Motor leftMotor(PORT_B);

} //End Extern C
#endif 
 
