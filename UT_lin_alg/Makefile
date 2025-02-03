# Define the compiler
CC = gcc

# Define the compilation flags
CFLAGS = -std=c11 -O2 -Wall \
	-I../modules \
	-I../modules/common \
	-I../modules/lin_alg \

# Define coverage flags
GCOV_FLAGS = -fprofile-arcs -ftest-coverage

# Define the linker flags
LDFLAGS = -lm

# Define the directories
SRC_DIR = ../modules
OBJ_DIR = obj
BIN_DIR = bin
GCOV_DIR = coverage

# Define the main file and the executable name
TARGET = UT_lin_alg
# Define the object files
OBJS = $(OBJ_DIR)/UT_lin_alg.o \
	$(OBJ_DIR)/lin_alg.o \
	$(OBJ_DIR)/common.o

$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) -o $(BIN_DIR)/$(TARGET) $(OBJS) $(LDFLAGS) $(GCOV_FLAGS)
	@mv $(OBJ_DIR)/*.gcno $(GCOV_DIR)/ \

$(OBJ_DIR)/UT_lin_alg.o: UT_lin_alg.c
	@mkdir -p $(OBJ_DIR) $(GCOV_DIR)
	$(CC) $(CFLAGS) $(GCOV_FLAGS) -c UT_lin_alg.c -o $(OBJ_DIR)/UT_lin_alg.o

$(OBJ_DIR)/lin_alg.o: $(SRC_DIR)/lin_alg/lin_alg.c
	@mkdir -p $(OBJ_DIR) $(GCOV_DIR)
	$(CC) $(CFLAGS) $(GCOV_FLAGS) -c $(SRC_DIR)/lin_alg/lin_alg.c \
	-o $(OBJ_DIR)/lin_alg.o

$(OBJ_DIR)/common.o: $(SRC_DIR)/common/common.c
	@mkdir -p $(OBJ_DIR) $(GCOV_DIR)
	$(CC) $(CFLAGS) $(GCOV_FLAGS) -c $(SRC_DIR)/common/common.c \
	-o $(OBJ_DIR)/common.o

# Clean up
.PHONY: clean
clean:
	@echo "Cleaning up..."
	rm -rf $(OBJ_DIR) $(BIN_DIR) $(GCOV_DIR)

# Phony target to build the main project
.PHONY: all
all: $(TARGET)