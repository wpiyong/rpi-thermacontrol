/*
 * hardwaremanager.cpp
 *
 *  Created on: Mar 8, 2018
 *      Author: dev03
 */


#include "hardwaremanager.h"
#include "wiringPi.h"
#include "devices/device.h"
#include "threads/fancontrolthread.h"
#include "tasks/thermaldetectiontask.h"
#include <thread>
#include <vector>
#include "../dsrutils_global.h"

#define TACHO_PIN 7
#define FAN_PIN 3
#define SHUTTER_2_PIN 4
#define CUTOFF_PIN 22
#define SHUTTER_1_PIN 21
#define LED_PIN 26
#define THERMO_PIN 23
#define HEATER_PIN 11

using namespace Dsr;

char *_ds18b20ids[] = {"0517812410ff", "0517812410ff"};// 000002ec04a0
char *_pwmfanids[] = {"pwm-01"};

::std::vector<::std::pair<char*, char*>> _ds18b20_pwmfan = ::std::vector<::std::pair<char*, char*>>{::std::pair<char*, char*>("000002ec04a0", "pwm-01"),
	::std::pair<char*, char*>("0517812410ff", "pwm-01")};

static Dsr::HardwareManager *_hardmaninstance = NULL;
Dsr::HardwareManager* Dsr::HardwareManager::SharedInstance(){
    if(_hardmaninstance == NULL){
        _hardmaninstance = new Dsr::HardwareManager();
    }

    return _hardmaninstance;
}

Dsr::HardwareManager::HardwareManager() : fanControlThread(NULL), thermalTask(NULL){
    wiringPiSetup();
    this->addDs18B20s();
    this->addPwmFans();
    startFanControlThread();

    startThermalDetectionTask();
}

void Dsr::HardwareManager::startFanControlThread(){
    fanControlThread = new FanControlThread(this);
}

void Dsr::HardwareManager::startThermalDetectionTask(){

	if(thermalTask == NULL){
		thermalTask = new ThermalDetectionTask(devDs18B20s, fanControlThread);
	}
    ::std::thread localT(::std::ref(*thermalTask));
    localT.join();
}

void Dsr::HardwareManager::ThermalDetected(int value, char* devId){
    fanControlThread->enqueue(::std::pair<int, char*>(value, devId));
}

void Dsr::HardwareManager::ThermalDetectionFinished(){

    // repeat the task
    startThermalDetectionTask();
}

int Dsr::HardwareManager::addDs18B20s(){
    int size = sizeof(_ds18b20ids)/sizeof(_ds18b20ids[0]);
    for(int i = 0; i < size; ++i){
        DevDs18B20* dev = new DevDs18B20(_ds18b20ids[i], QTGPIO_DS18B20_PIN_BASE + i);
        this->devDs18B20s.push_back(dev);
    }
    return size;
}

int Dsr::HardwareManager::addPwmFans(){
    int size = sizeof(_pwmfanids)/sizeof(_pwmfanids[0]);
    for(int i = 0; i < size; ++i){
        DevPwmFan* dev = new DevPwmFan(_pwmfanids[i], QTGPIO_PWM_PIN_WPI_1);
        this->devPwmFans.push_back(dev);
    }
    return size;
}

Dsr::HardwareManager::~HardwareManager(){
    this->devDs18B20s.clear();
    this->devPwmFans.clear();
    delete thermalTask;
    fanControlThread->abort();
    delete fanControlThread;
}

int Dsr::HardwareManager::RefreshHardware(){
    pinMode(FAN_PIN, OUTPUT);
    pinMode(CUTOFF_PIN, OUTPUT);
    pinMode(LED_PIN, OUTPUT);
    pinMode(SHUTTER_2_PIN, OUTPUT);
    pinMode(SHUTTER_1_PIN, OUTPUT);
    pinMode(HEATER_PIN, OUTPUT);

    digitalWrite(FAN_PIN, 0);
    digitalWrite(CUTOFF_PIN, 0);
    digitalWrite(LED_PIN, 0);
    digitalWrite(SHUTTER_2_PIN, 0);
    digitalWrite(SHUTTER_1_PIN, 0);
    digitalWrite(HEATER_PIN, 0);

    return 0;
}

int Dsr::HardwareManager::PinForComponent(HardwareComponent component){
    switch(component){
        case FAN:
            return FAN_PIN;
        case CUTOFF:
            return CUTOFF_PIN;
        case LED:
            return LED_PIN;
        case SHUTTER2:
            return SHUTTER_2_PIN;
        case SHUTTER1:
            return SHUTTER_1_PIN;
        case HEATER:
            return HEATER_PIN;
    }

    return -1;
}

bool Dsr::HardwareManager::GetComponentState(HardwareComponent component){
    switch(component){
        case FAN:
            return this->fanEngaged;
        case CUTOFF:
            return this->cutoffEngaged;
        case LED:
            return this->ledEngaged;
        case SHUTTER2:
            return this->shutter2Engaged;
        case SHUTTER1:
            return this->shutter1Engaged;
        case HEATER:
            return this->heaterEngaged;
    }

    return false;
}
bool Dsr::HardwareManager::SetComponentState(HardwareComponent component, bool engaged){

    digitalWrite(this->PinForComponent(component), (engaged ? HIGH : LOW));

    switch(component){
        case FAN:
            this->fanEngaged = engaged;
        case CUTOFF:
            this->cutoffEngaged = engaged;
        case LED:
            this->ledEngaged = engaged;
        case SHUTTER2:
            this->shutter2Engaged = engaged;
        case SHUTTER1:
            this->shutter1Engaged = engaged;
        case HEATER:
            this->heaterEngaged = engaged;
    }

    return -1;
}

void Dsr::HardwareManager::TriggerCapture(){

    this->SetComponentState(HardwareComponent::SHUTTER1, true);
    //QThread::msleep(100);
    this->SetComponentState(HardwareComponent::SHUTTER2, true);
    //QThread::msleep(100);
    this->SetComponentState(HardwareComponent::SHUTTER2, false);
    //QThread::msleep(100);
    this->SetComponentState(HardwareComponent::SHUTTER1, false);
    //QThread::msleep(100);
}

Dsr::Device* Dsr::HardwareManager::findDeviceById(char* id) {
    // todo: need to figure the actual connections pi has two pwm pins BCM12/18 and BCM13 corresponding to wiringPi pin 1 and 23
    Device* dev = NULL;
    for(int i = 0; i < devDs18B20s.size(); i++){
        if(devDs18B20s.at(i)->getDeviceId() == id){
            dev = devDs18B20s.at(i);
            return dev;
        }
    }

    for(int i = 0; i < devPwmFans.size(); i++){
        if(devPwmFans.at(i)->getDeviceId() == id){
            dev = devPwmFans.at(i);
            return dev;
        }
    }

    return dev;
}

Dsr::Device* Dsr::HardwareManager::findPwmFanByDs18B20Id(char* id){
    Device* dev = NULL;
    for(int i = 0; i < _ds18b20_pwmfan.size(); i++){
        if(_ds18b20_pwmfan.at(i).first == id) {
            dev = findDeviceById(_ds18b20_pwmfan.at(i).second);
            return dev;
        }
    }
    return dev;
}


