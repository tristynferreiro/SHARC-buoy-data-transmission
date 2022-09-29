################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/src/intrinsics_tests/intrinsics_tests.c \
../Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/src/intrinsics_tests/intrinsics_tests_common_data.c 

OBJS += \
./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/src/intrinsics_tests/intrinsics_tests.o \
./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/src/intrinsics_tests/intrinsics_tests_common_data.o 

C_DEPS += \
./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/src/intrinsics_tests/intrinsics_tests.d \
./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/src/intrinsics_tests/intrinsics_tests_common_data.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/src/intrinsics_tests/%.o Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/src/intrinsics_tests/%.su: ../Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/src/intrinsics_tests/%.c Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/src/intrinsics_tests/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../../Inc -I../../Drivers/STM32F1xx_HAL_Driver/Inc -I../../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Drivers-2f-CMSIS-2f-DSP-2f-DSP_Lib_TestSuite-2f-Common-2f-src-2f-intrinsics_tests

clean-Drivers-2f-CMSIS-2f-DSP-2f-DSP_Lib_TestSuite-2f-Common-2f-src-2f-intrinsics_tests:
	-$(RM) ./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/src/intrinsics_tests/intrinsics_tests.d ./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/src/intrinsics_tests/intrinsics_tests.o ./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/src/intrinsics_tests/intrinsics_tests.su ./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/src/intrinsics_tests/intrinsics_tests_common_data.d ./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/src/intrinsics_tests/intrinsics_tests_common_data.o ./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/src/intrinsics_tests/intrinsics_tests_common_data.su

.PHONY: clean-Drivers-2f-CMSIS-2f-DSP-2f-DSP_Lib_TestSuite-2f-Common-2f-src-2f-intrinsics_tests

