
#include "TaskMain.h"


extern "C" 
{
/**
This task runs once every 500ms, I don't know if that should be the target time or not yet
**/
TASK(TaskRecord)
{
	PublixTestCnt += 0.1;
	PublixTest = (int) 100*sin(PublixTestCnt);
	
	TerminateTask();
} // End Task
} //End Extern C
