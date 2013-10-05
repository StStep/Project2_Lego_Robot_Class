#ifndef FIND_STATE_SM_H
#define FIND_STATE_SM_H

#include "StateMachine.h"
#include "LightData.h"
#include "MotorSuite.h"
#include "SensorSuite.h"
#include "LCDSuite.h"
 
// the MainTask state machine class
class FindStateSM : public StateMachine
{
public:
    FindStateSM();
 
    // external events taken by this state machine
    void Run(LightData*);
	void Reset();
	
private:

    // state machine state functions
	void ST_Cruise(LightData*);
	void ST_Align_Grey(LightData*);
	void ST_Step(void);
	void ST_Align_Grey_RV(LightData*);
	void ST_Doub_Step(void);
	void ST_Waypoint(LightData*);
	
	//Variables
	bool tape_flag;
	
	/**Find State Declarations**/
	void ST_Fwd_Until_Tan(LightData*);
	void ST_Align_White(LightData*);
	void ST_Rotate_Align(LightData*);
	void ST_Start_Track();

    // state map to define state function order
    BEGIN_STATE_MAP
        STATE_MAP_ENTRY(&FindStateSM::ST_Fwd_Until_Tan)
		STATE_MAP_ENTRY(&FindStateSM::ST_Align_White)
		STATE_MAP_ENTRY(&FindStateSM::ST_Rotate_Align)
		STATE_MAP_ENTRY(&FindStateSM::ST_Start_Track)
    END_STATE_MAP
 
    // state enumeration order must match the order of state
    // method entries in the state map
    enum E_States { 
        ST_FWD_UNTIL_TAN = 0,
		ST_ALIGN_WHITE,
		ST_ROTATE_ALIGN,
		ST_START_TRACK,
        ST_MAX_STATES
    };
};
#endif //FIND_STATE_SM_H
