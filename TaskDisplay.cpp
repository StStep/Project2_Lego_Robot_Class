
#include "TaskMain.h"
#include "LCDSuite.h"


extern "C" 
{
/**
This task runs once every 250ms, I don't know if that should be the target time or not yet
**/
TASK(TaskDisplay)
{
		// Read the sensors at every iteration
		int dist = sonar.getDistance(); // Read distance (sonar) sensor
		int bright_l = leftLight.getBrightness(); // Left light sensor
		int bright_r = rightLight.getBrightness(); // Right light sensor
		int steer_count = steering.getCount(); // Get steering encoder
		
		/** Create a LCD Task for this stuff**/
		// Display sensor information on LCD
		lcd.clear(); // Clear the current LCD display
		lcd.putf("sn", "NXT Sensors");
		lcd.putf("sddn", "Light: ", bright_l,0,bright_r,5);
		lcd.putf("sdn", "RMMult:", (int) (RMMult*100.0),0);
		lcd.putf("sdn", "LMMult:",(int) (LMMult*100.0),0);
		lcd.putf("sdn", "Sine Test:", PublixTest, 0);
		lcd.putf("sdn", "In State = ", PrintPlease, 0);
        lcd.disp(); // Update the LCD display
	
	TerminateTask();
} // End Task
} //End Extern C