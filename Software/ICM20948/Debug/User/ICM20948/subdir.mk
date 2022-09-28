################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../User/ICM20948/ICM20948.c 

OBJS += \
./User/ICM20948/ICM20948.o 

C_DEPS += \
./User/ICM20948/ICM20948.d 


# Each subdirectory must supply rules for building sources it contributes
User/ICM20948/%.o User/ICM20948/%.su: ../User/ICM20948/%.c User/ICM20948/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../../Inc -I../../Drivers/STM32F1xx_HAL_Driver/Inc -I../../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-User-2f-ICM20948

clean-User-2f-ICM20948:
	-$(RM) ./User/ICM20948/ICM20948.d ./User/ICM20948/ICM20948.o ./User/ICM20948/ICM20948.su

.PHONY: clean-User-2f-ICM20948

