################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../lib/devices/devds18b20.cpp \
../lib/devices/device.cpp \
../lib/devices/devpwmfan.cpp 

OBJS += \
./lib/devices/devds18b20.o \
./lib/devices/device.o \
./lib/devices/devpwmfan.o 

CPP_DEPS += \
./lib/devices/devds18b20.d \
./lib/devices/device.d \
./lib/devices/devpwmfan.d 


# Each subdirectory must supply rules for building sources it contributes
lib/devices/%.o: ../lib/devices/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	arm-linux-gnueabihf-g++ -I/usr/local/include -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


