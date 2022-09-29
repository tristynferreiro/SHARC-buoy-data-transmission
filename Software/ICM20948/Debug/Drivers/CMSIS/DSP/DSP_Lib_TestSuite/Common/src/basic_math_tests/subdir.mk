################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/src/basic_math_tests/abs_tests.c \
../Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/src/basic_math_tests/add_tests.c \
../Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/src/basic_math_tests/basic_math_test_common_data.c \
../Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/src/basic_math_tests/basic_math_test_group.c \
../Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/src/basic_math_tests/dot_prod_tests.c \
../Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/src/basic_math_tests/mult_tests.c \
../Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/src/basic_math_tests/negate_tests.c \
../Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/src/basic_math_tests/offset_tests.c \
../Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/src/basic_math_tests/scale_tests.c \
../Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/src/basic_math_tests/shift_tests.c \
../Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/src/basic_math_tests/sub_tests.c 

OBJS += \
./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/src/basic_math_tests/abs_tests.o \
./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/src/basic_math_tests/add_tests.o \
./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/src/basic_math_tests/basic_math_test_common_data.o \
./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/src/basic_math_tests/basic_math_test_group.o \
./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/src/basic_math_tests/dot_prod_tests.o \
./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/src/basic_math_tests/mult_tests.o \
./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/src/basic_math_tests/negate_tests.o \
./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/src/basic_math_tests/offset_tests.o \
./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/src/basic_math_tests/scale_tests.o \
./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/src/basic_math_tests/shift_tests.o \
./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/src/basic_math_tests/sub_tests.o 

C_DEPS += \
./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/src/basic_math_tests/abs_tests.d \
./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/src/basic_math_tests/add_tests.d \
./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/src/basic_math_tests/basic_math_test_common_data.d \
./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/src/basic_math_tests/basic_math_test_group.d \
./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/src/basic_math_tests/dot_prod_tests.d \
./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/src/basic_math_tests/mult_tests.d \
./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/src/basic_math_tests/negate_tests.d \
./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/src/basic_math_tests/offset_tests.d \
./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/src/basic_math_tests/scale_tests.d \
./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/src/basic_math_tests/shift_tests.d \
./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/src/basic_math_tests/sub_tests.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/src/basic_math_tests/%.o Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/src/basic_math_tests/%.su: ../Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/src/basic_math_tests/%.c Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/src/basic_math_tests/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../../Inc -I../../Drivers/STM32F1xx_HAL_Driver/Inc -I../../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Drivers-2f-CMSIS-2f-DSP-2f-DSP_Lib_TestSuite-2f-Common-2f-src-2f-basic_math_tests

clean-Drivers-2f-CMSIS-2f-DSP-2f-DSP_Lib_TestSuite-2f-Common-2f-src-2f-basic_math_tests:
	-$(RM) ./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/src/basic_math_tests/abs_tests.d ./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/src/basic_math_tests/abs_tests.o ./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/src/basic_math_tests/abs_tests.su ./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/src/basic_math_tests/add_tests.d ./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/src/basic_math_tests/add_tests.o ./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/src/basic_math_tests/add_tests.su ./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/src/basic_math_tests/basic_math_test_common_data.d ./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/src/basic_math_tests/basic_math_test_common_data.o ./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/src/basic_math_tests/basic_math_test_common_data.su ./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/src/basic_math_tests/basic_math_test_group.d ./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/src/basic_math_tests/basic_math_test_group.o ./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/src/basic_math_tests/basic_math_test_group.su ./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/src/basic_math_tests/dot_prod_tests.d ./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/src/basic_math_tests/dot_prod_tests.o ./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/src/basic_math_tests/dot_prod_tests.su ./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/src/basic_math_tests/mult_tests.d ./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/src/basic_math_tests/mult_tests.o ./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/src/basic_math_tests/mult_tests.su ./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/src/basic_math_tests/negate_tests.d ./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/src/basic_math_tests/negate_tests.o ./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/src/basic_math_tests/negate_tests.su ./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/src/basic_math_tests/offset_tests.d ./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/src/basic_math_tests/offset_tests.o ./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/src/basic_math_tests/offset_tests.su ./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/src/basic_math_tests/scale_tests.d ./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/src/basic_math_tests/scale_tests.o ./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/src/basic_math_tests/scale_tests.su ./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/src/basic_math_tests/shift_tests.d ./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/src/basic_math_tests/shift_tests.o ./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/src/basic_math_tests/shift_tests.su ./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/src/basic_math_tests/sub_tests.d ./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/src/basic_math_tests/sub_tests.o ./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/src/basic_math_tests/sub_tests.su

.PHONY: clean-Drivers-2f-CMSIS-2f-DSP-2f-DSP_Lib_TestSuite-2f-Common-2f-src-2f-basic_math_tests

