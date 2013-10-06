#ifndef _BTSuite_h_
#define _BTSuite_h_

//#include <string>
#include "Lcd.h"
#include "Nxt.h"
#include "Daq.h" // DAQ class for NXT GamePad
#include "Bluetooth.h"
#include "BTConnection.h"

static const CHAR* PASSKEY = "1234";

using namespace ecrobot;

extern "C" {
	// Initialize Bluettooth obj.
	Bluetooth BlueTooth;
	Nxt NXTDevice;
	
	// Initialize Daq object from bluetooth
	Daq daq(BlueTooth);
	S8 dataS08[2] = {0};
	S32 dataS32 = 0;
	BTConnection btConnection(BlueTooth, lcd, NXTDevice);
	
	//Using Bluetooth?
	bool BTConnected = false;

} //End Extern C
#endif 
 
