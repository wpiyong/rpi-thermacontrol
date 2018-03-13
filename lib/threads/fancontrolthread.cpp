/*
 * fancontrolthread.cpp
 *
 *  Created on: Mar 8, 2018
 *      Author: dev03
 */


#include <iostream>
#include "fancontrolthread.h"
#include <wiringPi.h>
#include "../hardwaremanager.h"

using namespace Dsr;

FanControlThread::FanControlThread(HardwareManager* hdManager) : piManager(hdManager),condVar(), mutex(), stop(false), dataQueue(), _thread(&FanControlThread::run, this) {

}

FanControlThread::~FanControlThread(){
	stop = true;
	_thread.join();
}

bool FanControlThread::isQueueEmpty(){
	::std::unique_lock<::std::mutex> lock(mutex);
    if(dataQueue.size()>0){
    	return false;
    }
    return true;
}

void FanControlThread::enqueue(::std::pair<int, char*> data){
    {
    	::std::unique_lock<::std::mutex> lock(mutex);
        dataQueue.push(data);
    }

    condVar.notify_one();
}

void FanControlThread::run(){
    while(true){
        if(stop) {

            return;
        }
        ::std::unique_lock<::std::mutex> lock(mutex);
        condVar.wait(lock, [&]{ return !dataQueue.empty()>0 || stop;});

        ::std::pair<int, char*> pair = dataQueue.front();

        int ret = process(pair.first, pair.second);
        dataQueue.pop();
    }
}

void FanControlThread::abort(){
    stop = true;
    condVar.notify_one();
}

int FanControlThread::process(int temp, char* id){
    Device* pwmDev = piManager->findPwmFanByDs18B20Id(id);

    int value = -1;
    if(temp < 25) {
    	pwmDev->write(pwmDev->getPinNum(), 1000);
    	delay(100);
        value = 1023 * 20 / 100;
        ::std::cout<<"fan speed: low"<<::std::endl;
    } else if(temp < 26) {
    	pwmDev->write(pwmDev->getPinNum(), 1000);
    	delay(100);
        value = 1023 * 50 / 100;
        ::std::cout<<"fan speed: middle"<<::std::endl;
    } else if(temp < 27) {
        value = 1023 * 70 / 100;
        ::std::cout<<"fan speed: middle-high"<<::std::endl;
    } else {
        value = 1023 * 90 / 100;
        ::std::cout<<"fan speed: high"<<::std::endl;
    }

    pwmDev->write(pwmDev->getPinNum(), value);
    return 0;
}
