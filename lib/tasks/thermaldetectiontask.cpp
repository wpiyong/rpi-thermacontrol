/*
 * thermaldetectiontask.cpp
 *
 *  Created on: Mar 8, 2018
 *      Author: dev03
 */


#include <iostream>
#include "thermaldetectiontask.h"
#include "../devices/devds18b20.h"
#include "../threads/fancontrolthread.h"
#include <thread>

using namespace Dsr;

ThermalDetectionTask::ThermalDetectionTask(::std::vector<DevDs18B20 *> &devs, FanControlThread* aControlThread) : devDs18B20s(devs), controlThread(aControlThread) {
	stop = false;
}

void ThermalDetectionTask::run(){
    attemptThermalDetection();
}

void ThermalDetectionTask::attemptThermalDetection() {
    for(int i = 0; i < devDs18B20s.size(); i++) {
        Device* dev = devDs18B20s.at(i);
        int val = dev->read(dev->getPinNum());
        ::std::cout<<val<<" at pin: "<<dev->getPinNum()<<::std::endl;
        if(val > 0) {
        	val = (val + 5) / 10;
        	controlThread->enqueue(::std::make_pair(val, dev->getDeviceId()));
        }
    }
    ::std::this_thread::sleep_for(::std::chrono::seconds(1));
}

void ThermalDetectionTask::operator()(){
	while(!stop) {
		run();
	}
}

void ThermalDetectionTask::abort(){
	stop = true;
}
