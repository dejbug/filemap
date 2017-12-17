# user settings

DEBUG ?= 1

NAME := filemap

SYMBOLS := WIN32_LEAN_AND_MEAN STRICT UNICODE _UNICODE
LDLIBS :=

BUILD_DIR := build
DEPLOY_DIR := deploy

CXXFLAGS_DEBUG := -g -Og
CXXFLAGS_RELEASE := -O3 -DNDEBUG

# prologue

CXX := g++

CXXFLAGS := -std=c++11 -pedantic -Wall -Wextra
CXXFLAGS += $(addprefix -D,$(SYMBOLS))
LDFLAGS := $(addprefix -l,$(LDLIBS))

ifneq ($(DEBUG),0)
CXXFLAGS += $(CXXFLAGS_DEBUG)
else
CXXFLAGS += $(CXXFLAGS_RELEASE)
endif

# generics

SOURCES := $(wildcard src/*.cpp)
PREREQS := $(SOURCES:src/%.cpp=$(BUILD_DIR)/%.d)
OBJECTS := $(SOURCES:src/%.cpp=$(BUILD_DIR)/%.o)

$(DEPLOY_DIR)/$(NAME).exe : $(OBJECTS) | $(DEPLOY_DIR)
$(DEPLOY_DIR)/$(NAME).exe : ; $(call LINK,$@,$^)

$(BUILD_DIR)/%.d : src/%.cpp | $(BUILD_DIR) ; $(call GEN_PREREQ,$@,$^)
$(BUILD_DIR)/%.o : src/%.cpp | $(BUILD_DIR) ; $(call COMPILE,$@,$^)

$(BUILD_DIR) : ; $(call MAKE_DIR,$@)
$(DEPLOY_DIR) : ; $(call MAKE_DIR,$@)

.PHONY : clean reset run

clean : ; $(call REMOVE_TREE,$(BUILD_DIR)/)
reset : | clean ; $(call REMOVE_TREE,$(DEPLOY_DIR)/)
run : $(DEPLOY_DIR)/$(NAME).exe ; $<

# synonyms ( i.e. canned MAKE commands )

## synonyms for building

define COMPILE
@echo [ -- compiling "$1" ]
$(CXX) -o $1 -c $(filter %.cpp,$2) $(CXXFLAGS)
endef

define LINK
@echo [ -- linking "$1" ]
$(CXX) -o $1 $(filter %.o %.a,$2) $(LDLIBS) $(CXXFLAGS) $(LDFLAGS)
endef

define GEN_PREREQ
@echo [ -- generating prerequisites "$1" ]
$(CXX) -MF $1 -MM $(firstword $2) -MT "$(subst .d,.o,$1) $1" $(CXXFLAGS)
endef

## misc synonyms

define TO_LIN_PATH
$(subst \,/,$1)
endef

define TO_WIN_PATH
$(subst /,\,$1)
endef

define DISTINCT
$(sort $1)
endef

define HAS_GOAL
$(findstring $1,$(MAKECMDGOALS))
endef

define HAS_NON_BUILD_GOAL
$(or $(call HAS_GOAL,clean),$(call HAS_GOAL,reset))
endef

# generic system calls ( only python was assumed )

define LIST_DIR
$(call TO_LIN_PATH,$(shell python -c "import os.path,itertools; it = os.walk('$1'); it = (tuple('\x22%s\x22' % os.path.join(t,n) for n in nn) for t,dd,nn in it); it = itertools.chain(*it); print ' '.join(it)"))
endef

define MAKE_DIR
$(shell python -c "import os.path; os.path.exists('$1') or os.makedirs('$1')")
endef

define REMOVE_TREE
$(shell python -c "import shutil; shutil.rmtree('$1', True)")
endef

define STRIP_PATH
$(call TO_LIN_PATH,$(shell python -c "print '$1'.strip(' /\\')"))
endef

define TO_ABS_PATH
$(call TO_LIN_PATH,$(shell python -c "import os.path; print os.path.abspath('$1')"))
endef

# epilogue

.DELETE_ON_ERROR :

ifneq ($(PRECISE),0)
# $(info '$(call HAS_NON_BUILD_GOAL)')
ifeq (,$(call HAS_NON_BUILD_GOAL))
include $(PREREQS)
endif
endif
