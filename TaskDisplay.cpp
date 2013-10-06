#include "TaskMain.h"
#include "LCDSuite.h"
#include "SensorSuite.h"


extern "C" 
{
/**
This task runs once every 250ms, updates display, mainly for debug purposes, not much port potential
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
		lcd.putf("sdn", "In State = ", PrintPlease, 0);
        lcd.disp(); // Update the LCD display
	
	TerminateTask();
} // End Task
} //End Extern C
