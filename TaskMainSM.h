#ifndef TASK_MAIN_SM_H
#define TASK_MAIN_SM_H

#include "StateMachine.h"
#include "LightData.h"
#include "MotorSuite.h"
#include "SensorSuite.h"
#include "LCDSuite.h"
#include "SecStateMachines.h"

class TrackStateSM;
class FindStateSM;
 
// the MainTask state machine class
class TaskMainSM : public StateMachine
{
public:
    TaskMainSM();
 
    // external events taken by this state machine
    void Touch(LightData*);
    void Run(LightData*);
	void Reset();
	void Next();
private:
    // state machine state functions
	void ST_Start();
	void ST_Find(LightData*);	
	void ST_Track(LightData*);
	void ST_Idle();
	
	//Internal StateMachines

 
    // state map to define state function order
    BEGIN_STATE_MAP
        STATE_MAP_ENTRY(&TaskMainSM::ST_Start)
		STATE_MAP_ENTRY(&TaskMainSM::ST_Find)
		STATE_MAP_ENTRY(&TaskMainSM::ST_Track)
		STATE_MAP_ENTRY(&TaskMainSM::ST_Idle)
    END_STATE_MAP
 
    // state enumeration order must match the order of state
    // method entries in the state map
    enum E_States { 
        ST_START = 0,
		ST_FIND,
		ST_TRACK,
		ST_IDLE,
        ST_MAX_STATES
    };
};
#endif //TASK_MAIN_SM_H
