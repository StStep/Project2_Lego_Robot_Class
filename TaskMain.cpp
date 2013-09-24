
#include "TaskMain.h"

extern "C" 
{
/**
   Main Loop
**/
TASK(TaskMain)
{
	// LOCATION B: LOOP INITIALIZATION CODE
	FlowerPower = new test_object(4);
	
	FlowerPower->ProcessFlower(); 
	
	// Endless loop
	while(1)
	{	
		// LOCATION C: LOOP CODE
		// Read the sensors at every iteration
        int dist = sonar.getDistance(); // Read distance (sonar) sensor
        int bump = FlowerPower->FlowerPartyYall(); // Read touch (bump) sensor touch.isPressed();//
        int bright_l = leftLight.getBrightness(); // Left light sensor
        int bright_r = rightLight.getBrightness(); // Right light sensor
        int steer_count = steering.getCount(); // Get steering encoder
        // Display sensor information on LCD
        lcd.clear(); // Clear the current LCD display
        lcd.putf("sn", "NXT Sensors");
        lcd.putf("sddn", "Light: ", bright_l,0,bright_r,5);
        lcd.putf("sdn", "Dist: ", dist, 0); // In centimeters
        lcd.putf("sdn", "Bump: ", bump,0); // Binary
        lcd.putf("sdn", "Steer: ", steer_count,0); // In degrees
		lcd.putf("sdn", "Flowers: ", FlowerPower->GetFlower(),0); // In flowers
		lcd.putf("sdn", "PublixTest: ", PublixTest,0); // In flowers
        lcd.disp(); // Update the LCD display
		
		// Wait some time between iterations
		clock.wait(MAIN_WAIT); //TODO: Find a better timing method than this later, shouldn't need
	}
	
	delete FlowerPower;
} // End Task

} // End Extern C
