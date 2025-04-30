CC = gcc
CFLAGS = -Wall -Wextra -Iinclude -g

SRC_DIR = src
TEST_DIR = tests
OBJ_DIR = obj
BIN_DIR = bin

SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC_FILES))
OBJ_FILES_NO_MAIN = $(filter-out $(OBJ_DIR)/main.o, $(OBJ_FILES))

# Create necessary directories
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Ensure directories exist before compilation
$(OBJ_DIR)/%.o: | $(OBJ_DIR)
$(BIN_DIR)/main: | $(BIN_DIR)
$(BIN_DIR)/test: | $(BIN_DIR)

# Execute the main file
run: main
	$(BIN_DIR)/main.exe

debug: main
	gdb $(BIN_DIR)/main.exe

# Compile every C file in src.
main: $(OBJ_FILES) | $(BIN_DIR)
	$(CC) -o $(BIN_DIR)/main $(OBJ_FILES) -g

# Execute the test_main.c file, compiling every other C file in src.
test: $(OBJ_FILES_NO_MAIN) $(OBJ_DIR)/test_main.o | $(BIN_DIR)
	$(CC) -o $(BIN_DIR)/test $(OBJ_FILES_NO_MAIN) $(OBJ_DIR)/test_main.o
	$(BIN_DIR)/test.exe

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) -c $< -o $@ $(CFLAGS)

$(OBJ_DIR)/test_main.o: $(TEST_DIR)/test_main.c | $(OBJ_DIR)
	$(CC) -c $< -o $@ $(CFLAGS)

clean:
	rm -f $(OBJ_DIR)/*.o $(BIN_DIR)/main $(BIN_DIR)/test
