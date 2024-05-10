AS := riscv-none-elf-as
GCC := riscv-none-elf-gcc
CXX := riscv-none-elf-g++
LD := riscv-none-elf-ld
OBJCOPY:=riscv-none-elf-objcopy

ODIRS :=
ALLOFILES :=
INCLUDES := -I./

ARCHFLAGS := -march=rv32i -mabi=ilp32 -mlittle-endian
LDFLAGS := -Wl,--gc-sections -nodefaultlibs -lstdc++ -lc -lgcc

TOP:=$(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))

RELEASE_DIR := release
RELEASE_TARGET := diceSimulator
RELEASE_TARGET_BIN := diceSimulator.bin

ODIRS += $(TARGET_DIR)

DIRS := $(shell ls -d */)
DIRS := $(DIRS:=rules.mk)

-include $(DIRS)
-include $(ALLOFILES:%.o=%.d)

.DEFAULT_GOAL := all

.PHONY: all echo


all: $(RELEASE_TARGET_BIN)

$(ODIRS) :
	mkdir -p $@

$(RELEASE_TARGET) : $(ALLOFILES)
	$(CXX) -o $@ $^ $(LDFLAGS) -Tlinker_script.ld $(ARCHFLAGS)

$(RELEASE_TARGET_BIN): $(RELEASE_TARGET)
	$(OBJCOPY) -j .text -j .data -j .bss -O binary $^ $@

clean:
	rm -rf $(RELEASE_DIR)
	rm -f $(RELEASE_TARGET)

echo:
	@echo $(DIRS)
	@echo $(CXXFLAGS)
	@echo $(TOP)
	@echo $(SRCDIR)
	@echo $(CFILES)
	@echo $(RELEASEODIR)
	@echo $(ALLOFILES)
