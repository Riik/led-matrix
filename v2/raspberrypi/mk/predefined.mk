SRCDIR := src/
INCDIR := inc/
TESTDIR := test/
DEBUG_DIR := debug
RELEASE_DIR := release
TEST_DIR := test
TESTDBG_DIR := testdbg
OBJ_DIR := obj
DEBUGOBJ_DIR := $(OBJ_DIR)/$(DEBUG_DIR)
RELEASEOBJ_DIR := $(OBJ_DIR)/$(RELEASE_DIR)
TESTOBJ_DIR := $(OBJ_DIR)/$(TEST_DIR)
TESTDBGOBJ_DIR := $(OBJ_DIR)/$(TESTDBG_DIR)
HOST-TEST-LIBS :=-lgtest -lgtest_main -lgmock
RPI4-DEBUG-LIBS :=
RPI4-RELEASE-LIBS :=
