################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/ADXL345.c \
../Src/main.c \
../Src/spi_dma.c \
../Src/syscalls.c \
../Src/sysmem.c 

OBJS += \
./Src/ADXL345.o \
./Src/main.o \
./Src/spi_dma.o \
./Src/syscalls.o \
./Src/sysmem.o 

C_DEPS += \
./Src/ADXL345.d \
./Src/main.d \
./Src/spi_dma.d \
./Src/syscalls.d \
./Src/sysmem.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o Src/%.su Src/%.cyclo: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F4 -DSTM32F446RETx -c -I../Inc -I"D:/sakthivel/udemy/Advanced Embedded Systems Bare-Metal Programming/STM32_bare_metal_practice/11_SPI_DMA/stm_chip_headers/CMSIS/Include" -I"D:/sakthivel/udemy/Advanced Embedded Systems Bare-Metal Programming/STM32_bare_metal_practice/11_SPI_DMA/stm_chip_headers/CMSIS/Device/ST/STM32F4xx/Include" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Src

clean-Src:
	-$(RM) ./Src/ADXL345.cyclo ./Src/ADXL345.d ./Src/ADXL345.o ./Src/ADXL345.su ./Src/main.cyclo ./Src/main.d ./Src/main.o ./Src/main.su ./Src/spi_dma.cyclo ./Src/spi_dma.d ./Src/spi_dma.o ./Src/spi_dma.su ./Src/syscalls.cyclo ./Src/syscalls.d ./Src/syscalls.o ./Src/syscalls.su ./Src/sysmem.cyclo ./Src/sysmem.d ./Src/sysmem.o ./Src/sysmem.su

.PHONY: clean-Src

