CC:=arm-linux-gnueabihf-gcc
SRCDIR:=src/
CFILES:=$(wildcard $(SRCDIR)*.c)
INC:= -Iinc/ -Ilibgpiod/include
LIBDIR:=
CFLAGS:=-std=gnu18 -Wall -Wfatal-errors -march=armv8-a+crc+simd -mfpu=vfpv3-d16 -mfloat-abi=hard -pthread
CPPFLAGS:=$(INC) -MMD -MP
LDFLAGS:=-Llibgpiod/lib -lgpiod -pthread
ODIR:=obj/
DEBUGODIR=$(ODIR)debug/
RELEASEODIR=$(ODIR)release/
DEBUG_OFILES = $(patsubst $(SRCDIR)%,$(DEBUGODIR)%,$(patsubst %.c,%.c.o,$(CFILES)))
RELEASE_OFILES = $(patsubst $(SRCDIR)%,$(RELEASEODIR)%,$(patsubst %.c,%.c.o,$(CFILES)))
ALL_OFILES = $(DEBUG_OFILES) $(RELEASE_OFILES)
RELEASE_TARGET := final
DEBUG_TARGET := final_debug

.DEFAULT_GOAL := all

.PHONY: all clean debug release docs

all: release debug

release: CFLAGS += -O2 -Werror
release: $(RELEASE_TARGET)

debug: CFLAGS += -Og -ggdb
debug: CPPFLAGS += -DDEBUG
debug: $(DEBUG_TARGET)

-include $(DEBUG_OFILES:%.o=%.d)
-include $(RELEASE_OFILES:%.o=%.d)

$(ALL_OFILES) : Makefile

$(RELEASEODIR) $(DEBUGODIR) :
	mkdir -p $@

$(DEBUGODIR)%.c.o: $(SRCDIR)%.c | $(DEBUGODIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(RELEASEODIR)%.c.o: $(SRCDIR)%.c | $(RELEASEODIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(DEBUG_TARGET): $(DEBUG_OFILES)
	$(CC) -o $@ $^ $(LDFLAGS)

$(RELEASE_TARGET): $(RELEASE_OFILES)
	$(CC) -o $@ $^ $(LDFLAGS)

clean:
	rm -rf $(ODIR)
	rm -f $(RELEASE_TARGET)
	rm -f $(DEBUG_TARGET)
