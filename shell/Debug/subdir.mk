################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../linkedlist.o \
../shell.o \
../stack.o \
../util.o 

C_SRCS += \
../linkedlist.c \
../linkedlistTest.c \
../shell.c \
../stack.c \
../util.c 

OBJS += \
./linkedlist.o \
./linkedlistTest.o \
./shell.o \
./stack.o \
./util.o 

C_DEPS += \
./linkedlist.d \
./linkedlistTest.d \
./shell.d \
./stack.d \
./util.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


