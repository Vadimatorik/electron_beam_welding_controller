################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../cpp_system_calls.c \
../main.c \
../system_stm32f2xx.c 

OBJS += \
./cpp_system_calls.o \
./main.o \
./system_stm32f2xx.o 

C_DEPS += \
./cpp_system_calls.d \
./main.d \
./system_stm32f2xx.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -O1 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -DSTM32F205xx -I"C:\Users\Vadim\Desktop\scan_el" -I"C:\Users\Vadim\Desktop\scan_el\Drivers" -I"C:\Users\Vadim\Desktop\scan_el\Drivers\CMSIS" -I"C:\Users\Vadim\Desktop\scan_el\Drivers\CMSIS\Include" -I"C:\Users\Vadim\Desktop\scan_el\Drivers\STM32F2xx_HAL_Driver\Inc" -I"C:\Users\Vadim\Desktop\scan_el\Drivers\STM32F2xx_HAL_Driver\Src" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


