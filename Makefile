# Target specific macros
TARGET = ECE6561_Project2
TARGET_CPP_SOURCES = TaskMain.cpp test_object.cpp TaskRecord.cpp StateMachine.cpp TaskMainSM.cpp
TOPPERS_OSEK_OIL_SOURCE = ./ECE6561_Project2.oil

 BUILD_MODE = RXE_ONLY

O_PATH ?= build

include /cygdrive/c/cygwin/nxtOSEK/ecrobot/ecrobot++.mak

