SRCDIR := src/
INCDIR := inc/
TESTDIR := test/
OBJ_DIR := obj/
RPI4_DEBUG_DIR := $(OBJ_DIR)rpi4_debug
RPI4_RELEASE_DIR := $(OBJ_DIR)rpi4_release
HOST_TEST_DIR := $(OBJ_DIR)host_test
HOST_DEBUG_DIR := $(OBJ_DIR)host_debug
HOST_RELEASE_DIR := $(OBJ_DIR)host_release
HOST-TEST-LIBS :=-lgtest -lgtest_main -lgmock
RPI4-DEBUG-LIBS :=
RPI4-RELEASE-LIBS :=
