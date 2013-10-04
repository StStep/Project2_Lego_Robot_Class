#include "SensorSuite.h"

extern "C" {

int GetLeftLightSen()
{
	return leftLight.getBrightness();
}

int GetRightLightSen()
{
	return rightLight.getBrightness();
}

int GetTouchSen()
{
	return touch.isPressed();
}

} //End Extern C