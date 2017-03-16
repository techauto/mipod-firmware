################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
/Applications/Simplicity\ Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0/platform/Device/SiliconLabs/EFM32WG/Source/system_efm32wg.c 

S_UPPER_SRCS += \
/Applications/Simplicity\ Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0/platform/Device/SiliconLabs/EFM32WG/Source/GCC/startup_efm32wg.S 

OBJS += \
./CMSIS/EFM32WG/startup_efm32wg.o \
./CMSIS/EFM32WG/system_efm32wg.o 

C_DEPS += \
./CMSIS/EFM32WG/system_efm32wg.d 


# Each subdirectory must supply rules for building sources it contributes
CMSIS/EFM32WG/startup_efm32wg.o: /Applications/Simplicity\ Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0/platform/Device/SiliconLabs/EFM32WG/Source/GCC/startup_efm32wg.S
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM Assembler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m4 -mthumb -c -x assembler-with-cpp -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0//platform/CMSIS/Include" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0//hardware/kit/common/bsp" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0//platform/emlib/inc" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0//hardware/kit/common/drivers" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0//platform/Device/SiliconLabs/EFM32WG/Include" '-DEFM32WG840F256=1' -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

CMSIS/EFM32WG/system_efm32wg.o: /Applications/Simplicity\ Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0/platform/Device/SiliconLabs/EFM32WG/Source/system_efm32wg.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m4 -mthumb -std=c99 '-DEFM32WG840F256=1' '-DDEBUG=1' -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0/platform/emdrv/dmadrv/inc" -I"/Users/Tony/SimplicityStudio/v4_workspace/mipod2/bmx_inc" -I"/Users/Tony/SimplicityStudio/v4_workspace/mipod2/sensors" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0/platform/emdrv/common/inc" -I"/Users/Tony/SimplicityStudio/v4_workspace/mipod2/bmx_inc" -I"/Users/Tony/SimplicityStudio/v4_workspace/mipod2/src" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0/platform/emdrv/spidrv/config" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0/platform/emdrv/spidrv/inc" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0/platform/emdrv/rtcdrv/config" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0/platform/emdrv/rtcdrv/inc" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0/platform/emdrv/dmadrv/config" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0//platform/CMSIS/Include" -I"/Users/Tony/SimplicityStudio/v4_workspace/mipod2/sensors" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0//hardware/kit/common/bsp" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0//platform/emlib/inc" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0//hardware/kit/common/drivers" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0//platform/Device/SiliconLabs/EFM32WG/Include" -O0 -Wall -c -fmessage-length=0 -mno-sched-prolog -fno-builtin -ffunction-sections -fdata-sections -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -MMD -MP -MF"CMSIS/EFM32WG/system_efm32wg.d" -MT"CMSIS/EFM32WG/system_efm32wg.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


