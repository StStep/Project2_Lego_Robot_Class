#include <assert.h>
#include "TaskMainSM.h"
 
  /**External Event Transition Definitions**/
 
// Trigger States waiting for a touch event
void TaskMainSM::Touch(void)
{
    // given the Halt event, transition to a new state based upon 
    // the current state of the state machine
    BEGIN_TRANSITION_MAP                      				// - Current State -
		TRANSITION_MAP_ENTRY (ST_FIND)				  	// ST_Start
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)  	// ST_Find
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)     // ST_Track
        TRANSITION_MAP_ENTRY (ST_TRACK)              	// ST_Idle
    END_TRANSITION_MAP(NULL)
}
 
 //The general run call for SM
void TaskMainSM::Run(void)
{
    // given the Halt event, transition to a new state based upon 
    // the current state of the state machine
    BEGIN_TRANSITION_MAP                      				// - Current State -
		TRANSITION_MAP_ENTRY (EVENT_IGNORED)  	// ST_Start
        TRANSITION_MAP_ENTRY (ST_FIND)  					// ST_Find
        TRANSITION_MAP_ENTRY (ST_TRACK)        		// ST_Track
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)     // ST_Idle
    END_TRANSITION_MAP(NULL)
}

//Go back to the Start state
void TaskMainSM::Reset(void)
{
    // given the Halt event, transition to a new state based upon 
    // the current state of the state machine
    BEGIN_TRANSITION_MAP                      				// - Current State -
		TRANSITION_MAP_ENTRY (ST_START)  				// ST_Start
		TRANSITION_MAP_ENTRY (ST_START)  				// ST_Find
		TRANSITION_MAP_ENTRY (ST_START)        		// ST_Track
		TRANSITION_MAP_ENTRY (ST_START)              	// ST_Idle
    END_TRANSITION_MAP(NULL)
}
 
 /**State Definitions**/
 

// SM starts here, and does initilizaing thing? or nothing
void TaskMainSM::ST_Start() 
{
	//Remove Later
	next_state = INIT;
	
}

// SM attempts to find the line
void TaskMainSM::ST_Find()
{
	//Sensor Input
	bright_l = leftLight.getBrightness(); // Left light sensor
	bright_r = rightLight.getBrightness(); // Right light sensor
	
	// LOCATION C: LOOP CODE
	switch(next_state)
	{
	case INIT:
		PrintPlease = 1;
		next_state  = state_init();
		break;
	case FWD_UNTIL_TAN:
		PrintPlease = 2;
		next_state = state_fwd_until_tan();
		break;
	case STEP:
		PrintPlease = 3;
		next_state = state_step();
		break;
	case ROTATE_ALIGN:
		PrintPlease = 4;
		next_state = state_rotate_align();
		break;
	case IDLE:
		PrintPlease = 0;
		next_state = state_idle();
		InternalEvent(ST_START);
		break;
	default:
		next_state = state_init();
		InternalEvent(ST_START);
		break;
	}
	
	//Jitter Mode Activate!
	//leftMotor.setPWM(NOSPEED); // Left motor stop
	//rightMotor.setPWM(NOSPEED); // Right motor stop

}
 
// Follow the line
void TaskMainSM::ST_Track()
{
}
 
// Sit around and wait for touch event, at waypoint
void TaskMainSM::ST_Idle()
{
}


/** Find SM FUnctions for now **/
extern "C" 
{
state_t state_init(void)
{
	return FWD_UNTIL_TAN;
}

state_t state_fwd_until_tan(void)
{
	//Defualt return state
	state_t ret =  FWD_UNTIL_TAN;
	
	//Both on line, change states
	if((isBlk(bright_l) && isBlk(bright_r)))
	{
		leftMotor.setPWM(NOSPEED); // Left motor stop
		rightMotor.setPWM(NOSPEED); // Right motor stop
		ret = STEP;
	}
	else if(isBlk(bright_r)) 
	{
		leftMotor.setPWM(BASESPEED); // Left motor fwd
		rightMotor.setPWM(-HALFSPEED); // Right motor stop
		clock.wait(MOTORTIMESTEP); // Perform for duration of .1 seconds
	}
	else if(isBlk(bright_l)) 
	{
		leftMotor.setPWM(-HALFSPEED); // Left motor fwd
		rightMotor.setPWM(BASESPEED); // Right motor stop
		clock.wait(MOTORTIMESTEP); // Perform for duration of .1 seconds
	}
	else //while neither of the sensors have detected black tape, move forward
	{
		leftMotor.setPWM(BASESPEED); // Left motor forward
		rightMotor.setPWM(BASESPEED); // Right motor forward
		clock.wait(MOTORTIMESTEP); // Perform for duration of .1 seconds
	}
	
	return ret;
	
}

state_t state_step(void)
{
	
	//Default return state
	state_t ret =  STEP;
	
	//Both on line, change states
	if((isWht(bright_l) && isWht(bright_r)))
	{
		leftMotor.setPWM(NOSPEED); // Left motor stop
		rightMotor.setPWM(NOSPEED); // Right motor stop
		ret = ROTATE_ALIGN;
	}
	else if(isWht(bright_r)) 
	{
		leftMotor.setPWM(BASESPEED); // Left motor fwd
		rightMotor.setPWM(-HALFSPEED); // Right motor stop
		clock.wait(MOTORTIMESTEP); // Perform for duration of .1 seconds
	}
	else if(isWht(bright_l)) 
	{
		leftMotor.setPWM(-HALFSPEED); // Left motor fwd
		rightMotor.setPWM(BASESPEED); // Right motor stop
		clock.wait(MOTORTIMESTEP); // Perform for duration of .1 seconds
	}
	else //while neither of the sensors have detected black tape, move forward
	{
		leftMotor.setPWM(BASESPEED); // Left motor forward
		rightMotor.setPWM(BASESPEED); // Right motor forward
		clock.wait(MOTORTIMESTEP); // Perform for duration of .1 seconds
	}
	
	return ret;
}


state_t state_rotate_align(void)
{

	//Default return state
	state_t ret =  ROTATE_ALIGN;
	
	if(tape_flag == 0)
	{ 
		if (isBlk(bright_l)) //when black tape first seen by left
		{
			tape_flag = 1;
		}
	}
	else if(tape_flag == 1)
	{
		if (!(isBlk(bright_l)) || isBlk(bright_r)) //when left no longer on black
		{
			tape_flag = 0;
			ret = IDLE;
		}
	}

	if (ret == ROTATE_ALIGN) 
	{
		leftMotor.setPWM(-BASESPEED); // Left motor forward
		rightMotor.setPWM(BASESPEED); // Right motor backwards
		clock.wait(MOTORTIMESTEP); // Perform for duration of .1 seconds
	}
	//bright_l = leftLight.getBrightness(); // Update light data
	//bright_r = rightLight.getBrightness();

	
	return ret;

}

state_t state_idle(void)
{
	
	leftMotor.setPWM(0); // Left motor forward
	rightMotor.setPWM(0); // Right motor forward
		
	return IDLE;
}

/** Track SM FUnctions for now **/

}

