# Define the compiler
CC = gcc

# Define the compilation flags
CFLAGS = -std=c11 -O0 -Wall -g -I ../ \
	-I../modules -I../modules/common \
	-I../modules/interface_visualize \
	-I../modules/interface_stress \
	-I../modules/lin_alg -I../modules/model \
	-I../modules/controller -I../modules/planner -I obj \
	-I../modules/parse_args -I../modules/map 

# Define the linker flags
LDFLAGS = -lm

# Define the directories
SRC_DIR = ../modules
OBJ_DIR = obj
BIN_DIR = bin

# Define the main file and the executable name
MAIN_FILE_NAME = main
EXEC = $(BIN_DIR)/$(MAIN_FILE_NAME)

# Define the source files
CSRCS = $(SRC_DIR)/controller/controller.c \
	$(SRC_DIR)/model/model.c \
	$(SRC_DIR)/interface_visualize/interface_visualize.c \
	$(SRC_DIR)/interface_stress/interface_stress.c \
	$(SRC_DIR)/lin_alg/lin_alg.c \
	$(SRC_DIR)/common/common.c \
	$(SRC_DIR)/parse_args/parse_args.c \
	$(SRC_DIR)/planner/planner.c \
	$(SRC_DIR)/map/map.c 

# Define the object files
OBJS =  $(OBJ_DIR)/$(MAIN_FILE_NAME).o \
	$(OBJ_DIR)/controller.o \
	$(OBJ_DIR)/model.o \
	$(OBJ_DIR)/interface_visualize.o \
	$(OBJ_DIR)/interface_stress.o \
	$(OBJ_DIR)/lin_alg.o \
	$(OBJ_DIR)/common.o \
	$(OBJ_DIR)/parse_args.o \
	$(OBJ_DIR)/planner.o \
	$(OBJ_DIR)/map.o

# Rule to build the main executable
$(EXEC): $(OBJS)
	@mkdir -p $(BIN_DIR)
	@echo "Linking $@"
	$(CC) $(OBJS) $(LDFLAGS) -o $@

# Rule to build main object file
$(OBJ_DIR)/$(MAIN_FILE_NAME).o: ../$(MAIN_FILE_NAME).c
	@echo "Compiling $< to $@"
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

# Loop to create rules for each source file
define MAKE_OBJ_RULE
$(OBJ_DIR)/$(notdir $(basename $(1))).o: $(1)
	@echo "Compiling $$< to $$@"
	@mkdir -p $(dir $(OBJ_DIR)/$(notdir $(1)))
	$(CC) $(CFLAGS) -c -o $$@ $$<
endef

$(foreach src,$(CSRCS),$(eval $(call MAKE_OBJ_RULE,$(src))))

# Rule to print the file names
print_src_files:
	@for src in $(CSRCS); do \
		echo "File: $$src"; \
	done

# Clean up
.PHONY: clean
clean:
	@echo "Cleaning up..."
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# Phony target to build the main project
.PHONY: all
all: $(EXEC)
