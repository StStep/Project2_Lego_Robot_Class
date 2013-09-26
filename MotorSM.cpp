#include <assert.h>
#include "MotorSM.h"
 
// halt motor external event
void MotorSM::Halt(void)
{
    // given the Halt event, transition to a new state based upon 
    // the current state of the state machine
    BEGIN_TRANSITION_MAP                      // - Current State -
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)  // ST_Idle
        TRANSITION_MAP_ENTRY (CANNOT_HAPPEN)  // ST_Stop
        TRANSITION_MAP_ENTRY (ST_STOP)        // ST_Start
        TRANSITION_MAP_ENTRY (ST_STOP)        // ST_ChangeSpeed
    END_TRANSITION_MAP(NULL)
}
 
// set motor speed external event
void MotorSM::SetSpeed(MotorData* pData)
{
    BEGIN_TRANSITION_MAP                      // - Current State -
        TRANSITION_MAP_ENTRY (ST_START)       // ST_Idle       
        TRANSITION_MAP_ENTRY (CANNOT_HAPPEN)  // ST_Stop       
        TRANSITION_MAP_ENTRY (ST_CHANGE_SPEED)// ST_Start      
        TRANSITION_MAP_ENTRY (ST_CHANGE_SPEED)// ST_ChangeSpeed
    END_TRANSITION_MAP(pData)
}
 
// state machine sits here when motor is not running
void MotorSM::ST_Idle() 
{
}
 
// stop the motor 
void MotorSM::ST_Stop()
{
    // perform the stop motor processing here
    // transition to ST_Idle via an internal event
    InternalEvent(ST_IDLE);
}
 
// start the motor going
void MotorSM::ST_Start(MotorData* pData)
{
    // set initial motor speed processing here
}
 
// changes the motor speed once the motor is moving
void MotorSM::ST_ChangeSpeed(MotorData* pData)
{
    // perform the change motor speed to pData->speed here
}
