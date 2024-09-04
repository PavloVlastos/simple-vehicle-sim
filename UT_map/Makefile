# Define the compiler
CC = gcc

# Define the compilation flags
CFLAGS = -std=c11 -O0 -Wall -g \
	-I ../modules -I ../modules/common -I ../modules/planner \
	-I ../modules/map 

# Define the linker flags
LDFLAGS = -lm

# Define the directories
SRC_DIR = ../modules
OBJ_DIR = obj
BIN_DIR = bin

# Define the main file and the executable name
TARGET = UT_map

# Define the object files
OBJS = $(OBJ_DIR)/UT_map.o \
	$(OBJ_DIR)/map.o \
	$(OBJ_DIR)/common.o \
	$(OBJ_DIR)/planner.o

$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $(BIN_DIR)/$(TARGET) $(OBJS) $(LDFLAGS)

$(OBJ_DIR)/UT_map.o: UT_map.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c UT_map.c -o $(OBJ_DIR)/UT_map.o 

$(OBJ_DIR)/map.o: $(SRC_DIR)/map/map.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $(SRC_DIR)/map/map.c -o $(OBJ_DIR)/map.o 

$(OBJ_DIR)/common.o: $(SRC_DIR)/common/common.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $(SRC_DIR)/common/common.c -o $(OBJ_DIR)/common.o

$(OBJ_DIR)/planner.o: $(SRC_DIR)/planner/planner.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $(SRC_DIR)/planner/planner.c -o $(OBJ_DIR)/planner.o

# Clean up
.PHONY: clean
clean:
	@echo "Cleaning up..."
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# Phony target to build the main project
.PHONY: all
all: $(TARGET)
