# Target specific macros
TARGET = ECE6561_Project2
TARGET_CPP_SOURCES = TaskMain.cpp TaskRecord.cpp StateMachine.cpp TaskMainSM.cpp TaskDisplay.cpp MotorSuite.cpp SensorSuite.cpp TaskBTConnect.cpp
TOPPERS_OSEK_OIL_SOURCE = ./ECE6561_Project2.oil

BUILD_MODE = RXE_ONLY

O_PATH ?= build

include ../../../ecrobot/ecrobot++.mak
#include /cygdrive/c/cygwin/nxtOSEK/ecrobot/ecrobot++.mak

