################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/DEV_Config.c \
../Src/ICM20948.c \
../Src/IMU.c \
../Src/main_icm.c 

OBJS += \
./Src/DEV_Config.o \
./Src/ICM20948.o \
./Src/IMU.o \
./Src/gpio.o \
./Src/i2c.o \
./Src/main.o \
./Src/main_icm.o \
./Src/stm32f1xx_hal_msp.o \
./Src/stm32f1xx_it.o \
./Src/usart.o 

C_DEPS += \
./Src/DEV_Config.d \
./Src/ICM20948.d \
./Src/IMU.d \
./Src/gpio.d \
./Src/i2c.d \
./Src/main.d \
./Src/main_icm.d \
./Src/stm32f1xx_hal_msp.d \
./Src/stm32f1xx_it.d \
./Src/usart.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o Src/%.su: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../../Inc -I../../Drivers/STM32F1xx_HAL_Driver/Inc -I../../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Src

clean-Src:
	-$(RM) ./Src/DEV_Config.d ./Src/DEV_Config.o ./Src/DEV_Config.su ./Src/ICM20948.d ./Src/ICM20948.o ./Src/ICM20948.su ./Src/IMU.d ./Src/IMU.o ./Src/IMU.su ./Src/gpio.d ./Src/gpio.o ./Src/gpio.su ./Src/i2c.d ./Src/i2c.o ./Src/i2c.su ./Src/main.d ./Src/main.o ./Src/main.su ./Src/main_icm.d ./Src/main_icm.o ./Src/main_icm.su ./Src/stm32f1xx_hal_msp.d ./Src/stm32f1xx_hal_msp.o ./Src/stm32f1xx_hal_msp.su ./Src/stm32f1xx_it.d ./Src/stm32f1xx_it.o ./Src/stm32f1xx_it.su ./Src/usart.d ./Src/usart.o ./Src/usart.su

.PHONY: clean-Src

