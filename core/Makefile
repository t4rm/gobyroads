CC = gcc
CFLAGS = -Wall -Wextra -Iinclude -g -O0

SRC_DIR = src
TEST_DIR = tests
UNITY_DIR = tests/unity/src
OBJ_DIR = obj
BIN_DIR = bin

# Source files (excluding main.c for tests)
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC_FILES))
OBJ_FILES_NO_MAIN = $(filter-out $(OBJ_DIR)/main.o, $(OBJ_FILES))

# Unity files
UNITY_SRC = $(UNITY_DIR)/unity.c
UNITY_OBJ = $(OBJ_DIR)/unity.o

# Test files
TEST_SRC_FILES = $(wildcard $(TEST_DIR)/*.c)
TEST_OBJ_FILES = $(patsubst $(TEST_DIR)/%.c, $(OBJ_DIR)/test_%.o, $(TEST_SRC_FILES))

# Create necessary directories
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Ensure directories exist before compilation
$(OBJ_DIR)/%.o: | $(OBJ_DIR)
$(BIN_DIR)/%: | $(BIN_DIR)

# Execute the main program
run: main
	./$(BIN_DIR)/main

# Execute tests
run_tests: tests
	./$(BIN_DIR)/tests

debug: main
	gdb $(BIN_DIR)/main

debug_tests: tests
	gdb $(BIN_DIR)/tests

# Compile main program
main: $(OBJ_FILES) | $(BIN_DIR)
	$(CC) -o $(BIN_DIR)/main $(OBJ_FILES) $(CFLAGS)

# Compile tests (link with source files except main.c)
tests: $(TEST_OBJ_FILES) $(UNITY_OBJ) $(OBJ_FILES_NO_MAIN) | $(BIN_DIR)
	$(CC) -o $(BIN_DIR)/tests $(TEST_OBJ_FILES) $(UNITY_OBJ) $(OBJ_FILES_NO_MAIN) $(CFLAGS)

# Compile Unity framework
$(UNITY_OBJ): $(UNITY_SRC) | $(OBJ_DIR)
	$(CC) -c $< -o $@ $(CFLAGS) -I$(UNITY_DIR)

# Compile source files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) -c $< -o $@ $(CFLAGS)

# Compile test files with special naming to avoid conflicts
$(OBJ_DIR)/test_%.o: $(TEST_DIR)/%.c | $(OBJ_DIR)
	$(CC) -c $< -o $@ $(CFLAGS) -I$(UNITY_DIR)

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# Create Unity directory structure
setup_unity:
	mkdir -p $(UNITY_DIR)
	@echo "Unity framework directory created"
	@echo "Please download unity.c and unity.h from Unity framework"
