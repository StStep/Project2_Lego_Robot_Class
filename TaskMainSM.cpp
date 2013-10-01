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
	
	switch(next_state)
	{
	case INIT:
		PrintPlease = 1;
		next_state  = FWD_UNTIL_TAN;
		break;
	case FWD_UNTIL_TAN:
		PrintPlease = 2;
		if(align(isBlk(bright_l),isBlk(bright_l))) 
			next_state = STEP;
		else
			next_state = FWD_UNTIL_TAN;
		break;
	case STEP:
		PrintPlease = 3;
		if(align(isWht(bright_l),isWht(bright_l))) 
			next_state = ROTATE_ALIGN;
		else
			next_state = STEP;
		break;
	case ROTATE_ALIGN:
		PrintPlease = 4;
		next_state = state_rotate_align();
		break;
	default:
		PrintPlease = 0;
<<<<<<< HEAD
		next_state = IDLE;
		
		leftMotor.setPWM(0); // Left motor stop
		rightMotor.setPWM(0); // Right motor stop
		InternalEvent(ST_START);
		next_state = INIT;
=======
		next_state = state_idle();
		InternalEvent(ST_TRACK);
		break;
	default:
		next_state = state_init();
>>>>>>> Initial Track and Reach Waypoint(Gray)
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
	//editing begins 9/29
	//Sensor Input
	bright_l = leftLight.getBrightness(); // Left light sensor
	bright_r = rightLight.getBrightness(); // Right light sensor
	
	// LOCATION C: LOOP CODE
	switch(next_state_tt)
	{
	case CRUISE:
		PrintPlease = 5;
		next_state_tt = state_cruise();
		break;
	case CORNERTURNLEFT:
		PrintPlease = 6;
		next_state_tt = state_corner_turn_left();
		break;
	case CORNERTURNRIGHT:
		PrintPlease = 7;
		next_state_tt = state_corner_turn_right();
		break;
	case ALIGNGREY:
		PrintPlease = 8;
		next_state_tt = state_align_grey();
		break;
	case WAYPOINT:
		PrintPlease = 9;
		next_state_tt = state_waypoint();
		break;
	default:
		next_state_tt = state_cruise();
		InternalEvent(ST_TRACK);
		break;
	}
	//Jitter Mode Activate!
	//leftMotor.setPWM(NOSPEED); // Left motor stop
	//rightMotor.setPWM(NOSPEED); // Right motor stop
}
 
// Sit around and wait for touch event, at waypoint
void TaskMainSM::ST_Idle()
{
	bright_l = leftLight.getBrightness(); // Left light sensor
	bright_r = rightLight.getBrightness(); // Right light sensor
}


/** Find SM FUnctions for now **/
extern "C" 
{
bool align(bool isLeftTrue, bool isRightTrue)
{
<<<<<<< HEAD
	//Defualt return state
	bool ret =  false;
=======
	//Default return state
	state_t ret =  FWD_UNTIL_TAN;
>>>>>>> Initial Track and Reach Waypoint(Gray)
	
	//Both on line, change states
	if(isLeftTrue && isRightTrue)
	{
		leftMotor.setPWM(NOSPEED); // Left motor stop
		rightMotor.setPWM(NOSPEED); // Right motor stop
		ret = true;
	}
	else if(isRightTrue) 
	{
		leftMotor.setPWM(BASESPEED); // Left motor fwd
		rightMotor.setPWM(-HALFSPEED); // Right motor stop
		clock.wait(MOTORTIMESTEP); // Perform for duration of .1 seconds
	}
	else if(isLeftTrue) 
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

FindSM_states state_rotate_align(void)
{

	//Default return state
	FindSM_states ret =  ROTATE_ALIGN;
	
	if(tape_flag == false)
	{ 
		if (isBlk(bright_l)) //when black tape first seen by left
		{
			tape_flag = true;
		}
	}
	else if(tape_flag == true)
	{
		if (!(isBlk(bright_l)) || isBlk(bright_r)) //when left no longer on black
		{
			tape_flag = false;
			ret = IDLE;
		}
	}

	if (ret == ROTATE_ALIGN) 
	{
		leftMotor.setPWM(-BASESPEED); // Left motor forward
		rightMotor.setPWM(BASESPEED); // Right motor backwards
		clock.wait(MOTORTIMESTEP); // Perform for duration of .1 seconds
	}
	
	return ret;

}

<<<<<<< HEAD
/** Track SM FUnctions for now **/
=======
state_t state_idle(void)
{
	
	leftMotor.setPWM(0); // Left motor forward
	rightMotor.setPWM(0); // Right motor forward
		
	return IDLE;
}
/**----------------------------**/
/** Track SM Functions for now **/
/**----------------------------**/
state_tt state_cruise(void)
{
	state_tt ret = CRUISE;
	
	if (isGry(bright_l) || isGry(bright_r))
	{
		ret = ALIGNGREY;
	}
	
	else if (lfix > 4.0 || rfix >4.0)
	{
		if (lfix > 4.0) 			//then sharp right turn
		{
			leftMotor.setPWM(BASESPEED); 
			rightMotor.setPWM(BASESPEED); 
			ret = CORNERTURNRIGHT;
		}
		else if (rfix > 4.0)		//then sharp left turn
		{
			leftMotor.setPWM(BASESPEED); 
			rightMotor.setPWM(BASESPEED); 
			ret = CORNERTURNLEFT; 
		}
	}
	else if (isBlk(bright_r))
	{
		lfix += .25;
		if (lfix > 4.0)
		{
			lfix = 4.1;
		}
		leftMotor.setPWM(lfix*BASESPEED); // Left motor forward
		rightMotor.setPWM(-BASESPEED); // Right motor zero
		clock.wait(MOTORTIMESTEP); // Perform for duration of .1 seconds
	}
	else if (isBlk(bright_l))
	{
		rfix += .25;
		if (rfix > 4.0)
		{
			rfix = 4.1;
		}
		leftMotor.setPWM(-BASESPEED); // Left motor zero
		rightMotor.setPWM(rfix*BASESPEED); // Right motor backwards
		clock.wait(MOTORTIMESTEP); // Perform for duration of .1 seconds
	}
	else
	{
		rfix = 1.2;
		lfix = 1.2;
		leftMotor.setPWM(lfix*BASESPEED); // Left motor forward
		rightMotor.setPWM(rfix*BASESPEED); // Right motor forward
		clock.wait(MOTORTIMESTEP); // Perform for duration of .1 seconds
	}

	return ret;
	
}

state_tt state_corner_turn_left(void)
{
	state_tt ret = CORNERTURNLEFT;
	if (isBlk(bright_l))
	{
		leftMotor.setPWM(-2*BASESPEED); // Left motor forward
		rightMotor.setPWM(2*BASESPEED); // Right motor backwards
		clock.wait(MOTORTIMESTEP); // Perform for duration of .1 seconds
	}
	// if (isBlk(bright_l) && isBlk(bright_r))
	// {
		// leftMotor.setPWM(-BASESPEED); // Left motor forward
		// rightMotor.setPWM(3*BASESPEED); // Right motor backwards
		// clock.wait(MOTORTIMESTEP); // Perform for duration of .1 seconds
	// }
	
	else if (isWht(bright_l) && isWht(bright_r))
	{
		ret = CRUISE;
	}
	return ret;
}

state_tt state_corner_turn_right(void)
{
	state_tt ret = CORNERTURNRIGHT;
	
	if (isBlk(bright_r))
	{
		leftMotor.setPWM(2*BASESPEED); // Left motor forward
		rightMotor.setPWM(-2*BASESPEED); // Right motor backwards
		clock.wait(MOTORTIMESTEP); // Perform for duration of .1 seconds
	}
	// if (isBlk(bright_l) && isBlk(bright_r))
	// {
		// leftMotor.setPWM(3*BASESPEED); // Left motor forward
		// rightMotor.setPWM(-BASESPEED); // Right motor backwards
		// clock.wait(MOTORTIMESTEP); // Perform for duration of .1 seconds
	// }
	else if (isWht(bright_l) && isWht(bright_r))
	{
		ret = CRUISE;
	}
	return ret;
}

state_tt state_align_grey(void)
{
	
	//Default return state
	state_tt ret =  ALIGNGREY;
	if (isGry(bright_r) && isGry(bright_l))
	{
		ret = WAYPOINT;
	}
	else if (!isGry(bright_l) && !isGry(bright_r))  //guarding against random values
	{
		leftMotor.setPWM(BASESPEED); // Left motor forward
		rightMotor.setPWM(BASESPEED); // Right motor backwards
		ret = CRUISE;
	}
	else if (isGry(bright_l))
	{
		leftMotor.setPWM(NOSPEED); // Left motor forward
		rightMotor.setPWM(BASESPEED); // Right motor backwards
		clock.wait(MOTORTIMESTEP); // Perform for duration of .1 seconds
	}
	else if (isGry(bright_r))
	{
		leftMotor.setPWM(BASESPEED); // Left motor forward
		rightMotor.setPWM(NOSPEED); // Right motor backwards
		clock.wait(MOTORTIMESTEP); // Perform for duration of .1 seconds
	}
	if (isGry(bright_r) && isGry(bright_l))
	{
		ret = WAYPOINT;
	}
	return ret;
}
>>>>>>> Initial Track and Reach Waypoint(Gray)

state_tt state_waypoint(void)
{
	leftMotor.setPWM(NOSPEED); // Left motor forward
	rightMotor.setPWM(NOSPEED); // Right motor forward
	clock.wait(MOTORTIMESTEP); // Perform for duration of .1 seconds
	return WAYPOINT;
}
}

