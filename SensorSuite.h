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

//Light Sensor Stuff
// values for colored tape
#define WHITEHIGH 600
#define WHITELOW 540
#define BLACKHIGH 385
#define BLACKLOW 325

//Light Sensor Defines
#define isBlk(x) (x < BLACKHIGH && x >BLACKLOW)
#define isWht(x) (x < WHITEHIGH && x >WHITELOW)

//typedef enum{BLACK, GREY, WHITE} Color;

extern "C" 
{

// Initialize Clock
Clock clock;

// Initialize Sensors
LightSensor  leftLight(PORT_3, true); // init light sensor ON
LightSensor  rightLight(PORT_1, true);
SonarSensor  sonar(PORT_2);
TouchSensor  touch(PORT_4);

} //End Extern C
#endif 
 
