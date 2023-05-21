include mk/compilers.mk
include mk/emptyvars.mk
include mk/predefined.mk
include mk/flags.mk
include mk/receipes.mk

TOP:=$(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))

TEST_TARGET := final_test
DEBUG_TARGET := final_debug
RELEASE_TARGET := final

ODIRS += $(TARGET_DIR)

DIRS := $(shell ls -d */)
DIRS := $(DIRS:=rules.mk)
DIRS += ./rules.mk

-include $(DIRS)
ALLOFILES := $(TEST_OFILES) $(DEBUG_OFILES) $(RELEASE_OFILES)
-include $(ALLOFILES:%.o=%.d)

INC := $(addprefix -I,$(INCDIRS))

.DEFAULT_GOAL := release

.PHONY: all test debug release

all: release debug test

test: $(TEST_TARGET)

debug: $(DEBUG_TARGET)

release: $(RELEASE_TARGET)

$(ALLOFILES) : Makefile

$(ODIRS) :
	$(MKDIR-RECEIPE)

$(TEST_TARGET) : $(TEST_OFILES) $(filter-out $(DEBUGODIR)main.cpp.o, $(DEBUG_OFILES)) | $(TARGET_DIR)
	$(TEST-LINKRECEIPE)

$(DEBUG_TARGET) : $(DEBUG_OFILES) | $(TARGET_DIR)
	$(DEBUG-LINKRECEIPE)

$(RELEASE_TARGET) : $(RELEASE_OFILES) | $(TARGET_DIR)
	$(RELEASE-LINKRECEIPE)

clean:
	rm -rf $(OBJ_DIR)
	rm -f $(TEST_TARGET)
	rm -f $(RELEASE_TARGET)
	rm -f $(DEBUG_TARGET)

echo:
	@echo $(ADIR)
	@echo $(BDIR)
