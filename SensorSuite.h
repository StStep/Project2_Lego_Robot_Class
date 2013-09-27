#ifndef _SensorSuite_
#define _SensorSuite_

// ECRobot++ API (http://lejos-osek.sourceforge.net/html/index.html)
#include "LightSensor.h"
#include "SonarSensor.h"
#include "SoundSensor.h"
#include "TouchSensor.h"
#include "Clock.h"
#include "Motor.h"
#include "Nxt.h"

using namespace ecrobot;

extern "C" {

// Initialize Clock
Clock clock;

// Initialize Sensors
LightSensor  leftLight(PORT_1, true); // init light sensor ON
LightSensor  rightLight(PORT_3, true);
SonarSensor  sonar(PORT_2);
TouchSensor  touch(PORT_4);

} //End Extern C
#endif 
 
