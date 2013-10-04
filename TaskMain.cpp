
#include "TaskMain.h"

extern "C" 
{
TASK(TaskMain)
{	
	// Endless loop
	while(1)
	{	
		if(bump == 1 )
		{
			TaskMainSM_inst.Touch();
		}
		else
		{
			TaskMainSM_inst.Run();
		}
		
		// Wait some time between iterations
		clock.wait(MAIN_WAIT); //TODO: Find a better timing method than this later, shouldn't need
	}
	
} // End Task

} // End Extern C
