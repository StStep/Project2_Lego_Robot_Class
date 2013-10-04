
#include "TaskMain.h"
#include "MotorSuite.h"
#include "LCDSuite.h"
#include "BTSuite.h"
using namespace ecrobot;
extern "C" 
{
/**
This task runs once every 500ms, I don't know if that should be the target time or not yet
**/
TASK(TaskRecord)
{
	if(BTConnected) 
	{
		dataS08[0]++;
		dataS08[1]--;
		dataS32++;
		daq.send(dataS08, dataS32);
	}
	
	TerminateTask();
} // End Task
} //End Extern C
