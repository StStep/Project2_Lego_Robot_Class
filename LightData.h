#ifndef LIGHT_DATA_H
#define LIGHT_DATA_H

#include "EventData.h"
 
// structure to hold event data passed into state machine
struct LightData : public EventData
{
    int LeftLightSen;
    int RightLightSen;
};
#endif //LIGHT_DATA_H 
