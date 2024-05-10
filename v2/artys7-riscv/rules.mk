DIR := $(subst ./,,$(dir $(subst $(TOP)/,,$(abspath $(lastword $(MAKEFILE_LIST))))))

OPTFLAGS := -mbranch-cost=3 -DNDEBUG -O3
CXXLANGFLAGS := -std=gnu++20 -fno-rtti -fno-use-cxa-atexit -fno-exceptions -fno-threadsafe-statics
CLANGFLAGS := -std=gnu17
WARNINGFLAGS := -Wall -Wshadow=local -Werror -Wno-error=unused-variable -Wno-error=unused-but-set-variable -Wdouble-promotion
MAKESUPPORTFLAGS := -MMD -MP
CFLAGS := $(CLANGFLAGS) $(OPTFLAGS) $(WARNINGFLAGS) $(MAKESUPPORTFLAGS) $(ARCHFLAGS)
CXXFLAGS := $(CXXLANGFLAGS) $(OPTFLAGS) $(WARNINGFLAGS) $(MAKESUPPORTFLAGS) $(ARCHFLAGS)

INCLUDES += -I$(DIR)inc
SRCDIR := $(DIR)src/
ASMDIR := $(DIR)asm/

RELEASEODIR := $(RELEASE_DIR)/$(DIR)

ODIRS += $(RELEASEODIR)

CXXFILES := $(wildcard $(SRCDIR)*.cpp)
CFILES := $(wildcard $(SRCDIR)*.c)
ASMFILES := $(wildcard $(ASMDIR)*.asm)

ALLOFILES += $(patsubst $(SRCDIR)%,$(RELEASEODIR)%,$(patsubst %.cpp,%.cpp.o,$(CXXFILES)))
ALLOFILES += $(patsubst $(SRCDIR)%,$(RELEASEODIR)%,$(patsubst %.c,%.c.o,$(CFILES)))
ALLOFILES += $(patsubst $(ASMDIR)%,$(RELEASEODIR)%,$(patsubst %.asm,%.asm.o,$(ASMFILES)))

$(RELEASEODIR)%.cpp.o: $(SRCDIR)%.cpp | $(RELEASEODIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(RELEASEODIR)%.c.o: $(SRCDIR)%.c | $(RELEASEODIR)
	$(GCC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(RELEASEODIR)%.asm.o : $(ASMDIR)%.asm | $(RELEASEODIR)
	$(AS) $(ARCHFLAGS) $< -o $@
