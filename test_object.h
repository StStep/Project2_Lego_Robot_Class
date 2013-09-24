#ifndef _test_object_
#define _test_object_

#include "TouchSensor.h"
// #include "Nxt.h"
// #include "ecrobot_interface.h"
// #include "kernel.h"
// #include "kernel_id.h"
#include "Port.h"

using namespace ecrobot;

TouchSensor  touch(PORT_4);

class test_object 
{

public:

	test_object(int flower)
	{
		this->flower = flower;	
	}
	
	virtual ~test_object() 
	{
	
	}
	
	void ProcessFlower();
	
	bool FlowerPartyYall();
	
	int GetFlower() 
	{
		return this->flower;
	}

protected:

	int flower;
	
};
#endif
