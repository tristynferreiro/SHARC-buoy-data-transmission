################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../User/IMU/IMU.c 

OBJS += \
./User/IMU/IMU.o 

C_DEPS += \
./User/IMU/IMU.d 


# Each subdirectory must supply rules for building sources it contributes
User/IMU/%.o User/IMU/%.su: ../User/IMU/%.c User/IMU/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../../Inc -I../../Drivers/STM32F1xx_HAL_Driver/Inc -I../../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-User-2f-IMU

clean-User-2f-IMU:
	-$(RM) ./User/IMU/IMU.d ./User/IMU/IMU.o ./User/IMU/IMU.su

.PHONY: clean-User-2f-IMU

