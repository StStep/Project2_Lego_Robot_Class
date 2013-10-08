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
		Debug_PrintPlease = 1;
		Find_Next_State  = FS_ALIGN_BLACK;
		break;
	case FS_ALIGN_BLACK:
		Debug_PrintPlease = 2;
		if(align(isBlk(LeftLightSen),isBlk(RightLightSen),DFLT_SP_MULT)) 
			Find_Next_State = FS_ALIGN_WHITE;
		else
			Find_Next_State = FS_ALIGN_BLACK;
		break;
	case FS_ALIGN_WHITE:
		Debug_PrintPlease = 3;
		if(align(isWht(LeftLightSen),isWht(RightLightSen),DFLT_SP_MULT)) 
			Find_Next_State = FS_ROTATE_ALIGN;
		else
			Find_Next_State = FS_ALIGN_WHITE;
		break;
	case FS_ROTATE_ALIGN:
		Debug_PrintPlease = 4;
		Find_Next_State = FS_rotate_align(LeftLightSen, RightLightSen);
		break;
	default:
		Find_Next_State = FS_INIT;
		Debug_PrintPlease = 0;
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
		Debug_PrintPlease = 5;
		Track_Next_State = TS_cruise(LeftLightSen, RightLightSen);
		GryTapeCnt = 0;
		break;
	case TS_ALIGN_GREY:
		Debug_PrintPlease = 6;
		if(align(isGry(LeftLightSen),isGry(RightLightSen), DFLT_SP_MULT)) 
		{
			Track_Next_State = TS_STEP;
			GryTapeCnt++;
		}
		else
			Track_Next_State = TS_ALIGN_GREY;
		break;
	case TS_STEP:
		Debug_PrintPlease = 7;
		MotorStep(BASESPEED, BASESPEED, MOTORTIMESTEP*5);
		if(GryTapeCnt < 3)
			Track_Next_State =TS_ALIGN_GREY_RV;
		else 
			Track_Next_State =TS_CRUISE;
		break;
	case TS_ALIGN_GREY_RV:
		Debug_PrintPlease = 8;
		if(align(isGry(LeftLightSen),isGry(RightLightSen), -DFLT_SP_MULT))
		{
			if(GryTapeCnt < 2)
				Track_Next_State = TS_DOUB_STEP;
			else
				Track_Next_State = TS_WAYPOINT;
		}
		else
			Track_Next_State = TS_ALIGN_GREY_RV;
		break;
	case TS_DOUB_STEP:
		Debug_PrintPlease = 9;
		MotorStep(BASESPEED, BASESPEED, MOTORTIMESTEP*10);
		Track_Next_State =TS_ALIGN_GREY;
		break;
	case TS_WAYPOINT:
		Debug_PrintPlease = 10;
		MotorStep(NOSPEED, NOSPEED, 0);
		
		Track_Next_State = TS_STEP;	
		GryTapeCnt++;
		
		InternalEvent(ST_IDLE);
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

/**---------------------------------**/
/** Find SM Functions for now **/
/**--------------------------------**/
FindSM_state TaskMainSM::FS_rotate_align(int LeftLightSen, int RightLightSen)
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
TrackSM_state TaskMainSM::TS_cruise(int LeftLightSen, int RightLightSen)
{
	
	int LeftMot = 0;
	int RightMot = 0;
		
	if (isBlk(RightLightSen))
	{
		LMMult += .25;
		//RMMult -= .1;
		
		LeftMot = (int) LMMult*BASESPEED; 
		RightMot =(int)  -RMMult*HALFSPEED; 
	}
	else if (isBlk(LeftLightSen))
	{	
		//LMMult -= .1;
		RMMult += .25;

		LeftMot = (int) -LMMult*HALFSPEED;
		RightMot = (int) RMMult*BASESPEED;
	}
	else
	{

		RMMult = DFLT_SP_MULT;
		LMMult = DFLT_SP_MULT;
		
		LeftMot = (int) LMMult*BASESPEED;
		RightMot = (int) RMMult*BASESPEED; 
	}
	
	if (isGry(LeftLightSen) || isGry(RightLightSen))
		GryCnt +=1;
	else
		GryCnt = 0;
	
	//State Transitions
	TrackSM_state ret = TS_CRUISE;
	if (GryCnt > GRY_CNT_THRESH)
	{
		GryCnt = 0;
		ret = TS_ALIGN_GREY;
	}
	else if (GryCnt > GRY_CNT_THRESH_3QUART)
	{
		//Reverses a bit to make extra sure grey tape is detected
		LeftMot = -HALFSPEED;
		RightMot = HALFSPEED;
	}
	else if (GryCnt > GRY_CNT_THRESH_HALF)
	{
		//Reverses a bit to make extra sure grey tape is detected
		LeftMot = -HALFSPEED;
		RightMot = -HALFSPEED;
	}
	else if (GryCnt > GRY_CNT_THRESH_QUART)
	{
		//Reverses a bit to make extra sure grey tape is detected
		LeftMot = HALFSPEED;
		RightMot = HALFSPEED;
	}
	
	//Use Motors
	MotorStep(LeftMot, RightMot, MOTORTIMESTEP);

	return ret;
	
}
