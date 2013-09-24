// ECRobot++ API (http://lejos-osek.sourceforge.net/html/index.html)
#include "LightSensor.h"
#include "SonarSensor.h"
#include "SoundSensor.h"
#include "TouchSensor.h"
#include "Clock.h"
#include "Lcd.h"
#include "Motor.h"
#include "Nxt.h"

#include "test_object.h"

using namespace ecrobot;

// Encoder value for the robot to be facing straight forward
#define CENTER 60

// Value for each motor to move full speed ahead
#define FULLSPEED -100

// Various operational states
#define FWD_MODE  0
#define SCAN_MODE 1
#define TURN_MODE 2
#define REV_MODE  3

// Time to wait (in milliseconds) between control updates
#define MAIN_WAIT 200

extern "C" {

#include "kernel.h"
#include "kernel_id.h"
#include "ecrobot_interface.h"

/* OSEK declarations */
DeclareCounter(SysTimerCnt);
DeclareTask(TaskMain); 
DeclareTask(TaskRecord);

//DeclareEvent(TouchSensorOnEvent); /* Event declaration */
//DeclareEvent(TouchSensorOffEvent); /* Event declaration */

// Initialize Sensors
LightSensor  leftLight(PORT_1, true); // init light sensor ON
LightSensor  rightLight(PORT_3, true);
SonarSensor  sonar(PORT_2);
//TouchSensor  touch(PORT_4);

// Initialize Actuators
Motor steering(PORT_A);
Motor rightMotor(PORT_B);
Motor leftMotor(PORT_C);

// Initialize Clock
Clock clock;

// Initialize LCD
Lcd lcd;

int PublixTest = 0;	

//Initialize Test Object
test_object *FlowerPower;

/* nxtOSEK hook on initilization */
void ecrobot_device_initialize(void)
{
  //nxt_motor_set_speed(NXT_PORT_A, 0, 1); 
}

/* nxtOSEK hook on termination */
void ecrobot_device_terminate(void)
{
  //nxt_motor_set_speed(NXT_PORT_A, 0, 1); 
}

/* nxtOSEK hook to be invoked from an ISR in category 2 */
/* Called every 1ms */
void user_1ms_isr_type2(void)
{
	SleeperMonitor(); // needed for I2C device and Clock classes
	
	StatusType ercd;

	ercd = SignalCounter(SysTimerCnt); /* Increment OSEK Alarm Counter */
	if (ercd != E_OK)
	{
		ShutdownOS(ercd);
	}
}

} //End Extern C
