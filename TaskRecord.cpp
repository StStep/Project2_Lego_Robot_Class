
#include "TaskMain.h"


extern "C" 
{
/**
This task runs once every 500ms, I don't know if that should be the target time or not yet
**/
TASK(TaskRecord)
{
	PublixTest++;
	
	TerminateTask();
} // End Task
} //End Extern C