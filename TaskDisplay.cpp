#include "TaskMain.h"
#include "LCDSuite.h"
#include "SensorSuite.h"


extern "C" 
{
/**
This task runs once every 250ms, I don't know if that should be the target time or not yet
**/
TASK(TaskDisplay)
{
		// Read the sensors at every iteration
    	int LeftLightSen = GetLeftLightSen();
   		int RightLightSen = GetRightLightSen();
		
		/** Create a LCD Task for this stuff**/
		// Display sensor information on LCD
		lcd.clear(); // Clear the current LCD display
		lcd.putf("sn", "NXT Sensors");
		lcd.putf("sddn", "Light: ", LeftLightSen,0,RightLightSen,5);
		lcd.putf("sdn", "RMMult:", (int) (RMMult*100.0),0);
		lcd.putf("sdn", "LMMult:",(int) (LMMult*100.0),0);
		lcd.putf("sdn", "In State = ", PrintPlease, 0);
        lcd.disp(); // Update the LCD display
	
	TerminateTask();
} // End Task
} //End Extern C
