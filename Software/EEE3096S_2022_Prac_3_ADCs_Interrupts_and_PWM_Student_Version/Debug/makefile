################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Drivers/STM32F0xx_HAL_Driver/Src/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := EEE3096S_2022_Prac_3_ADCs_Interrupts_and_PWM_Student_Version
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
EEE3096S_2022_Prac_3_ADCs_Interrupts_and_PWM_Student_Version.elf \

MAP_FILES += \
EEE3096S_2022_Prac_3_ADCs_Interrupts_and_PWM_Student_Version.map \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
EEE3096S_2022_Prac_3_ADCs_Interrupts_and_PWM_Student_Version.list \


# All Target
all: main-build

# Main-build Target
main-build: EEE3096S_2022_Prac_3_ADCs_Interrupts_and_PWM_Student_Version.elf secondary-outputs

# Tool invocations
EEE3096S_2022_Prac_3_ADCs_Interrupts_and_PWM_Student_Version.elf EEE3096S_2022_Prac_3_ADCs_Interrupts_and_PWM_Student_Version.map: $(OBJS) $(USER_OBJS) C:\Users\9510\ i9\Desktop\3096-CSSSHA020-FRRTRI001\Practical3\WorkPackage3\EEE3096S_2022_Prac_3_ADCs_Interrupts_and_PWM_Student_Version\STM32F051R8TX_FLASH.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-gcc -o "EEE3096S_2022_Prac_3_ADCs_Interrupts_and_PWM_Student_Version.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m0 -T"C:\Users\9510 i9\Desktop\3096-CSSSHA020-FRRTRI001\Practical3\WorkPackage3\EEE3096S_2022_Prac_3_ADCs_Interrupts_and_PWM_Student_Version\STM32F051R8TX_FLASH.ld" --specs=nosys.specs -Wl,-Map="EEE3096S_2022_Prac_3_ADCs_Interrupts_and_PWM_Student_Version.map" -Wl,--gc-sections -static -u _printf_float --specs=nano.specs -mfloat-abi=soft -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

EEE3096S_2022_Prac_3_ADCs_Interrupts_and_PWM_Student_Version.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "EEE3096S_2022_Prac_3_ADCs_Interrupts_and_PWM_Student_Version.list"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) EEE3096S_2022_Prac_3_ADCs_Interrupts_and_PWM_Student_Version.elf EEE3096S_2022_Prac_3_ADCs_Interrupts_and_PWM_Student_Version.list EEE3096S_2022_Prac_3_ADCs_Interrupts_and_PWM_Student_Version.map default.size.stdout
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents main-build fail-specified-linker-script-missing warn-no-linker-script-specified

-include ../makefile.targets
