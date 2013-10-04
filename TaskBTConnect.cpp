
#include "TaskMain.h"
#include "BTSuite.h"

using namespace ecrobot;
extern "C" 
{
TASK(TaskBTConnect)
{
	//BLUETOOTH Connect
	if(btConnection.connect(PASSKEY) == 1)
		BTConnected = true;
		
	TerminateTask();
} // End Task
} //End Extern C
