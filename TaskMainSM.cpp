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
	case FS_INIT:
		PrintPlease = 1;
		Find_Next_State  = FS_FWD_UNTIL_TAN;
		break;
	case FS_FWD_UNTIL_TAN:
		PrintPlease = 2;
		if(align(isBlk(bright_l),isBlk(bright_r),DFLT_SP_MULT)) 
			Find_Next_State = FS_WHITE_ALIGN;
		else
			Find_Next_State = FS_FWD_UNTIL_TAN;
		break;
	case FS_WHITE_ALIGN:
		PrintPlease = 3;
		if(align(isWht(bright_l),isWht(bright_r),DFLT_SP_MULT)) 
			Find_Next_State = FS_ROTATE_ALIGN;
		else
			Find_Next_State = FS_WHITE_ALIGN;
		break;
	case FS_ROTATE_ALIGN:
		PrintPlease = 4;
		Find_Next_State = FS_rotate_align();
		break;
	default:
		Find_Next_State = FS_INIT;
		PrintPlease = 0;
		MotorStep(NOSPEED, NOSPEED, 0);
		InternalEvent(ST_TRACK);
		break;
	}

}
 
// Follow the line
void TaskMainSM::ST_Track()
{

	//Sensor Input
	bright_l = leftLight.getBrightness(); // Left light sensor
	bright_r = rightLight.getBrightness(); // Right light sensor
	
	switch(Track_Next_State)
	{
	case TS_CRUISE:
		PrintPlease = 5;
		Track_Next_State = TS_cruise();
		break;
	case TS_ALIGN_GREY:
		PrintPlease = 6;
		if(align(isGry(bright_l),isGry(bright_r), DFLT_SP_MULT)) 
			Track_Next_State = TS_STEP;
		else
			Track_Next_State = TS_ALIGN_GREY;
		break;
	case TS_STEP:
		PrintPlease = 7;
		MotorStep(BASESPEED, BASESPEED, MOTORTIMESTEP*5);		
		Track_Next_State =TS_ALIGN_GREY_RV;
		break;
	case TS_ALIGN_GREY_RV:
		PrintPlease = 8;
		if(align(isGry(bright_l),isGry(bright_r), -DFLT_SP_MULT)) 
			Track_Next_State = TS_STEP2;
		else
			Track_Next_State = TS_ALIGN_GREY_RV;
		break;
	case TS_STEP2:
		PrintPlease = 7;
		MotorStep(BASESPEED, BASESPEED, MOTORTIMESTEP*10);
		Track_Next_State =TS_ALIGN_GREY2;
		break;
	case TS_ALIGN_GREY2:
		PrintPlease = 6;
		if(align(isGry(bright_l),isGry(bright_r), DFLT_SP_MULT)) 
			Track_Next_State = TS_STEP3;
		else
			Track_Next_State = TS_ALIGN_GREY2;
		break;
	case TS_STEP3:
		PrintPlease = 7;		
		MotorStep(BASESPEED, BASESPEED, MOTORTIMESTEP*5);
		Track_Next_State =TS_ALIGN_GREY_RV2;
		break;
	case TS_ALIGN_GREY_RV2:
		PrintPlease = 8;
		if(align(isGry(bright_l),isGry(bright_r), -DFLT_SP_MULT)) 
			Track_Next_State = TS_WAYPOINT;
		else
			Track_Next_State = TS_ALIGN_GREY_RV2;
		break;
	case TS_WAYPOINT:
		PrintPlease = 0;
		Track_Next_State = TS_STEP4;
		
		MotorStep(NOSPEED, NOSPEED, 0);
		
		InternalEvent(ST_IDLE);
		break;
	case TS_STEP4:
		PrintPlease = 7;
		MotorStep(BASESPEED, BASESPEED, MOTORTIMESTEP*5);
		Track_Next_State = TS_CRUISE;
		break;
	default:
		Track_Next_State =TS_CRUISE;
		break;
	}

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

bool align(bool isLeftTrue, bool isRightTrue, float Mult)
{
	//Defualt return state
	bool ret =  false;

	//Both on line, change states
	if(isLeftTrue && isRightTrue)
	{
		MotorStep(NOSPEED, NOSPEED, 0);
		ret = true;
	}
	else if(isRightTrue) 
	{
		MotorStep((int) Mult*BASESPEED, -(int) Mult*HALFSPEED, MOTORTIMESTEP);
	}
	else if(isLeftTrue) 
	{
		MotorStep(-(int) Mult*HALFSPEED, (int) Mult*BASESPEED, MOTORTIMESTEP);
	}
	else //while neither of the sensors have detected black tape, move forward
	{
		MotorStep((int) Mult*BASESPEED, (int) Mult*BASESPEED, MOTORTIMESTEP);
	}

	return ret;

}

FindSM_state FS_rotate_align(void)
{

	//Default return state
	FindSM_state ret =  FS_ROTATE_ALIGN;

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
			ret = FS_IDLE;
		}
	}

	if (ret == FS_ROTATE_ALIGN) 
	{
		MotorStep(-BASESPEED, BASESPEED, MOTORTIMESTEP);
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
		
		LeftMot = HALFSPEED;//QUARTSPEED;
		RightMot = HALFSPEED;//QUARTSPEED;
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
	TrackSM_state ret = TS_CRUISE;
	if (GryCnt > GRY_CNT_THRESH)
	{
		GryCnt = 0;
		ret = TS_ALIGN_GREY;
	}
	else if (GryCnt > GRY_CNT_THRESH/2)
	{
		//Maybe stopping and looking is a good idea?
		//Perhaps once one passes a threshold of 2 ina row?
		LeftMot = -HALFSPEED;//QUARTSPEED;
		RightMot = -HALFSPEED;//QUARTSPEED;
	}
	
	//Use Motors
	MotorStep(LeftMot, RightMot, MOTORTIMESTEP);

	return ret;
	
}
} //End Extren C
