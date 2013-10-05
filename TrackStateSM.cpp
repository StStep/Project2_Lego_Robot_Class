#include <assert.h>
#include "TrackStateSM.h"
 
/**External Event Transition Definitions**/
 
TrackStateSM::TrackStateSM() : StateMachine(ST_MAX_STATES)
{
	//Var Init
	RMMult = 1.0;
	LMMult = 1.0;
	GryCnt = 0;
	GryTapeCnt = 0;
	
}

//The general run call for SM
void TrackStateSM::Run(LightData* lData)
{
    // given the Run event, transition to a new state based upon 
    // the current state of the state machine
    BEGIN_TRANSITION_MAP                      	// - Current State -
		TRANSITION_MAP_ENTRY (ST_CRUISE)  			// ST_Cruise
        TRANSITION_MAP_ENTRY (ST_ALIGN_GREY)  		// ST_Align_Grey
        TRANSITION_MAP_ENTRY (CANNOT_HAPPEN)        // ST_Step
        TRANSITION_MAP_ENTRY (ST_ALIGN_GREY_RV)     // ST_Align_Grey_RV
        TRANSITION_MAP_ENTRY (CANNOT_HAPPEN)     	// ST_Doub_Step
        TRANSITION_MAP_ENTRY (ST_STEP)     			// ST_Waypoint
    END_TRANSITION_MAP(lData)
}

//Go back to the Start state
void TrackStateSM::Reset(void)
{
    // given the Halt event, transition to a new state based upon 
    // the current state of the state machine
    BEGIN_TRANSITION_MAP                      	// - Current State -
		TRANSITION_MAP_ENTRY (ST_CRUISE)  			// ST_Cruise
        TRANSITION_MAP_ENTRY (ST_CRUISE)  			// ST_Align_Grey
        TRANSITION_MAP_ENTRY (CANNOT_HAPPEN)        // ST_Step
        TRANSITION_MAP_ENTRY (ST_CRUISE)    		// ST_Align_Grey_RV
        TRANSITION_MAP_ENTRY (CANNOT_HAPPEN)     	// ST_Doub_Step
        TRANSITION_MAP_ENTRY (ST_CRUISE)     		// ST_Waypoint
    END_TRANSITION_MAP(NULL)
}
 
 /**State Definitions**/
void TrackStateSM::ST_Cruise(LightData* lData)
{
	//Tape Count
	GryTapeCnt = 0;
	
	//Sensor Input
	int LeftLightSen = lData->LeftLightSen; // Left light sensor
	int RightLightSen = lData->RightLightSen; // Right light sensor
	
	int LeftMot = 0;
	int RightMot = 0;
	
	if (isGry(LeftLightSen) || isGry(RightLightSen))
	{
		GryCnt +=1;
		
		LeftMot = HALFSPEED;//QUARTSPEED;
		RightMot = HALFSPEED;//QUARTSPEED;
	}		
	else if (isBlk(RightLightSen))
	{
		GryCnt = 0;
		LMMult += .25;
		//RMMult -= .1;
		
		LeftMot = (int) LMMult*BASESPEED; // Left motor forward
		RightMot =(int)  -RMMult*HALFSPEED; // Right motor zero
	}
	else if (isBlk(LeftLightSen))
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
	if (GryCnt > GRY_CNT_THRESH)
	{
		GryCnt = 0;
		InternalEvent(ST_ALIGN_GREY);
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
}

void TrackStateSM::ST_Align_Grey(LightData* lData)
{
	//Grey Tape Count
	GryTapeCnt++;
	
	//Sensor Input
	int LeftLightSen = lData->LeftLightSen; // Left light sensor
	int RightLightSen = lData->RightLightSen; // Right light sensor
	
	if(align(isGry(LeftLightSen),isGry(RightLightSen), DFLT_SP_MULT)) 
		InternalEvent(ST_STEP);
}

void TrackStateSM::ST_Step(void)
{
		MotorStep(BASESPEED, BASESPEED, MOTORTIMESTEP*5);
		if(GryTapeCnt< 3)	
			InternalEvent(ST_ALIGN_GREY_RV);
		else
			InternalEvent(ST_CRUISE);
}

void TrackStateSM::ST_Align_Grey_RV(LightData* lData)
{
	//Sensor Input
	int LeftLightSen = lData->LeftLightSen; // Left light sensor
	int RightLightSen = lData->RightLightSen; // Right light sensor
	
	bool Aligned = align(isGry(LeftLightSen),isGry(RightLightSen), DFLT_SP_MULT);
	
	if(Aligned && (GryTapeCnt< 2)) 
		InternalEvent(ST_DOUB_STEP);
	else if(Aligned) 
		InternalEvent(ST_WAYPOINT);
}

void TrackStateSM::ST_Doub_Step(void)
{
	MotorStep(BASESPEED, BASESPEED, MOTORTIMESTEP*10);	
	InternalEvent(ST_ALIGN_GREY);
}

void TrackStateSM::ST_Waypoint(LightData* lData)
{
	//Grey Tape Count
	GryTapeCnt++;
	
	Next = true;
}

