/*
 * device.h
 *
 *  Created on: Mar 8, 2018
 *      Author: dev03
 */

#ifndef LIB_DEVICE_H_
#define LIB_DEVICE_H_

namespace Dsr {

class Device{
protected:

    void ThrowDeviceExceptionWithMessage();
public:
    explicit Device();

    virtual void write(int pin, int value)=0;
    virtual int read(int pin)=0;
    virtual int getPinNum()=0;
    virtual char* getDeviceId()=0;
    virtual ~Device();
};

}



#endif /* LIB_DEVICE_H_ */
