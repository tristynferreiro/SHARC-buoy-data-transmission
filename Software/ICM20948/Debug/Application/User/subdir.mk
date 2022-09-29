################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/gpio.c \
../Src/i2c.c \
../Src/main.c \
../Src/stm32f1xx_hal_msp.c \
../Src/stm32f1xx_it.c \
../Application/User/syscalls.c \
../Application/User/sysmem.c \
../Src/usart.c 

OBJS += \
./Application/User/gpio.o \
./Application/User/i2c.o \
./Application/User/main.o \
./Application/User/stm32f1xx_hal_msp.o \
./Application/User/stm32f1xx_it.o \
./Application/User/syscalls.o \
./Application/User/sysmem.o \
./Application/User/usart.o 

C_DEPS += \
./Application/User/gpio.d \
./Application/User/i2c.d \
./Application/User/main.d \
./Application/User/stm32f1xx_hal_msp.d \
./Application/User/stm32f1xx_it.d \
./Application/User/syscalls.d \
./Application/User/sysmem.d \
./Application/User/usart.d 


# Each subdirectory must supply rules for building sources it contributes
Application/User/gpio.o: /Users/tris/Documents/2022UCT/2nd\ sem/3097/SHARC_buoy_data_transmission/Software/ICM20948/Src/gpio.c Application/User/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../../Inc -I../../Drivers/STM32F1xx_HAL_Driver/Inc -I../../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Application/User/gpio.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Application/User/i2c.o: /Users/tris/Documents/2022UCT/2nd\ sem/3097/SHARC_buoy_data_transmission/Software/ICM20948/Src/i2c.c Application/User/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../../Inc -I../../Drivers/STM32F1xx_HAL_Driver/Inc -I../../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Application/User/i2c.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Application/User/main.o: /Users/tris/Documents/2022UCT/2nd\ sem/3097/SHARC_buoy_data_transmission/Software/ICM20948/Src/main.c Application/User/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../../Inc -I../../Drivers/STM32F1xx_HAL_Driver/Inc -I../../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Application/User/main.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Application/User/stm32f1xx_hal_msp.o: /Users/tris/Documents/2022UCT/2nd\ sem/3097/SHARC_buoy_data_transmission/Software/ICM20948/Src/stm32f1xx_hal_msp.c Application/User/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../../Inc -I../../Drivers/STM32F1xx_HAL_Driver/Inc -I../../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Application/User/stm32f1xx_hal_msp.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Application/User/stm32f1xx_it.o: /Users/tris/Documents/2022UCT/2nd\ sem/3097/SHARC_buoy_data_transmission/Software/ICM20948/Src/stm32f1xx_it.c Application/User/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../../Inc -I../../Drivers/STM32F1xx_HAL_Driver/Inc -I../../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Application/User/stm32f1xx_it.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Application/User/%.o Application/User/%.su: ../Application/User/%.c Application/User/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../../Inc -I../../Drivers/STM32F1xx_HAL_Driver/Inc -I../../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Application/User/usart.o: /Users/tris/Documents/2022UCT/2nd\ sem/3097/SHARC_buoy_data_transmission/Software/ICM20948/Src/usart.c Application/User/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../../Inc -I../../Drivers/STM32F1xx_HAL_Driver/Inc -I../../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Application/User/usart.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Application-2f-User

clean-Application-2f-User:
	-$(RM) ./Application/User/gpio.d ./Application/User/gpio.o ./Application/User/gpio.su ./Application/User/i2c.d ./Application/User/i2c.o ./Application/User/i2c.su ./Application/User/main.d ./Application/User/main.o ./Application/User/main.su ./Application/User/stm32f1xx_hal_msp.d ./Application/User/stm32f1xx_hal_msp.o ./Application/User/stm32f1xx_hal_msp.su ./Application/User/stm32f1xx_it.d ./Application/User/stm32f1xx_it.o ./Application/User/stm32f1xx_it.su ./Application/User/syscalls.d ./Application/User/syscalls.o ./Application/User/syscalls.su ./Application/User/sysmem.d ./Application/User/sysmem.o ./Application/User/sysmem.su ./Application/User/usart.d ./Application/User/usart.o ./Application/User/usart.su

.PHONY: clean-Application-2f-User

