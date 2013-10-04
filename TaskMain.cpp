
#include "TaskMain.h"

extern "C" 
{
TASK(TaskMain)
{	
	int bump = touch.isPressed(); //TODO: FInd better way to do this
	
	if(bump == 1 )
	{
		TaskMainSM_inst.Touch();
	}
	else
	{
		TaskMainSM_inst.Run();
	}
	
	TerminateTask();
	
} // End Task

} // End Extern C
