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
	
	switch(Track_Next_State)
	{
	case CRUISE:
		PrintPlease = 5;
		Track_Next_State = TS_cruise();
		break;
	case ALIGN_GREY:
		PrintPlease = 6;
		if(align(isGry(bright_l),isGry(bright_r))) 
			Track_Next_State = ALIGN_WHITE;
		else
			Track_Next_State = ALIGN_GREY;
		break;
	case ALIGN_WHITE:
		PrintPlease = 7;
		if(align(isWht(bright_l),isWht(bright_r))) 
			Track_Next_State = WAYPOINT;
		else
			Track_Next_State = ALIGN_WHITE;
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

extern "C" 
{

/**---------------------------------**/
/** Find SM Functions for now **/
/**--------------------------------**/

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
	TrackSM_state TS_cruise(void)
	{
		
		int LeftMot = 0;
		int RightMot = 0;
		
		if (isGry(bright_l) || isGry(bright_r))
		{
			GryCnt +=1;
			
			LeftMot = QUARTSPEED;
			RightMot = QUARTSPEED;
		}		
		else if (isBlk(bright_r))
		{
			GryCnt = 0;
			LMMult += .25;
			//RMMult -= .1;
			
			LeftMot = (int) LMMult*BASESPEED; // Left motor forward
			RightMot =(int)  -RMMult*HALFSPEED; // Right motor zero
		}
		else if (isBlk(bright_l))
		{	
			GryCnt = 0;
			//LMMult -= .1;
			RMMult += .25;

			LeftMot = (int) -LMMult*HALFSPEED; // Left motor zero
			RightMot = (int) RMMult*BASESPEED; // Right motor backwards
		}
		else
		{
			GryCnt = 0;
			RMMult = DFLT_SP_MULT;
			LMMult = DFLT_SP_MULT;
			
			LeftMot = (int) LMMult*BASESPEED; // Left motor zero
			RightMot = (int) RMMult*BASESPEED; // Right motor backwards
		}
		
		//State Transitions
		TrackSM_state ret = CRUISE;
		if (GryCnt > GRY_CNT_THRESH)
		{
			GryCnt = 0;
			ret = ALIGN_GREY;
		}
		else if (GryCnt > GRY_CNT_THRESH/2)
		{
			//Maybe stopping and looking is a good idea?
			//Perhaps once one passes a threshold of 2 ina row?
			LeftMot = -QUARTSPEED;
			RightMot = -QUARTSPEED;
		}
		
		//Check Motor Thresholds
		if(LeftMot > FULLSPEED)
			LeftMot = FULLSPEED;
		else if(LeftMot < -FULLSPEED)
			LeftMot = -FULLSPEED;
		if(RightMot > FULLSPEED)
			RightMot = FULLSPEED;
		else if(RightMot < -FULLSPEED)
			RightMot = -FULLSPEED;
		
			
		//Use Motors
		leftMotor.setPWM(LeftMot); // Left motor forward
		rightMotor.setPWM(RightMot); // Right motor forward
		clock.wait(MOTORTIMESTEP); // Perform for duration of .1 seconds

		return ret;
		
	}
} //End Extren "C"