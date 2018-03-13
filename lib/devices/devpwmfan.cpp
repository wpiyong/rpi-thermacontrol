/*
 * devpwmfan.cpp
 *
 *  Created on: Mar 8, 2018
 *      Author: dev03
 */


#include "devpwmfan.h"
#include <wiringPi.h>
#include <softPwm.h>

using namespace Dsr;

DevPwmFan::DevPwmFan(char* id, int pin) : deviceId(id), pinNum(pin), Device() {
    try{
        int res = init();
        if(res != 0) {
            throw -1;
        }
    } catch(int exc) {

    }
}

int DevPwmFan::getPinNum(){
    return pinNum;
}

char* DevPwmFan::getDeviceId(){
    return deviceId;
}

DevPwmFan::~DevPwmFan(){

}

int DevPwmFan::init(){
    pinMode (pinNum, PWM_OUTPUT) ;
    // Setting PWM frequency to be 10KHz with a full range of 1024 steps
    // PWM frequency = 19.2MHz / (divisor * range)
    // 10000 = 19200000 / (divisor * 1024) => divisor = 1.875 ~ 2;
    pwmSetMode(PWM_MODE_MS);
    pwmSetRange(1024);
    pwmSetClock(2);
    return 0;
}

void DevPwmFan::write(int pin, int value) {

    pwmWrite(pin, value);
//    delay(10);
//    pinMode(11, OUTPUT);
//    delay(10);
//    digitalWrite(11, 1);
}

int DevPwmFan::read(int pin) {
    return 0;
}

