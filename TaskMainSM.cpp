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
	Find_Next_State = INIT;

}

// SM attempts to find the line
void TaskMainSM::ST_Find()
{
	//Sensor Input
	bright_l = leftLight.getBrightness(); // Left light sensor
	bright_r = rightLight.getBrightness(); // Right light sensor

	switch(Find_Next_State)
	{
	case INIT:
		PrintPlease = 1;
		Find_Next_State  = FWD_UNTIL_TAN;
		break;
	case FWD_UNTIL_TAN:
		PrintPlease = 2;
		if(align(isBlk(bright_l),isBlk(bright_r))) 
			Find_Next_State = STEP;
		else
			Find_Next_State = FWD_UNTIL_TAN;
		break;
	case STEP:
		PrintPlease = 3;
		if(align(isWht(bright_l),isWht(bright_r))) 
			Find_Next_State = ROTATE_ALIGN;
		else
			Find_Next_State = STEP;
		break;
	case ROTATE_ALIGN:
		PrintPlease = 4;
		Find_Next_State = FS_rotate_align();
		break;
	default:
		Find_Next_State = INIT;
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

	//Sensor Input
	bright_l = leftLight.getBrightness(); // Left light sensor
	bright_r = rightLight.getBrightness(); // Right light sensor
	
	// LOCATION C: LOOP CODE
	switch(Track_Next_State)
	{
	case CRUISE:
		PrintPlease = 5;
		Track_Next_State = TS_cruise();
		break;
	case CORNERTURNLEFT:
		PrintPlease = 6;
		Track_Next_State = TS_corner_turn_left();
		break;
	case CORNERTURNRIGHT:
		PrintPlease = 7;
		Track_Next_State = TS_corner_turn_right();
		break;
	case ALIGNGREY:
		PrintPlease = 8;
		if(align(isGry(bright_l),isGry(bright_r))) 
			Track_Next_State = WAYPOINT;
		else
			Track_Next_State = ALIGNGREY;
		break;
	case WAYPOINT:
		PrintPlease = 0;
		Track_Next_State = CRUISE;
		
		leftMotor.setPWM(NOSPEED); // Left motor forward
		rightMotor.setPWM(NOSPEED); // Right motor forward
		
		InternalEvent(ST_IDLE);
		break;
	default:
		Track_Next_State = CRUISE;
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

FindSM_state FS_rotate_align(void)
{

	//Default return state
	FindSM_state ret =  ROTATE_ALIGN;

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
	TrackSM_state TS_cruise(void)
	{
		TrackSM_state ret = CRUISE;
		
		//Speed Multiplier Bounds
		// if (LMMult > MAX_SP_MULT)
			// LMMult = MAX_SP_MULT;
		// else if (LMMult < DFLT_SP_MULT)
			// LMMult = DFLT_SP_MULT;
		
		if (isGry(bright_l) || isGry(bright_r))
		{
			GryCnt +=1;
			if (GryCnt > GRY_CNT_THRESH)
				{
					GryCnt = 0;
					ret = ALIGNGREY;
					
					RMMult = 0.0; //TESTING
					LMMult = 0.0; //TESTING
				}
		}		
		else if (LMMult >= MAX_SP_MULT || RMMult >= MAX_SP_MULT)
		{
			GryCnt = 0;
			if (LMMult >= MAX_SP_MULT) 			//then sharp right turn
			{
				leftMotor.setPWM(NOSPEED); 
				rightMotor.setPWM(NOSPEED); 
				ret = WAYPOINT;//CORNERTURNRIGHT;
				RMMult = 0.0; //TESTING
				LMMult = 0.0; //TESTING
			}
			else if (RMMult >= MAX_SP_MULT)		//then sharp left turn
			{
				leftMotor.setPWM(NOSPEED); 
				rightMotor.setPWM(NOSPEED); 
				ret = WAYPOINT;//CORNERTURNLEFT; 
				RMMult = 0.0; //TESTING
				LMMult = 0.0; //TESTING
			}
		}
		else if (isBlk(bright_r))
		{
			GryCnt = 0;
			LMMult += .25;
			RMMult -= .1;
			
			if (LMMult > MAX_SP_MULT)
				LMMult = MAX_SP_MULT;
			else if (LMMult < DFLT_SP_MULT)
				LMMult = DFLT_SP_MULT;
			leftMotor.setPWM(LMMult*BASESPEED); // Left motor forward
			rightMotor.setPWM(-LMMult*HALFSPEED); // Right motor zero
			clock.wait(MOTORTIMESTEP); // Perform for duration of .1 seconds
		}
		else if (isBlk(bright_l))
		{	
			GryCnt = 0;
			LMMult -= .1;
			RMMult += .25;
			
			if (RMMult > MAX_SP_MULT)
				RMMult = MAX_SP_MULT;
			else if(RMMult<DFLT_SP_MULT)
				RMMult = DFLT_SP_MULT;
			leftMotor.setPWM(-LMMult*HALFSPEED); // Left motor zero
			rightMotor.setPWM(RMMult*BASESPEED); // Right motor backwards
			clock.wait(MOTORTIMESTEP); // Perform for duration of .1 seconds
		}
		else
		{
			GryCnt = 0;
			RMMult = DFLT_SP_MULT;
			LMMult = DFLT_SP_MULT;
			leftMotor.setPWM(BASESPEED); // Left motor forward
			rightMotor.setPWM(BASESPEED); // Right motor forward
			clock.wait(MOTORTIMESTEP); // Perform for duration of .1 seconds
		}
		
			
		//Use Motors
		

		return ret;
		
	}

	TrackSM_state TS_corner_turn_left(void)
	{
		TrackSM_state ret = CORNERTURNLEFT;
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

	TrackSM_state TS_corner_turn_right(void)
	{
		TrackSM_state ret = CORNERTURNRIGHT;
		
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