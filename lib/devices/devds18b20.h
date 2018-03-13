/*
 * devds18b20.h
 *
 *  Created on: Mar 8, 2018
 *      Author: dev03
 */

#ifndef LIB_DEVICES_DEVDS18B20_H_
#define LIB_DEVICES_DEVDS18B20_H_



#include "device.h"

namespace Dsr {

class DevDs18B20 : public Device {
private:
    char* deviceId;
    int pinNum;
    int init();
public:
    DevDs18B20(char* id, int pin);
    virtual void write(int pin, int value);
    virtual int read(int pin);
    virtual int getPinNum();
    virtual char* getDeviceId();
    ~DevDs18B20();
};

}


#endif /* LIB_DEVICES_DEVDS18B20_H_ */
