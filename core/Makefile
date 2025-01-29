CC = gcc
CFLAGS = -Wall -Wextra -Iinclude

SRC_DIR = src
TEST_DIR = tests
OBJ_DIR = obj
BIN_DIR = bin

SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC_FILES))
OBJ_FILES_NO_MAIN = $(filter-out $(OBJ_DIR)/main.o, $(OBJ_FILES))

all: main

# Execute the main.c file, compiling every other C files in src.
main: $(OBJ_FILES)
	$(CC) -o $(BIN_DIR)/main $(OBJ_FILES); bin/main.exe

# Execute the test_main.c file, compiling every other C files in src. Note: We might need to compile every file in the tests folder, in case we need other files (e.g. for testing function definition)
test: $(OBJ_FILES_NO_MAIN) $(OBJ_DIR)/test_main.o
	$(CC) -o $(BIN_DIR)/test $(OBJ_FILES_NO_MAIN) $(OBJ_DIR)/test_main.o; bin/test.exe

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) -c $< -o $@ $(CFLAGS)

$(OBJ_DIR)/test_main.o: $(TEST_DIR)/test_main.c
	$(CC) -c $< -o $@ $(CFLAGS)

clean:
	rm -f $(OBJ_DIR)/*.o $(BIN_DIR)/main $(BIN_DIR)/test
