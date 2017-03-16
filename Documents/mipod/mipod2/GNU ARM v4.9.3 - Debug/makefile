################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include sensors/subdir.mk
-include emlib/subdir.mk
-include bmx/subdir.mk
-include CMSIS/EFM32WG/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: mipod2.axf

# Tool invocations
mipod2.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GNU ARM C Linker'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m4 -mthumb -T "mipod2.ld" -Xlinker --gc-sections -Xlinker -Map="mipod2.map" -mfpu=fpv4-sp-d16 -mfloat-abi=softfp --specs=nano.specs -o mipod2.axf "./CMSIS/EFM32WG/startup_efm32wg.o" "./CMSIS/EFM32WG/system_efm32wg.o" "./bmx/em_acmp.o" "./bmx/em_dma.o" "./bmx/em_int.o" "./bmx/em_lcd.o" "./bmx/em_ldma.o" "./bmx/em_rtc.o" "./bmx/em_usart.o" "./emlib/em_cmu.o" "./emlib/em_ebi.o" "./emlib/em_emu.o" "./emlib/em_gpio.o" "./emlib/em_i2c.o" "./emlib/em_leuart.o" "./emlib/em_system.o" "./sensors/adxl375.o" "./sensors/bma055.o" "./sensors/bmg055.o" "./sensors/bmx055.o" "./sensors/nrf51422.o" "./sensors/rv3049.o" "./sensors/stc3105.o" "./src/bus.o" "./src/common.o" "./src/debug.o" "./src/main.o" "./src/s34ml04g2.o" -Wl,--start-group -lgcc -lc -lnosys -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

	@echo 'Building hex file: mipod2.hex'
	arm-none-eabi-objcopy -O ihex "mipod2.axf" "mipod2.hex"
	@echo ' '

	@echo 'Building bin file: mipod2.bin'
	arm-none-eabi-objcopy -O binary "mipod2.axf" "mipod2.bin"
	@echo ' '

	@echo 'Building s37 file: mipod2.s37'
	arm-none-eabi-objcopy -O srec "mipod2.axf" "mipod2.s37"
	@echo ' '

	@echo 'Running size tool'
	arm-none-eabi-size "mipod2.axf"
	@echo ' '

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(C_DEPS) mipod2.axf
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
