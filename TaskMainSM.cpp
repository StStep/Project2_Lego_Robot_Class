#include <assert.h>
#include "TaskMainSM.h"
 
  /**External Event Transition Definitions**/
 
// Trigger States waiting for a touch event
void TaskMainSM::Touch(void)
{
    // given the Halt event, transition to a new state based upon 
    // the current state of the state machine
    BEGIN_TRANSITION_MAP                      				// - Current State -
	    TRANSITION_MAP_ENTRY (ST_START)  				// TESTING
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
	    TRANSITION_MAP_ENTRY (ST_START)  				// TESTING
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
		TRANSITION_MAP_ENTRY (ST_START)  				// TESTING
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
}

// SM attempts to find the line
void TaskMainSM::ST_Find()
{
}
 
// Follow the line
void TaskMainSM::ST_Track()
{
}
 
// Sit around and wait for touch event, at waypoint
void TaskMainSM::ST_Idle()
{
}

