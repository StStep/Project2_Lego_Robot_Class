#ifndef _TaskMain_
#define _TaskMain_

#include "SensorSuite.h"
#include "MotorSuite.h"

#include "Lcd.h"

//#include "test_object.h"
#include "TaskMainSM.h"

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

#include <new>
#include <malloc.h>

//Overide new and delete to reduce file size
void* operator new(std::size_t size) {
    return malloc(size);
}

void* operator new[](std::size_t size) {
    return malloc(size);
}

void operator delete(void* ptr) {
    free(ptr);
}

void operator delete[](void* ptr) {
    free(ptr);
}

extern "C" {

//Overide Virtual Function call to reduce file size
void __cxa_pure_virtual() { while(1); }

#include "kernel.h"
#include "kernel_id.h"
#include "ecrobot_interface.h"

/* OSEK declarations */
DeclareCounter(SysTimerCnt);
DeclareTask(TaskMain); 
DeclareTask(TaskRecord);

// Initialize LCD
Lcd lcd;

int PublixTest = 0;	

TaskMainSM *TaskMainSM_inst;

/* nxtOSEK hook on initilization */
void ecrobot_device_initialize(void)
{
  //nxt_motor_set_speed(NXT_PORT_A, 0, 1); 
  TaskMainSM_inst = new TaskMainSM();
}

/* nxtOSEK hook on termination */
void ecrobot_device_terminate(void)
{
  //nxt_motor_set_speed(NXT_PORT_A, 0, 1); 
  delete TaskMainSM_inst;
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
#endif 
 
