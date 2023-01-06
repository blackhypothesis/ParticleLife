################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Color.cpp \
../src/Force.cpp \
../src/ForceEditor.cpp \
../src/Graph.cpp \
../src/GroupParticle.cpp \
../src/Particle.cpp \
../src/ParticleLife.cpp \
../src/ParticleType.cpp \
../src/RandomFloat.cpp \
../src/Selector.cpp \
../src/ValueSetter.cpp 

CPP_DEPS += \
./src/Color.d \
./src/Force.d \
./src/ForceEditor.d \
./src/Graph.d \
./src/GroupParticle.d \
./src/Particle.d \
./src/ParticleLife.d \
./src/ParticleType.d \
./src/RandomFloat.d \
./src/Selector.d \
./src/ValueSetter.d 

OBJS += \
./src/Color.o \
./src/Force.o \
./src/ForceEditor.o \
./src/Graph.o \
./src/GroupParticle.o \
./src/Particle.o \
./src/ParticleLife.o \
./src/ParticleType.o \
./src/RandomFloat.o \
./src/Selector.o \
./src/ValueSetter.o 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++17 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src

clean-src:
	-$(RM) ./src/Color.d ./src/Color.o ./src/Force.d ./src/Force.o ./src/ForceEditor.d ./src/ForceEditor.o ./src/Graph.d ./src/Graph.o ./src/GroupParticle.d ./src/GroupParticle.o ./src/Particle.d ./src/Particle.o ./src/ParticleLife.d ./src/ParticleLife.o ./src/ParticleType.d ./src/ParticleType.o ./src/RandomFloat.d ./src/RandomFloat.o ./src/Selector.d ./src/Selector.o ./src/ValueSetter.d ./src/ValueSetter.o

.PHONY: clean-src

