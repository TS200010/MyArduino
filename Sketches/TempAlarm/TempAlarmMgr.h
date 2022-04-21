#ifndef TEMP_ALARM_MGR_H
#define TEMP_ALARM_MGR_H

#include "Globals.h"
#include "Task.h"
#include "TaskScheduler.h"
#include "TempProbe.h"
#include "TriColLED.h"
#include "LCDWnd.h"


class TempAlarmMgr : public TimedTask {
  public:
    TempAlarmMgr();  
    bool Start();
    void run( uint32_t now );   

  private:
    int        SDWriteCnt;  // To track SD versus LCD writes 
    TempProbe *ambTempProbe;
    TempProbe *tempProbes[ g_numSensors ];
    TriColLED *triColLEDs[ g_numLEDs ];
    CLCDWnd   *LCDWnds[ g_numSensors ];
};

#endif
