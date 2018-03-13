/*
 * devds18b20.cpp
 *
 *  Created on: Mar 8, 2018
 *      Author: dev03
 */

#include <iostream>
#include "devds18b20.h"
#include <wiringPi.h>
#include <ds18b20.h>


using namespace Dsr;

DevDs18B20::DevDs18B20(char* id, int pin) : deviceId(id), pinNum(pin) {
    try{
        int res = this->init();
        if(res != 0) {
            char* msg = "Initial ds18b20 failed";
            throw -1;
        }
    } catch(int x) {
        //::std::cout<<"failed in devds18b20";
    }
}

int DevDs18B20::getPinNum(){
    return pinNum;
}

char* DevDs18B20::getDeviceId(){
    return deviceId;
}

DevDs18B20::~DevDs18B20(){

}

int DevDs18B20::init(){
    const char* device = this->deviceId;
    int ret = ds18b20Setup(this->pinNum, device);
    ::std::cout<<"result: "<<ret<<::std::endl;
    pinMode(this->pinNum, OUTPUT);
    return 0;
}

void DevDs18B20::write(int pin, int value) {

    return;
}

int DevDs18B20::read(int pin) {
    int res = analogRead(pin);
    return res;
}

