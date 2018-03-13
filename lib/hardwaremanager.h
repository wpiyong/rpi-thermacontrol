/*
 * hardwaremanager.h
 *
 *  Created on: Mar 8, 2018
 *      Author: dev03
 */

#ifndef LIB_HARDWAREMANAGER_H_
#define LIB_HARDWAREMANAGER_H_

#include "devices/devds18b20.h"
#include "devices/devpwmfan.h"
#include <vector>
#include <thread>
#include "tasks/thermaldetectiontask.h"
namespace Dsr {

enum HardwareComponent {
    FAN,
    CUTOFF,
    LED,
    SHUTTER2,
    SHUTTER1,
    HEATER
};

class FanControlThread;

class HardwareManager {

private:
    bool fanEngaged = false;
    bool cutoffEngaged = false;
    bool ledEngaged = false;
    bool shutter2Engaged = false;
    bool shutter1Engaged = false;
    bool heaterEngaged = false;

    ::std::vector<DevDs18B20*> devDs18B20s;
    ::std::vector<DevPwmFan*> devPwmFans;

    int addDs18B20s();
    int addPwmFans();
    int PinForComponent(HardwareComponent);
private:
    explicit HardwareManager();
public:
    static HardwareManager* SharedInstance();
    ~HardwareManager();

    int RefreshHardware();

    bool GetComponentState(HardwareComponent);
    bool SetComponentState(HardwareComponent,bool);
    Device* findDeviceById(char* id);
    Device* findPwmFanByDs18B20Id(char* id);
    void TriggerCapture();

    FanControlThread* fanControlThread;
    ThermalDetectionTask* thermalTask;
    void startFanControlThread();
    void startThermalDetectionTask();

    void ThermalDetected(int value, char* devId);
    void ThermalDetectionFinished();
};

}



#endif /* LIB_HARDWAREMANAGER_H_ */
