################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/CMSIS/Device/ST/STM32F2xx/Source/Templates/system_stm32f2xx.c 

OBJS += \
./Drivers/CMSIS/Device/ST/STM32F2xx/Source/Templates/system_stm32f2xx.o 

C_DEPS += \
./Drivers/CMSIS/Device/ST/STM32F2xx/Source/Templates/system_stm32f2xx.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/CMSIS/Device/ST/STM32F2xx/Source/Templates/%.o: ../Drivers/CMSIS/Device/ST/STM32F2xx/Source/Templates/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -I"/home/vadim/proj/scan_el" -I"/home/vadim/proj/scan_el/Drivers" -I"/home/vadim/proj/scan_el/Drivers/CMSIS" -I"/home/vadim/proj/scan_el/Drivers/CMSIS/Include" -I"/home/vadim/proj/scan_el/Drivers/STM32F2xx_HAL_Driver/Inc" -I"/home/vadim/proj/scan_el/Drivers/STM32F2xx_HAL_Driver/Src" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


