OPTFLAGS := -mbranch-cost=3 -DNDEBUG -O3
CXXLANGFLAGS := -std=gnu++20
CLANGFLAGS := -std=gnu17
WARNINGFLAGS := -Wall -Wshadow=local -Werror -Wno-error=unused-variable -Wno-error=unused-but-set-variable -Wdouble-promotion
MAKESUPPORTFLAGS := -MMD -MP
ARCHFLAGS := -march=rv32i -mabi=ilp32 -mlittle-endian
ASFLAGS := $(ARCHFLAGS)
CFLAGS := $(CLANGFLAGS) $(OPTFLAGS) $(WARNINGFLAGS) $(MAKESUPPORTFLAGS) $(ARCHFLAGS)
CXXFLAGS := $(CXXLANGFLAGS) $(OPTFLAGS) $(WARNINGFLAGS) $(MAKESUPPORTFLAGS) $(ARCHFLAGS)
LDFLAGS := -Wl,--gc-sections -nodefaultlibs -lc -lgcc
