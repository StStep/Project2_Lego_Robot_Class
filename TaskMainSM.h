#ifndef TASK_MAIN_SM_H
#define TASK_MAIN_SM_H

#include "StateMachine.h"
#include "SensorSuite.h"
#include "MotorSuite.h"
#include "LCDSuite.h"

//From Early Imp
extern "C" 
{
<<<<<<< HEAD

/**Find State Declarations**/
typedef enum{INIT, FWD_UNTIL_TAN, STEP, ROTATE_ALIGN, IDLE} FindSM_states;
bool align(bool isLeftTrue, bool isRightTrue);
FindSM_states state_rotate_align(void);
//Silly globasl, fix later
FindSM_states next_state;
=======
typedef enum{INIT, FWD_UNTIL_TAN, STEP, ROTATE_ALIGN, IDLE} state_t;
typedef enum{CRUISE, CORNERTURNLEFT, CORNERTURNRIGHT, ALIGNGREY, WAYPOINT} state_tt;

//find functions
state_t state_init(void);
state_t state_fwd_until_tan(void);
// state_t state_find(void);
// state_t state_align(void);
state_t state_step(void);
state_t state_rotate_align(void);
state_t state_idle(void);


float rfix = 1.0;
float lfix = 1.0;
//track functions
state_tt state_cruise(void);
state_tt state_corner_turn_left(void);
state_tt state_corner_turn_right(void);
state_tt state_align_grey(void);
state_tt state_waypoint(void);

//Silly globals, fix later
state_t next_state;
state_tt next_state_tt;

>>>>>>> Initial Track and Reach Waypoint(Gray)
int bright_l, bright_r;
bool tape_flag = 0;

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
