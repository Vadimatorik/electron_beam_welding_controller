################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_UPPER_SRCS += \
../Drivers/CMSIS/startup_stm32f205xx.S 

OBJS += \
./Drivers/CMSIS/startup_stm32f205xx.o 

S_UPPER_DEPS += \
./Drivers/CMSIS/startup_stm32f205xx.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/CMSIS/%.o: ../Drivers/CMSIS/%.S
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM Cross Assembler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -O1 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -x assembler-with-cpp -I"C:\Users\Vadim\Desktop\scan_el" -I"C:\Users\Vadim\Desktop\scan_el\Drivers" -I"C:\Users\Vadim\Desktop\scan_el\Drivers\CMSIS" -I"C:\Users\Vadim\Desktop\scan_el\Drivers\CMSIS\Include" -I"C:\Users\Vadim\Desktop\scan_el\Drivers\STM32F2xx_HAL_Driver\Inc" -I"C:\Users\Vadim\Desktop\scan_el\Drivers\STM32F2xx_HAL_Driver\Src" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


