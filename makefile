PROGRAM:=git-big

################################################################################

BIG_ROOT_DIR:=$(shell DIRECTORY="$(dir $(realpath $(lastword $(MAKEFILE_LIST))))"; echo $${DIRECTORY%/})

BIN_DIR:=$(BIG_ROOT_DIR)/bin
OBJ_DIR:=$(BIG_ROOT_DIR)/obj
SRC_DIR:=$(BIG_ROOT_DIR)/src

CC:=clang
CC_FLAGS:=-std=c99
CPP_FLAGS:=

LD:=clang
LD_FLAGS:=

################################################################################

ifeq ($(OS),Windows_NT)
    CPP_FLAGS+=-D TARGET_OS_WINDOWS
    TARGET_LIBRARIES:=
else
    UNAME_S:=$(shell uname -s)

    ifeq ($(UNAME_S),Linux)
        CPP_FLAGS+=-D TARGET_OS_LINUX
        TARGET_LIBRARIES:=-lcrypto
    endif
    ifeq ($(UNAME_S),Darwin)
        CPP_FLAGS+=-D TARGET_OS_OSX
        TARGET_LIBRARIES:=
    endif
endif

################################################################################

SOURCES:=$(shell find $(SRC_DIR) -type f -name "*.c")
OBJECTS:=$(patsubst $(SRC_DIR)/%,$(OBJ_DIR)/%,$(SOURCES:.c=.o))

DEPS:=$(OBJECTS:.o=.deps)

PROGRAM_BINARY:=$(BIN_DIR)/$(PROGRAM)

################################################################################

all: $(PROGRAM_BINARY)

clean:
	@rm -rf $(PROGRAM_BINARY)
	@rm -rf $(DEPS)
	@rm -rf $(OBJECTS)

################################################################################

$(PROGRAM_BINARY): $(OBJECTS)
	@mkdir -p $(shell dirname $@)
	@$(LD) $(LD_FLAGS) -lgit2 $(TARGET_LIBRARIES) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(shell dirname $@)
	@$(CC) -c $(CPP_FLAGS) $(CC_FLAGS) -MD -MF $(@:.o=.deps) -o $@ $<

################################################################################

-include $(DEPS)
