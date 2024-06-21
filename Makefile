# Define the compiler
CC = gcc

# Define the compilation flags
CFLAGS = -std=c11 -O2 -Wall -I./modules -I./modules/common \
	-I./modules/interface -I./modules/lin_alg -I./modules/model \
	-I./modules/controller

# Define the linker flags
LDFLAGS = -L/opt/rh/gcc-toolset-10/root/usr/lib/gcc/x86_64-redhat-linux/10 -lm

# Define the directories
SRC_DIR = modules
OBJ_DIR = obj
BIN_DIR = bin

# Define the source and object files
CSRCS = $(wildcard $(SRC_DIR)/*.c) main.c \
	$(SRC_DIR)/controller/controller.c \
	$(SRC_DIR)/model/model.c \
	$(SRC_DIR)/interface/interface.c \
	$(SRC_DIR)/lin_alg/lin_alg.c \
	$(SRC_DIR)/common/common.c
COBJS = $(CSRCS:%.c=$(OBJ_DIR)/%.o)
OBJS = $(COBJS)

# Define the executable name
EXEC = $(BIN_DIR)/mav-sim

# Rule to build the executable
$(EXEC): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

# Rule to build object files
$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $<

# Clean up
.PHONY: clean
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR) *.o

# Phony target to build the project
.PHONY: all
all: $(EXEC)
