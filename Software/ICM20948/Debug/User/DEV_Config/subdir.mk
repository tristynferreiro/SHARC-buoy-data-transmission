################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../User/DEV_Config/DEV_Config.c 

OBJS += \
./User/DEV_Config/DEV_Config.o 

C_DEPS += \
./User/DEV_Config/DEV_Config.d 


# Each subdirectory must supply rules for building sources it contributes
User/DEV_Config/%.o User/DEV_Config/%.su: ../User/DEV_Config/%.c User/DEV_Config/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../../Inc -I../../Drivers/STM32F1xx_HAL_Driver/Inc -I../../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-User-2f-DEV_Config

clean-User-2f-DEV_Config:
	-$(RM) ./User/DEV_Config/DEV_Config.d ./User/DEV_Config/DEV_Config.o ./User/DEV_Config/DEV_Config.su

.PHONY: clean-User-2f-DEV_Config

