
#include "TaskMain.h"
#include "MotorSuite.h"
#include "LCDSuite.h"
#include "BTSuite.h"
#include <math.h>
using namespace ecrobot;
extern "C" 
{

//Calc Variables
float ThetaPast = 0.0;
float xPast = 0.0;
float yPast = 0.0;
int rMotOld = 0;
int lMotOld = 0;


/**
This task runs once every 500ms, I don't know if that should be the target time or not yet
**/
TASK(TaskRecord)
{
	//DEAD RECKONING ODOMETRY ALGORITHM
	int lMotNew = GetLeftMotCnt();
	int rMotNew = GetRightMotCnt();
	
	float dr = ((float)(rMotNew - rMotOld))*WHEEL_MULT;
    float dl = ((float)(lMotNew - lMotOld))*WHEEL_MULT;
    float ThetaNew = ThetaPast + ((dr-dl)/BODY_WIDTH);
    float DistNew = (dr+dl)/2.0;
    float xNew = xPast + DistNew*cos(ThetaNew);   
    float yNew = yPast + DistNew*sin(ThetaNew);   
	
	ThetaPast = ThetaNew;
    xPast = xNew;
    yPast = yNew;
	
	rMotOld = rMotNew;
	lMotOld = lMotNew;
	
	if(BTConnected) 
	{
		dataS08[0]= xNew*100;
		dataS08[1]= yNew*100;
		dataS32++;
		daq.send(dataS08, dataS32);
	}
	
	TerminateTask();
} // End Task
} //End Extern C
