#include <assert.h>
#include "TaskMainSM.h"
 
  /**External Event Transition Definitions**/
TaskMainSM::TaskMainSM() : StateMachine(ST_MAX_STATES)
{
	 TrackStateSM_inst = new TrackStateSM(this);
}

// Trigger States waiting for a touch event
void TaskMainSM::Touch(LightData* lData)
{
    // given the Touch event, transition to a new state based upon 
    // the current state of the state machine
    BEGIN_TRANSITION_MAP                      	// - Current State -
		TRANSITION_MAP_ENTRY (ST_FIND)				// ST_Start
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)  		// ST_Find
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)     	// ST_Track
        TRANSITION_MAP_ENTRY (ST_TRACK)             // ST_Idle
    END_TRANSITION_MAP(lData)
}
 
 //The general run call for SM
void TaskMainSM::Run(LightData* lData)
{
    // given the Run event, transition to a new state based upon 
    // the current state of the state machine
    BEGIN_TRANSITION_MAP                      	// - Current State -
		TRANSITION_MAP_ENTRY (EVENT_IGNORED)  		// ST_Start
        TRANSITION_MAP_ENTRY (ST_FIND)  			// ST_Find
        TRANSITION_MAP_ENTRY (ST_TRACK)        		// ST_Track
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)     	// ST_Idle
    END_TRANSITION_MAP(lData)
}

//Go back to the Start state
void TaskMainSM::Reset(void)
{
    // given the Halt event, transition to a new state based upon 
    // the current state of the state machine
    BEGIN_TRANSITION_MAP                      	// - Current State -
		TRANSITION_MAP_ENTRY (ST_START)  			// ST_Start
		TRANSITION_MAP_ENTRY (ST_START)  			// ST_Find
		TRANSITION_MAP_ENTRY (ST_START)        		// ST_Track
		TRANSITION_MAP_ENTRY (ST_START)            	// ST_Idle
    END_TRANSITION_MAP(NULL)
}

// Goto the Idle State from Track
void TaskMainSM::GotoIdle()
{
    // given the Touch event, transition to a new state based upon 
    // the current state of the state machine
    BEGIN_TRANSITION_MAP                      	// - Current State -
		TRANSITION_MAP_ENTRY (ST_IDLE)				// ST_Start
        TRANSITION_MAP_ENTRY (ST_IDLE)  			// ST_Find
        TRANSITION_MAP_ENTRY (ST_IDLE)     			// ST_Track
        TRANSITION_MAP_ENTRY (ST_IDLE)        		// ST_Idle
    END_TRANSITION_MAP(NULL)
}
 
 /**State Definitions**/
 

// SM starts here, and does initilizaing thing? or nothing
void TaskMainSM::ST_Start() 
{
}

// SM attempts to find the line
void TaskMainSM::ST_Find(LightData* lData)
{
	//Sensor Input
	int LeftLightSen = lData->LeftLightSen; // Left light sensor
	int RightLightSen = lData->RightLightSen; // Right light sensor

	switch(Find_Next_State)
	{
	case FS_INIT:
		PrintPlease = 1;
		Find_Next_State  = FS_FWD_UNTIL_TAN;
		break;
	case FS_FWD_UNTIL_TAN:
		PrintPlease = 2;
		if(align(isBlk(LeftLightSen),isBlk(RightLightSen),DFLT_SP_MULT)) 
			Find_Next_State = FS_WHITE_ALIGN;
		else
			Find_Next_State = FS_FWD_UNTIL_TAN;
		break;
	case FS_WHITE_ALIGN:
		PrintPlease = 3;
		if(align(isWht(LeftLightSen),isWht(RightLightSen),DFLT_SP_MULT)) 
			Find_Next_State = FS_ROTATE_ALIGN;
		else
			Find_Next_State = FS_WHITE_ALIGN;
		break;
	case FS_ROTATE_ALIGN:
		PrintPlease = 4;
		Find_Next_State = FS_rotate_align(LeftLightSen, RightLightSen);
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
void TaskMainSM::ST_Track(LightData* lData)
{
	TrackStateSM_inst->Run(lData);
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
FindSM_state FS_rotate_align(int LeftLightSen, int RightLightSen)
{

	//Default return state
	FindSM_state ret =  FS_ROTATE_ALIGN;

	if(tape_flag == false)
	{ 
		if (isBlk(LeftLightSen)) //when black tape first seen by left
		{
			tape_flag = true;
		}
	}
	else if(tape_flag == true)
	{
		if (!(isBlk(LeftLightSen)) || isBlk(RightLightSen)) //when left no longer on black
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
} //End Extren C
