
#include "TaskMain.h"

extern "C" 
{
TASK(TaskMain)
{	
	
	// Read the Touch and Light Sensor
	int Bump = GetTouchSen();
	LightData* lData = new LightData;
	lData->LeftLightSen = GetLeftLightSen();
	lData->RightLightSen = GetRightLightSen();
	
	if(Bump == 1 )
	{
		TaskMainSM_inst.Touch(lData);
	}
	else
	{	
		TaskMainSM_inst.Run(lData);
	}
	
	TerminateTask();
	
} // End Task
} // End Extern C
