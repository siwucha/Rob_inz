################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../startup/startup_stm32.s 

OBJS += \
./startup/startup_stm32.o 


# Each subdirectory must supply rules for building sources it contributes
startup/%.o: ../startup/%.s
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Assembler'
	@echo %cd%
	arm-none-eabi-as -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -I"C:/Users/Darek/workspace/Rob_inz/inc" -I"C:/Users/Darek/workspace/Rob_inz/CMSIS/core" -I"C:/Users/Darek/workspace/Rob_inz/CMSIS/device" -I"C:/Users/Darek/workspace/Rob_inz/StdPeriph_Driver/inc" -IC:/GnuWin32/include -g -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


