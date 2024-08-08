# Define the compiler
CC = gcc

# Define the compilation flags
CFLAGS = -std=c11 -O2 -Wall -I../modules -I../modules/common \
	-I../modules/interface -I../modules/lin_alg -I../modules/model \
	-I../modules/controller -I../modules/plan

# Define the linker flags
LDFLAGS = -L/opt/rh/gcc-toolset-10/root/usr/lib/gcc/x86_64-redhat-linux/10 -lm

# Define the directories
SRC_DIR = ../modules
OBJ_DIR = obj
BIN_DIR = bin

# Define the main file and the executable name
MAIN_FILE_NAME = svs_sim_main
EXEC = $(BIN_DIR)/$(MAIN_FILE_NAME)

# Define the source files
CSRCS = ../$(MAIN_FILE_NAME).c \
	$(SRC_DIR)/controller/controller.c \
	$(SRC_DIR)/model/model.c \
	$(SRC_DIR)/interface/interface.c \
	$(SRC_DIR)/lin_alg/lin_alg.c \
	$(SRC_DIR)/common/common.c

# Define the object files
OBJS = $(OBJ_DIR)/svs_sim_main.o \
	$(OBJ_DIR)/controller.o \
	$(OBJ_DIR)/model.o \
	$(OBJ_DIR)/interface.o \
	$(OBJ_DIR)/lin_alg.o \
	$(OBJ_DIR)/common.o

# Rule to build the main executable
$(EXEC): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(OBJS) $(LDFLAGS) -o $@

# Rule to build object files from modules
$(OBJ_DIR)/%.o: $(SRC_DIR)/%/%.c
	@echo ">>>>>>> $@"
	@echo ">>>>>>> $<"
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c -o $< $@

# Diagnostic rule for a specific module source file
$(OBJ_DIR)/controller.o: $(SRC_DIR)/controller/controller.c
	@mkdir -p $(OBJ_DIR)
	@echo "Compiling controller.c to controller.o"
	$(CC) $(CFLAGS) -c -o $@ $<

# Rule to build main object file
$(OBJ_DIR)/$(MAIN_FILE_NAME).o: ../$(MAIN_FILE_NAME).c
	@mkdir -p $(OBJ_DIR)
	@echo "Compiling $<"
	$(CC) $(CFLAGS) -c -o $@ $<

# Clean up
.PHONY: clean
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# Phony target to build the main project
.PHONY: all
all: $(EXEC)
