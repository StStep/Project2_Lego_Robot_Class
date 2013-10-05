#include <assert.h>
#include "TaskMainSM.h"
 
  /**External Event Transition Definitions**/
TaskMainSM::TaskMainSM() : StateMachine(ST_MAX_STATES)
{
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

// Goto the Next State
void TaskMainSM::Next()
{
    BEGIN_TRANSITION_MAP                      	// - Current State -
		TRANSITION_MAP_ENTRY (EVENT_IGNORED)	// ST_Start
        TRANSITION_MAP_ENTRY (ST_TRACK)  			// ST_Find
        TRANSITION_MAP_ENTRY (ST_IDLE)     			// ST_Track
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)  // ST_Idle
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
	FindStateSM_inst.Run(lData);
}
 
// Follow the line
void TaskMainSM::ST_Track(LightData* lData)
{
	TrackStateSM_inst.Run(lData);
}
 
// Sit around and wait for touch event, at waypoint
void TaskMainSM::ST_Idle()
{
}
