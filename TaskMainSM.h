#ifndef TASK_MAIN_SM_H
#define TASK_MAIN_SM_H
#include "StateMachine.h"
#include "SensorSuite.h"
#include "MotorSuite.h"

//First Imp  of Find #defines

// Value for each motor to move full speed ahead
#define FULLSPEED 100
#define TIME 100

// values for colored tape
#define WHITEHIGH 600
#define WHITELOW 540
#define BLACKHIGH 385
#define BLACKLOW 325


//From Early Imp
extern "C" 
{
typedef enum{INIT, FIND, ALIGN, STEP, ROTATE_ALIGN, IDLE} state_t;
float baser = .3;
float basel = .3;

state_t state_init(void);
state_t state_find(void);
state_t state_align(void);
state_t state_step(void);
state_t state_rotate_align(void);
state_t state_idle(void);

state_t next_state;
int bright_l, bright_r;

}
 
// the MainTask state machine class
class TaskMainSM : public StateMachine
{
public:
    TaskMainSM() : StateMachine(ST_MAX_STATES) {}
 
    // external events taken by this state machine
    void Touch();
    void Run();
	void Reset();
private:
    // state machine state functions
	void ST_Start();
	void ST_Find();	
	void ST_Track();
	void ST_Idle();
 
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
