################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../lib/tasks/thermaldetectiontask.cpp 

OBJS += \
./lib/tasks/thermaldetectiontask.o 

CPP_DEPS += \
./lib/tasks/thermaldetectiontask.d 


# Each subdirectory must supply rules for building sources it contributes
lib/tasks/%.o: ../lib/tasks/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	arm-linux-gnueabihf-g++ -I/usr/local/include -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


