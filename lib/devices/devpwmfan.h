/*
 * devpwmfan.h
 *
 *  Created on: Mar 8, 2018
 *      Author: dev03
 */

#ifndef LIB_DEVICES_DEVPWMFAN_H_
#define LIB_DEVICES_DEVPWMFAN_H_


#include "device.h"

namespace Dsr {

class DevPwmFan : public Device {
private:
    char* deviceId;
    int pinNum;
    int init();
public:
    DevPwmFan(char* id, int pin);
    virtual void write(int pin, int value);
    virtual int read(int pin);
    virtual int getPinNum();
    virtual char* getDeviceId();
    ~DevPwmFan();
};

}


#endif /* LIB_DEVICES_DEVPWMFAN_H_ */
