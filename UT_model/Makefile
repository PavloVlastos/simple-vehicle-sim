# Define the compiler
CC = gcc

# Define the compilation flags
CFLAGS = -std=c11 -O0 -Wall \
	-I /usr/lib/gcc/x86_64-redhat-linux/8/include \
	-I ../modules -I ../modules/common \
	-I ../modules/lin_alg -I ../modules/model 

# Define the linker flags
# LDFLAGS = -L/opt/rh/gcc-toolset-10/root/usr/lib/gcc/x86_64-redhat-linux/10 
LDFLAGS = -L/usr/lib/gcc/x86_64-redhat-linux/8/include -lm

# Define the directories
SRC_DIR = ../modules
OBJ_DIR = obj
BIN_DIR = bin

# Define the main file and the executable name
TARGET = UT_model

# Define the object files
OBJS = $(OBJ_DIR)/UT_model.o \
	$(OBJ_DIR)/model.o \
	$(OBJ_DIR)/common.o

$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $(BIN_DIR)/$(TARGET) $(OBJS) $(LDFLAGS)

$(OBJ_DIR)/UT_model.o: UT_model.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c UT_model.c -o $(OBJ_DIR)/UT_model.o 

$(OBJ_DIR)/model.o: $(SRC_DIR)/model/model.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $(SRC_DIR)/model/model.c -o $(OBJ_DIR)/model.o 

$(OBJ_DIR)/common.o: $(SRC_DIR)/common/common.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $(SRC_DIR)/common/common.c -o $(OBJ_DIR)/common.o

# Clean up
.PHONY: clean
clean:
	@echo "Cleaning up..."
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# Phony target to build the main project
.PHONY: all
all: $(TARGET)
