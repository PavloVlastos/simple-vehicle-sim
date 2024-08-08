# Define the compiler
CC = gcc

# Define the compilation flags
CFLAGS = -std=c11 -O2 -Wall -I../modules -I../modules/common \
	-I../modules/interface -I../modules/lin_alg -I../modules/model \
	-I../modules/controller -I../modules/plan -I obj

# Define the linker flags
LDFLAGS = -L/opt/rh/gcc-toolset-10/root/usr/lib/gcc/x86_64-redhat-linux/10 -lm

# Define the directories
SRC_DIR = ../modules
OBJ_DIR = obj
BIN_DIR = bin

# Define the main file and the executable name
MAIN_FILE_NAME = UT_model
EXEC = $(BIN_DIR)/$(MAIN_FILE_NAME)

# Define the source files
CSRCS = $(SRC_DIR)/model/model.c \
	$(SRC_DIR)/common/common.c

# Define the object files
OBJS = $(OBJ_DIR)/UT_model.o \
	$(OBJ_DIR)/model.o \
	$(OBJ_DIR)/common.o

# Rule to build the main executable
$(EXEC): $(OBJS)
	@mkdir -p $(BIN_DIR)
	@echo "Linking $@"
	$(CC) $(OBJS) $(LDFLAGS) -o $@

# Rule to build main object file
$(OBJ_DIR)/UT_model.o: $(MAIN_FILE_NAME).c
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
