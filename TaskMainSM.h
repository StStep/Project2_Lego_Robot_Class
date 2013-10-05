#ifndef TASK_MAIN_SM_H
#define TASK_MAIN_SM_H

#include "StateMachine.h"
#include "TrackStateSM.h"
#include "MotorSuite.h"
#include "SensorSuite.h"
#include "LCDSuite.h"


extern "C" 
{

// structure to hold event data passed into state machine
struct LightData : public EventData
{
    int LeftLightSen;
    int RightLightSen;
};

/**Find State Declarations**/
typedef enum{FS_INIT, FS_FWD_UNTIL_TAN, FS_WHITE_ALIGN, FS_ROTATE_ALIGN, FS_IDLE} FindSM_state;
//Find Function
FindSM_state FS_rotate_align(int LeftLightSen, int RightLightSen);
//Silly globasl, fix later
FindSM_state Find_Next_State;
bool tape_flag = 0;

}
 
 //class TaskStateSM;
 
// the MainTask state machine class
class TaskMainSM : public StateMachine
{
public:
    TaskMainSM();
 
    // external events taken by this state machine
    void Touch(LightData*);
    void Run(LightData*);
	void Reset();
	void GotoIdle();
private:
    // state machine state functions
	void ST_Start();
	void ST_Find(LightData*);	
	void ST_Track(LightData*);
	void ST_Idle();
	
	//Internal StateMachines
 	TrackStateSM *TrackStateSM_inst;
 
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
