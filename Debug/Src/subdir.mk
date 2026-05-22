################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/a6_driver.c \
../Src/a6_lib.c \
../Src/fifo.c \
../Src/hardware_modules.c \
../Src/main.c \
../Src/rtc.c \
../Src/syscalls.c \
../Src/sysmem.c 

OBJS += \
./Src/a6_driver.o \
./Src/a6_lib.o \
./Src/fifo.o \
./Src/hardware_modules.o \
./Src/main.o \
./Src/rtc.o \
./Src/syscalls.o \
./Src/sysmem.o 

C_DEPS += \
./Src/a6_driver.d \
./Src/a6_lib.d \
./Src/fifo.d \
./Src/hardware_modules.d \
./Src/main.d \
./Src/rtc.d \
./Src/syscalls.d \
./Src/sysmem.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o Src/%.su Src/%.cyclo: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DNUCLEO_F401RE -DSTM32 -DSTM32F401RETx -DSTM32F4 -DSTM32F401xE -c -I../Inc -I"../${{ProjDirPath}}/chip_headers/CMSIS/Device/ST/STM32F4xx/Include" -I"../${{ProjDirPath}}/chip_headers/CMSIS/Include" -I"C:/Users/Flow/Desktop/A6_GSMGPRS/Workspace_A6/0_A6_driver_1/chip_headers/CMSIS/Device/ST/STM32F4xx/Include" -I"C:/Users/Flow/Desktop/A6_GSMGPRS/Workspace_A6/0_A6_driver_1/chip_headers/CMSIS/Include" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Src

clean-Src:
	-$(RM) ./Src/a6_driver.cyclo ./Src/a6_driver.d ./Src/a6_driver.o ./Src/a6_driver.su ./Src/a6_lib.cyclo ./Src/a6_lib.d ./Src/a6_lib.o ./Src/a6_lib.su ./Src/fifo.cyclo ./Src/fifo.d ./Src/fifo.o ./Src/fifo.su ./Src/hardware_modules.cyclo ./Src/hardware_modules.d ./Src/hardware_modules.o ./Src/hardware_modules.su ./Src/main.cyclo ./Src/main.d ./Src/main.o ./Src/main.su ./Src/rtc.cyclo ./Src/rtc.d ./Src/rtc.o ./Src/rtc.su ./Src/syscalls.cyclo ./Src/syscalls.d ./Src/syscalls.o ./Src/syscalls.su ./Src/sysmem.cyclo ./Src/sysmem.d ./Src/sysmem.o ./Src/sysmem.su

.PHONY: clean-Src

