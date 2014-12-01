################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../src/Model.o \
../src/ShaderIO.o \
../src/ex0.o \
../src/ex2.o 

CPP_SRCS += \
../src/Model.cpp \
../src/ShaderIO.cpp \
../src/ex2.cpp 

OBJS += \
./src/Model.o \
./src/ShaderIO.o \
./src/ex2.o 

CPP_DEPS += \
./src/Model.d \
./src/ShaderIO.d \
./src/ex2.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -I/cs/course/2013/cg/OpenMesh/include -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


