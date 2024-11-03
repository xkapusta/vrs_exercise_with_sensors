################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lps25hb/lps25hb.c 

OBJS += \
./lps25hb/lps25hb.o 

C_DEPS += \
./lps25hb/lps25hb.d 


# Each subdirectory must supply rules for building sources it contributes
lps25hb/%.o lps25hb/%.su lps25hb/%.cyclo: ../lps25hb/%.c lps25hb/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32F303x8 -DUSE_FULL_LL_DRIVER -DHSE_VALUE=8000000 -DHSE_STARTUP_TIMEOUT=100 -DLSE_STARTUP_TIMEOUT=5000 -DLSE_VALUE=32768 -DEXTERNAL_CLOCK_VALUE=8000000 -DHSI_VALUE=8000000 -DLSI_VALUE=40000 -DVDD_VALUE=3300 -DPREFETCH_ENABLE=1 -c -I../Core/Inc -I../Drivers/STM32F3xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F3xx/Include -I../Drivers/CMSIS/Include -I"D:/STM32CubeIDE_app/WorkSpace/vrs_exercise_repo/vrs_exercise_sensors/lps25hb" -I"D:/STM32CubeIDE_app/WorkSpace/vrs_exercise_repo/vrs_exercise_sensors/hts221" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-lps25hb

clean-lps25hb:
	-$(RM) ./lps25hb/lps25hb.cyclo ./lps25hb/lps25hb.d ./lps25hb/lps25hb.o ./lps25hb/lps25hb.su

.PHONY: clean-lps25hb

