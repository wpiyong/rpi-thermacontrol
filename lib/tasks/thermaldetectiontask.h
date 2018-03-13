/*
 * thermaldetectiontask.h
 *
 *  Created on: Mar 8, 2018
 *      Author: dev03
 */

#ifndef LIB_TASKS_THERMALDETECTIONTASK_H_
#define LIB_TASKS_THERMALDETECTIONTASK_H_


#include <vector>

namespace Dsr {

class DevDs18B20;
class FanControlThread;

class ThermalDetectionTask{

private:
    const ::std::vector<DevDs18B20*>& devDs18B20s;
    FanControlThread* controlThread;
    void attemptThermalDetection();
    bool stop;
public:
    explicit ThermalDetectionTask(::std::vector<DevDs18B20*>& devs, FanControlThread* aControlThread);
public:
    void operator()();
    void run();

    void abort();
    void ThermalDetected(int value, char* devId);
    void ThermalDetectionFinished();
};

}



#endif /* LIB_TASKS_THERMALDETECTIONTASK_H_ */
