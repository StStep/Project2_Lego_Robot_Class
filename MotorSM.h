#ifndef MOTORSM_H
#define MOTORSM_H
#include "StateMachine.h"
 
// structure to hold event data passed into state machine
struct MotorData : public EventData
{
    int speed;
};
 
// the Motor state machine class
class MotorSM : public StateMachine
{
public:
    MotorSM() : StateMachine(ST_MAX_STATES) {}
 
    // external events taken by this state machine
    void Halt();
    void SetSpeed(MotorData*);
private:
    // state machine state functions
    void ST_Idle();
    void ST_Stop();
    void ST_Start(MotorData*);
    void ST_ChangeSpeed(MotorData*);
 
    // state map to define state function order
    BEGIN_STATE_MAP
        STATE_MAP_ENTRY(&MotorSM::ST_Idle)
        STATE_MAP_ENTRY(&MotorSM::ST_Stop)
        STATE_MAP_ENTRY(&MotorSM::ST_Start)
        STATE_MAP_ENTRY(&MotorSM::ST_ChangeSpeed)
    END_STATE_MAP
 
    // state enumeration order must match the order of state
    // method entries in the state map
    enum E_States { 
        ST_IDLE = 0,
        ST_STOP,
        ST_START,
        ST_CHANGE_SPEED,
        ST_MAX_STATES
    };
};
#endif //MOTORSM_H
