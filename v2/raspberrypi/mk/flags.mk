DEBUGFLAGS := -D_GLIBCXX_DEBUG -DDEBUG -Og -ggdb
RELEASEFLAGS := -DNDEBUG -O2
CXXLANGFLAGS := -std=gnu++20
WARNINGFLAGS := -Wall -Wshadow=local -Werror -Wno-error=unused-variable -Wno-error=unused-but-set-variable
MAKESUPPORTFLAGS := -MMD -MP
TEST-CXXFLAGS := $(CXXLANGFLAGS) $(DEBUGFLAGS) $(WARNINGFLAGS) $(MAKESUPPORTFLAGS)
DEBUG-CXXFLAGS := $(CXXLANGFLAGS) $(DEBUGFLAGS) $(WARNINGFLAGS) $(MAKESUPPORTFLAGS)
RELEASE-CXXFLAGS := $(CXXLANGFLAGS) $(RELEASEFLAGS) $(WARNINGFLAGS) $(MAKESUPPORTFLAGS)
TEST-LDFLAGS := -pthread
DEBUG-LDFLAGS := -pthread
RELEASE-LDFLAGS := -s -pthread
