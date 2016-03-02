################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/circular_buffer.c \
../src/console.c \
../src/gps.c \
../src/main.c \
../src/position.c \
../src/syscalls.c \
../src/system_stm32f4xx.c 

OBJS += \
./src/circular_buffer.o \
./src/console.o \
./src/gps.o \
./src/main.o \
./src/position.o \
./src/syscalls.o \
./src/system_stm32f4xx.o 

C_DEPS += \
./src/circular_buffer.d \
./src/console.d \
./src/gps.d \
./src/main.d \
./src/position.d \
./src/syscalls.d \
./src/system_stm32f4xx.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32F429ZITx -DNUCLEO_F429ZI -DSTM32F4 -DSTM32 -DDEBUG -DUSE_STDPERIPH_DRIVER -DSTM32F429_439xx -I"C:/Users/Darek/workspace/Rob_inz/inc" -I"C:/Users/Darek/workspace/Rob_inz/CMSIS/core" -I"C:/Users/Darek/workspace/Rob_inz/CMSIS/device" -I"C:/Users/Darek/workspace/Rob_inz/StdPeriph_Driver/inc" -IC:/GnuWin32/include -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


