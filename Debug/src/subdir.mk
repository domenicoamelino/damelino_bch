################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/bch.c \
../src/damelino_bch.c \
../src/galois.c \
../src/lfrs.c \
../src/utilities.c 

OBJS += \
./src/bch.o \
./src/damelino_bch.o \
./src/galois.o \
./src/lfrs.o \
./src/utilities.o 

C_DEPS += \
./src/bch.d \
./src/damelino_bch.d \
./src/galois.d \
./src/lfrs.d \
./src/utilities.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


