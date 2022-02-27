#ifndef TEMP_ALARM_MGR_H
#define TEMP_ALARM_MGR_H

#include "Globals.h"
#include "Task.h"
#include "TaskScheduler.h"
#include "TempProbe.h"
#include "TriColLED.h";

class TempAlarmMgr : public TimedTask {
  public:
    TempAlarmMgr();
    void run( uint32_t now );     
//  virtual bool canRun(uint32_t now); 
    bool Start();

  private:
    int        SDWriteCnt;  // To track SD versus LCD writes 
    int        numSensors;
    TempProbe *tempProbes[ g_numSensorsAndLEDs ];
    TriColLED *triColLEDs[ g_numSensorsAndLEDs ];
};

#endif
