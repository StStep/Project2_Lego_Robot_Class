#include <assert.h>
#include "TaskMainSM.h"
 
  /**External Event Transition Definitions**/
 
// Trigger States waiting for a touch event
void TaskMainSM::Touch(LightData* lData)
{
    // given the Halt event, transition to a new state based upon 
    // the current state of the state machine
    BEGIN_TRANSITION_MAP                      			// - Current State -
		TRANSITION_MAP_ENTRY (ST_FIND)					// ST_Start
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)  	// ST_Find
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)     	// ST_Track
        TRANSITION_MAP_ENTRY (ST_TRACK)             	// ST_Idle
    END_TRANSITION_MAP(lData)
}
 
 //The general run call for SM
void TaskMainSM::Run(LightData* lData)
{
    // given the Halt event, transition to a new state based upon 
    // the current state of the state machine
    BEGIN_TRANSITION_MAP                      			// - Current State -
		TRANSITION_MAP_ENTRY (EVENT_IGNORED)  	// ST_Start
        TRANSITION_MAP_ENTRY (ST_FIND)  				// ST_Find
        TRANSITION_MAP_ENTRY (ST_TRACK)        		// ST_Track
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)     	// ST_Idle
    END_TRANSITION_MAP(lData)
}

//Go back to the Start state
void TaskMainSM::Reset(void)
{
    // given the Halt event, transition to a new state based upon 
    // the current state of the state machine
    BEGIN_TRANSITION_MAP                      		// - Current State -
		TRANSITION_MAP_ENTRY (ST_START)  			// ST_Start
		TRANSITION_MAP_ENTRY (ST_START)  			// ST_Find
		TRANSITION_MAP_ENTRY (ST_START)        	// ST_Track
		TRANSITION_MAP_ENTRY (ST_START)            // ST_Idle
    END_TRANSITION_MAP(NULL)
}
 
 /**State Definitions**/
 

// SM starts here
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
		Find_Next_State  = FS_ALIGN_BLACK;
		break;
	case FS_ALIGN_BLACK:
		PrintPlease = 2;
		if(align(isBlk(LeftLightSen),isBlk(RightLightSen),DFLT_SP_MULT)) 
			Find_Next_State = FS_ALIGN_WHITE;
		else
			Find_Next_State = FS_ALIGN_BLACK;
		break;
	case FS_ALIGN_WHITE:
		PrintPlease = 3;
		if(align(isWht(LeftLightSen),isWht(RightLightSen),DFLT_SP_MULT)) 
			Find_Next_State = FS_ROTATE_ALIGN;
		else
			Find_Next_State = FS_ALIGN_WHITE;
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

	//Sensor Input
	int LeftLightSen = lData->LeftLightSen; // Left light sensor
	int RightLightSen = lData->RightLightSen; // Right light sensor
	
	switch(Track_Next_State)
	{
	case TS_CRUISE:
		PrintPlease = 5;
		Track_Next_State = TS_cruise(LeftLightSen, RightLightSen);
		break;
	case TS_ALIGN_GREY:
		PrintPlease = 6;
		if(align(isGry(LeftLightSen),isGry(RightLightSen), DFLT_SP_MULT)) 
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
		if(align(isGry(LeftLightSen),isGry(RightLightSen), -DFLT_SP_MULT)) 
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
		if(align(isGry(LeftLightSen),isGry(RightLightSen), DFLT_SP_MULT)) 
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
		if(align(isGry(LeftLightSen),isGry(RightLightSen), -DFLT_SP_MULT)) 
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

/**----------------------------------**/
/** Track SM Functions for now **/
/**---------------------------------**/
TrackSM_state TS_cruise(int LeftLightSen, int RightLightSen)
{
	
	int LeftMot = 0;
	int RightMot = 0;
	
	if (isGry(LeftLightSen) || isGry(RightLightSen))
	{
		GryCnt +=1;
		
		LeftMot = HALFSPEED;
		RightMot = HALFSPEED;
	}		
	else if (isBlk(RightLightSen))
	{
		GryCnt = 0;
		LMMult += .25;
		//RMMult -= .1;
		
		LeftMot = (int) LMMult*BASESPEED; 
		RightMot =(int)  -RMMult*HALFSPEED; 
	}
	else if (isBlk(LeftLightSen))
	{	
		GryCnt = 0;
		//LMMult -= .1;
		RMMult += .25;

		LeftMot = (int) -LMMult*HALFSPEED;
		RightMot = (int) RMMult*BASESPEED;
	}
	else
	{
		GryCnt = 0;
		RMMult = DFLT_SP_MULT;
		LMMult = DFLT_SP_MULT;
		
		LeftMot = (int) LMMult*BASESPEED;
		RightMot = (int) RMMult*BASESPEED; 
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
		//Reverses a bit to make extra sure grey tape is detected
		LeftMot = -HALFSPEED;
		RightMot = -HALFSPEED;
	}
	
	//Use Motors
	MotorStep(LeftMot, RightMot, MOTORTIMESTEP);

	return ret;
	
}
} //End Extren C
