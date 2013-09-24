# Target specific macros
TARGET = ECE6561_Project2
TARGET_CPP_SOURCES = TaskMain.cpp test_object.cpp TaskRecord.cpp
TOPPERS_OSEK_OIL_SOURCE = ./ECE6561_Project2.oil

O_PATH ?= build

include /cygdrive/c/cygwin/nxtOSEK/ecrobot/ecrobot++.mak

