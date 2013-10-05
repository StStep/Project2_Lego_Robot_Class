#include <assert.h>
#include "FindStateSM.h"
 
/**External Event Transition Definitions**/
 
FindStateSM::FindStateSM() : StateMachine(ST_MAX_STATES)
{	
	//Var Init
	tape_flag = 0;	
}

//The general run call for SM
void FindStateSM::Run(LightData* lData)
{
    // given the Run event, transition to a new state based upon 
    // the current state of the state machine
    BEGIN_TRANSITION_MAP                      					// - Current State -
		TRANSITION_MAP_ENTRY (ST_FWD_UNTIL_TAN)  		// ST_Fwd_Until_Tan
        TRANSITION_MAP_ENTRY (ST_ALIGN_WHITE)  			// ST_Align_White
        TRANSITION_MAP_ENTRY (ST_ROTATE_ALIGN)        		// ST_Rotate_Align
        TRANSITION_MAP_ENTRY (CANNOT_HAPPEN)    			// ST_Start_Track
    END_TRANSITION_MAP(lData)
}

//Go back to the Start state
void FindStateSM::Reset(void)
{
    // given the Halt event, transition to a new state based upon 
    // the current state of the state machine
    BEGIN_TRANSITION_MAP                      					// - Current State -
		TRANSITION_MAP_ENTRY (ST_FWD_UNTIL_TAN)  			// ST_Fwd_Until_TanST_Cruise
        TRANSITION_MAP_ENTRY (ST_FWD_UNTIL_TAN)  			// ST_Align_WhiteST_Align_Grey
        TRANSITION_MAP_ENTRY (ST_FWD_UNTIL_TAN) 			// ST_Rotate_Align/ ST_Step
        TRANSITION_MAP_ENTRY (CANNOT_HAPPEN)    				// ST_Start_Track
    END_TRANSITION_MAP(NULL)
}
 
 /**State Definitions**/
void FindStateSM::ST_Fwd_Until_Tan(LightData* lData)
{
	//Sensor Input
	int LeftLightSen = lData->LeftLightSen; // Left light sensor
	int RightLightSen = lData->RightLightSen; // Right light sensor
	
	if(align(isBlk(LeftLightSen),isBlk(RightLightSen),DFLT_SP_MULT)) 
		InternalEvent(ST_ALIGN_WHITE);
}

void FindStateSM::ST_Align_White(LightData* lData)
{
	//Sensor Input
	int LeftLightSen = lData->LeftLightSen; // Left light sensor
	int RightLightSen = lData->RightLightSen; // Right light sensor
	
	if(align(isWht(LeftLightSen),isWht(RightLightSen),DFLT_SP_MULT)) 
		InternalEvent(ST_ROTATE_ALIGN);
}

void FindStateSM::ST_Rotate_Align(LightData* lData)
{
	bool done = false;
	
	//Sensor Input
	int LeftLightSen = lData->LeftLightSen; // Left light sensor
	int RightLightSen = lData->RightLightSen; // Right light sensor
	
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
			done = true;
			InternalEvent(ST_START_TRACK);
		}
	}

	if (done == false) 
	{
		MotorStep(-BASESPEED, BASESPEED, MOTORTIMESTEP);
	}
}

void FindStateSM::ST_Start_Track()
{
	Next = true;
}
 

