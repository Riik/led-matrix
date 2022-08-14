include mk/compilers.mk
include mk/emptyvars.mk
include mk/predefined.mk
include mk/flags.mk
include mk/receipes.mk

HOST_TARGET_DIR := host

RPI4_RELEASE_TARGET := final
RPI4_DEBUG_TARGET := final_debug
RPI4_PROFILE_TARGET := final_profile
HOST_TEST_TARGET := $(HOST_TARGET_DIR)/final_test
HOST_DEBUG_TARGET := $(HOST_TARGET_DIR)/final_debug
HOST_RELEASE_TARGET := $(HOST_TARGET_DIR)/final

ODIRS += $(HOST_TARGET_DIR)

DIRS := $(shell ls -d */)
DIRS := $(DIRS:=rules.mk)
DIRS += ./rules.mk

-include $(DIRS)
-include $(ALLOFILES:%.o=%.d)

INC := $(addprefix -I,$(INCDIRS))

.DEFAULT_GOAL := host-test

.PHONY: all host-test host-debug host-release rpi4-debug rpi4-release rpi4-profile clean rpi4 host-build

all: rpi4 host-build

rpi4: rpi4-release rpi4-debug rpi4-profile

host-build: host-release host-debug host-test

rpi4-release: $(RPI4_RELEASE_TARGET)

rpi4-profile: $(RPI4_PROFILE_TARGET)

rpi4-debug: $(RPI4_DEBUG_TARGET)

host-test: $(HOST_TEST_TARGET)

host-debug: $(HOST_DEBUG_TARGET)

host-release: $(HOST_RELEASE_TARGET)

$(ALLOFILES) : Makefile

$(ODIRS) :
	$(MKDIR-RECEIPE)

$(RPI4_RELEASE_TARGET) : $(RPI4_RELEASE_OFILES)
	$(RPI4-RELEASE-LINKRECEIPE)

$(RPI4_DEBUG_TARGET) : $(RPI4_DEBUG_OFILES)
	$(RPI4-DEBUG-LINKRECEIPE)

$(RPI4_PROFILE_TARGET) : $(RPI4_PROFILE_OFILES)
	$(RPI4-PROFILE-LINKRECEIPE)

$(HOST_TEST_TARGET) : $(HOST_TEST_OFILES) | $(HOST_TARGET_DIR)
	$(HOST-TEST-LINKRECEIPE)

$(HOST_DEBUG_TARGET) : $(HOST_DEBUG_OFILES) | $(HOST_TARGET_DIR)
	$(HOST-DEBUG-LINKRECEIPE)

$(HOST_RELEASE_TARGET) : $(HOST_RELEASE_OFILES) | $(HOST_TARGET_DIR)
	$(HOST-RELEASE-LINKRECEIPE)

clean:
	rm -rf $(OBJ_DIR)
	rm -f $(RPI4_RELEASE_TARGET)
	rm -f $(RPI4_DEBUG_TARGET)
	rm -f $(HOST_TEST_TARGET)
	rm -f $(HOST_RELEASE_TARGET)
	rm -f $(HOST_DEBUG_TARGET)

echo:
	@echo $(ODIRS)
