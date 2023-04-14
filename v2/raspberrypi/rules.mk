DIR := $(subst ./,,$(dir $(subst $(TOP)/,,$(abspath $(lastword $(MAKEFILE_LIST))))))

INCDIRS += $(DIR)$(INCDIR)
SRC := $(DIR)$(SRCDIR)
TEST := $(DIR)$(TESTDIR)

TESTODIR := $(TEST_DIR)/$(DIR)
DEBUGODIR := $(DEBUG_DIR)/$(DIR)
RELEASEODIR := $(RELEASE_DIR)/$(DIR)

ODIRS += $(TESTODIR) $(DEBUGODIR) $(RELEASEODIR)

CXXFILES := $(wildcard $(SRC)*.cpp)
TESTCXXFILES := $(wildcard $(TEST)*.cpp)

DEBUG_OFILES += $(patsubst $(SRC)%,$(DEBUGODIR)%,$(patsubst %.cpp,%.cpp.o,$(CXXFILES)))
RELEASE_OFILES += $(patsubst $(SRC)%,$(RELEASEODIR)%,$(patsubst %.cpp,%.cpp.o,$(CXXFILES)))
TEST_OFILES += $(patsubst $(TEST)%,$(TESTODIR)%,$(patsubst %.cpp,%.cpp.o,$(TESTCXXFILES)))

$(DEBUGODIR)%.cpp.o: $(SRC)%.cpp | $(DEBUGODIR)
	$(DEBUG-BUILDRECEIPE)

$(RELEASEODIR)%.cpp.o: $(SRC)%.cpp | $(RELEASEODIR)
	$(RELEASE-BUILDRECEIPE)

$(TESTODIR)%.cpp.o: $(TEST)%.cpp | $(TESTODIR)
	$(TEST-BUILDRECEIPE)
