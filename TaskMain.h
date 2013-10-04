#ifndef _TaskMain_
#define _TaskMain_

#include <math.h>
#include "SensorSuite.h"
#include "MotorSuite.h"
#include "LCDSuite.h"
#include "BTSuite.h"
#include "TaskMainSM.h"

using namespace ecrobot;

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
DeclareTask(TaskBTConnect);
DeclareTask(TaskDisplay);

/* State Machine Initionization */
TaskMainSM TaskMainSM_inst;

/* nxtOSEK hook on initilization */
void ecrobot_device_initialize(void)
{
}

/* nxtOSEK hook on termination */
void ecrobot_device_terminate(void)
{
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
 
