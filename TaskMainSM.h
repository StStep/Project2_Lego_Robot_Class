#ifndef TASK_MAIN_SM_H
#define TASK_MAIN_SM_H

#include "StateMachine.h"
#include "MotorSuite.h"
#include "SensorSuite.h"
#include "LCDSuite.h"

// structure to hold event data passed into state machine
struct LightData : public EventData
{
    int LeftLightSen;
    int RightLightSen;
};

//State Enumerations
//Find States
typedef enum{FS_INIT, FS_ALIGN_BLACK, FS_ALIGN_WHITE, FS_ROTATE_ALIGN, FS_IDLE} FindSM_state;
//Track States
typedef enum{TS_CRUISE, TS_ALIGN_GREY, TS_STEP, TS_ALIGN_GREY_RV, TS_DOUB_STEP,  TS_WAYPOINT} TrackSM_state;
 
// the MainTask state machine class
class TaskMainSM : public StateMachine
{
public:
    TaskMainSM() : StateMachine(ST_MAX_STATES) 
	{
		tape_flag = 0;
		RMMult = 1.00;
		LMMult = 1.00;
		GryTapeCnt = 0;
		GryCnt = 0;
	}
 
    // external events taken by this state machine
    void Touch(LightData*);
    void Run(LightData*);
	void Reset();
private:
    // state machine state functions
	void ST_Start();
	void ST_Find(LightData*);	
	void ST_Track(LightData*);
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
	
//Internal SM Resources
	
/**Find State Declarations**/
	FindSM_state Find_Next_State;
	bool tape_flag;

	//Function
	FindSM_state FS_rotate_align(int LeftLightSen, int RightLightSen);

/** Track State Declarations **/
	TrackSM_state Track_Next_State;
	float RMMult;
	float LMMult;
	int GryCnt;
	int GryTapeCnt;

	//Function
	TrackSM_state TS_cruise(int LeftLightSen, int RightLightSen);

};
#endif //TASK_MAIN_SM_H