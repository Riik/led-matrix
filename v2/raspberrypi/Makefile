include mk/compilers.mk
include mk/emptyvars.mk
include mk/predefined.mk
include mk/flags.mk
include mk/receipes.mk

RELEASE_TARGET := final
DEBUG_TARGET := final_debug
TEST_TARGET := final_test

DIRS := $(shell ls -d */)
DIRS := $(DIRS:=rules.mk)
DIRS += ./rules.mk

-include $(DIRS)
-include $(ALLOFILES:%.o=%.d)

INC := $(addprefix -I,$(INCDIRS))

.DEFAULT_GOAL := test

.PHONY: all test debug release clean

all: release debug test

release: $(RELEASE_TARGET)

debug: $(DEBUG_TARGET)

test: $(TEST_TARGET)

$(ALLOFILES) : Makefile

$(ODIRS) :
	$(MKDIR-RECEIPE)

$(RELEASE_TARGET) : $(RELEASE_OFILES)
	$(RPI4-RELEASE-LINKRECEIPE)

$(DEBUG_TARGET) : $(DEBUG_OFILES)
	$(RPI4-DEBUG-LINKRECEIPE)

$(TEST_TARGET) : $(filter-out $(ROOTTESTODIR)main.cpp.o, $(TEST_OFILES))
	$(HOST-TEST-LINKRECEIPE)

clean:
	rm -rf $(OBJ_DIR)
	rm -f $(RELEASE_TARGET)
	rm -f $(DEBUG_TARGET)
	rm -f $(TEST_TARGET)
