CC:=arm-linux-gnueabihf-gcc
CXX:=arm-linux-gnueabihf-g++
SRCDIR:=src/
CFILES:=$(wildcard $(SRCDIR)*.c)
CXXFILES:=$(wildcard $(SRCDIR)*.cpp)
INC:= -Iinc/
LIBDIR:=
CFLAGS:=-std=gnu18
CXXFLAGS:=-std=gnu++20
CPPFLAGS:=$(INC) -MMD -MP
LDFLAGS:= -pthread
ODIR:=obj/
DEBUGODIR:=$(ODIR)debug/
RELEASEODIR:=$(ODIR)release/
DEBUG_OFILES := $(patsubst $(SRCDIR)%,$(DEBUGODIR)%,$(patsubst %.c,%.c.o,$(CFILES)))
DEBUG_OFILES += $(patsubst $(SRCDIR)%,$(DEBUGODIR)%,$(patsubst %.cpp,%.cpp.o,$(CXXFILES)))
RELEASE_OFILES := $(patsubst $(SRCDIR)%,$(RELEASEODIR)%,$(patsubst %.c,%.c.o,$(CFILES)))
RELEASE_OFILES += $(patsubst $(SRCDIR)%,$(RELEASEODIR)%,$(patsubst %.cpp,%.cpp.o,$(CXXFILES)))
ALL_OFILES = $(DEBUG_OFILES) $(RELEASE_OFILES)
RELEASE_TARGET := final
DEBUG_TARGET := final_debug

WERROR_CONFIG :=-Wall -Wshadow=local -Werror -Wno-error=unused-variable -Wno-error=unused-but-set-variable
ARCH_CONFIG :=-march=armv8-a+crc+simd -mfpu=vfpv3-d16 -mfloat-abi=hard

RELEASE_CPP_CONFIG := $(CPPFLAGS)
RELEASE_C_CONFIG := -O2 $(CFLAGS) $(ARCH_CONFIG) $(WERROR_CONFIG)
RELEASE_CXX_CONFIG := -O2 $(CXXFLAGS) $(ARCH_CONFIG) $(WERROR_CONFIG)

DEBUG_CPP_CONFIG := -DDEBUG $(CPPFLAGS)
DEBUG_C_CONFIG := -Og -ggdb3 $(CFLAGS) $(WERROR_CONFIG)
DEBUG_CXX_CONFIG := -Og -ggdb3 $(CXXFLAGS) $(WERROR_CONFIG)

.DEFAULT_GOAL := all

.PHONY: all clean debug release

all: release debug

release: $(RELEASE_TARGET)

debug: $(DEBUG_TARGET)

-include $(DEBUG_OFILES:%.o=%.d)
-include $(RELEASE_OFILES:%.o=%.d)

$(ALL_OFILES) : Makefile

$(RELEASEODIR) $(DEBUGODIR) :
	mkdir -p $@

$(DEBUGODIR)%.c.o: $(SRCDIR)%.c | $(DEBUGODIR)
	$(CC) $(RELEASE_CPP_CONFIG) $(RELEASE_C_CONFIG) -c $< -o $@

$(DEBUGODIR)%.cpp.o: $(SRCDIR)%.cpp | $(DEBUGODIR)
	$(CXX) $(DEBUG_CPP_CONFIG) $(DEBUG_CXX_CONFIG) -c $< -o $@

$(RELEASEODIR)%.c.o: $(SRCDIR)%.c | $(RELEASEODIR)
	$(CC) $(DEBUG_CPP_CONFIG) $(DEBUG_C_CONFIG) -c $< -o $@

$(RELEASEODIR)%.cpp.o: $(SRCDIR)%.cpp | $(RELEASEODIR)
	$(CXX) $(RELEASE_CPP_CONFIG) $(RELEASE_CXX_CONFIG) -c $< -o $@

$(DEBUG_TARGET): $(DEBUG_OFILES)
	$(CXX) -o $@ $^ -g $(LDFLAGS)

$(RELEASE_TARGET): $(RELEASE_OFILES)
	$(CXX) -o $@ $^ $(LDFLAGS)

clean:
	rm -rf $(ODIR)
	rm -f $(RELEASE_TARGET)
	rm -f $(DEBUG_TARGET)
