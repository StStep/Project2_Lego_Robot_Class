#ifndef TRACK_STATE_SM_H
#define TRACK_STATE_SM_H

#include "StateMachine.h"
#include "LightData.h"
#include "MotorSuite.h"
#include "SensorSuite.h"
#include "LCDSuite.h"
  
// the MainTask state machine class
class TrackStateSM : public StateMachine
{
public:
    TrackStateSM();
 
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
	float RMMult;
	float LMMult ;
	int GryCnt;
	int GryTapeCnt;
 
    // state map to define state function order
    BEGIN_STATE_MAP
        STATE_MAP_ENTRY(&TrackStateSM::ST_Cruise)
		STATE_MAP_ENTRY(&TrackStateSM::ST_Align_Grey)
		STATE_MAP_ENTRY(&TrackStateSM::ST_Step)
		STATE_MAP_ENTRY(&TrackStateSM::ST_Align_Grey_RV)
		STATE_MAP_ENTRY(&TrackStateSM::ST_Doub_Step)
		STATE_MAP_ENTRY(&TrackStateSM::ST_Waypoint)
    END_STATE_MAP
 
    // state enumeration order must match the order of state
    // method entries in the state map
    enum E_States { 
        ST_CRUISE = 0,
		ST_ALIGN_GREY,
		ST_STEP,
		ST_ALIGN_GREY_RV,
		ST_DOUB_STEP,
		ST_WAYPOINT,
        ST_MAX_STATES
    };
};
#endif //TRACK_STATE_SM_H
