
#include "TaskMain.h"
#include "BTSuite.h"

using namespace ecrobot;
extern "C" 
{
//This Task waits for a Bluetooth connection and is non-intruruptable during then. It can be skipped with the orange lego button
TASK(TaskBTConnect)
{
	//BLUETOOTH Connect
	if(btConnection.connect(PASSKEY) == 1)
		BTConnected = true;
		
	TerminateTask();
} // End Task
} //End Extern C
