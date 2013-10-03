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
		if(align(isBlk(bright_l),isBlk(bright_r))) 
			next_state = STEP;
		else
			next_state = FWD_UNTIL_TAN;
		break;
	case STEP:
		PrintPlease = 3;
		if(align(isWht(bright_l),isWht(bright_r))) 
			next_state = ROTATE_ALIGN;
		else
			next_state = STEP;
		break;
	case ROTATE_ALIGN:
		PrintPlease = 4;
		next_state = state_rotate_align();
		break;
	default:
		next_state = INIT;
		PrintPlease = 0;

		leftMotor.setPWM(0); // Left motor stop
		rightMotor.setPWM(0); // Right motor stop
		InternalEvent(ST_TRACK);
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
		if(align(isGry(bright_l),isGry(bright_r))) 
			next_state_tt = WAYPOINT;
		else
			next_state_tt = ALIGNGREY;
		break;
	case WAYPOINT:
		PrintPlease = 0;
		next_state_tt = CRUISE;
		
		leftMotor.setPWM(NOSPEED); // Left motor forward
		rightMotor.setPWM(NOSPEED); // Right motor forward
		
		InternalEvent(ST_IDLE);
		break;
	default:
		next_state_tt = state_cruise();
		break;
	}
	//Jitter Mode Activate!
	//leftMotor.setPWM(NOSPEED); // Left motor stop
	//rightMotor.setPWM(NOSPEED); // Right motor stop

}
 
// Sit around and wait for touch event, at waypoint
void TaskMainSM::ST_Idle()
{
}


/**---------------------------------**/
/** Find SM Functions for now **/
/**--------------------------------**/
extern "C" 
{
bool align(bool isLeftTrue, bool isRightTrue)
{
	//Defualt return state
	bool ret =  false;

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

/**----------------------------------**/
/** Track SM Functions for now **/
/**---------------------------------**/
extern "C" 
{
	state_tt state_cruise(void)
	{
		state_tt ret = CRUISE;
		
		//Speed Multiplier Bounds
		// if (lfix > MAX_SP_MULT)
			// lfix = MAX_SP_MULT;
		// else if (lfix < DFLT_SP_MULT)
			// lfix = DFLT_SP_MULT;
		
		if (isGry(bright_l) || isGry(bright_r))
		{
			greychecker +=1;
			if (greychecker > GREYCOUNTTHRESHOLD)
				{
					greychecker = 0;
					ret = ALIGNGREY;
					
					rfix = 0.0; //TESTING
					lfix = 0.0; //TESTING
				}
		}		
		else if (lfix > MAX_SP_MULT || rfix >MAX_SP_MULT)
		{
			greychecker = 0;
			if (lfix > MAX_SP_MULT) 			//then sharp right turn
			{
				leftMotor.setPWM(NOSPEED); 
				rightMotor.setPWM(NOSPEED); 
				ret = WAYPOINT;//CORNERTURNRIGHT;
				rfix = 0.0; //TESTING
				lfix = 0.0; //TESTING
			}
			else if (rfix > MAX_SP_MULT)		//then sharp left turn
			{
				leftMotor.setPWM(NOSPEED); 
				rightMotor.setPWM(NOSPEED); 
				ret = WAYPOINT;//CORNERTURNLEFT; 
				rfix = 0.0; //TESTING
				lfix = 0.0; //TESTING
			}
		}
		else if (isBlk(bright_r))
		{
			greychecker = 0;
			lfix += .25;
			rfix -= .1;
			
			if (lfix > MAX_SP_MULT)
				lfix = 2.1;
			else if (lfix < DFLT_SP_MULT)
				lfix = 1.0;
			leftMotor.setPWM(lfix*BASESPEED); // Left motor forward
			rightMotor.setPWM(-lfix*HALFSPEED); // Right motor zero
			clock.wait(MOTORTIMESTEP); // Perform for duration of .1 seconds
		}
		else if (isBlk(bright_l))
		{	
			greychecker = 0;
			lfix -= .1;
			rfix += .25;
			
			if (rfix > 2.0)
				rfix = 2.1;
			else if(rfix<1.0)
				rfix = 1.0;
			leftMotor.setPWM(-lfix*HALFSPEED); // Left motor zero
			rightMotor.setPWM(rfix*BASESPEED); // Right motor backwards
			clock.wait(MOTORTIMESTEP); // Perform for duration of .1 seconds
		}
		else
		{
			greychecker = 0;
			rfix = 1.0;
			lfix = 1.0;
			leftMotor.setPWM(BASESPEED); // Left motor forward
			rightMotor.setPWM(BASESPEED); // Right motor forward
			clock.wait(MOTORTIMESTEP); // Perform for duration of .1 seconds
		}
		
			
		//Use Motors
		

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
}
}