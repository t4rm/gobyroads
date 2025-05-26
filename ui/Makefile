# OS : WINDOWS

# Compilateur et flags
CC        = gcc
CFLAGS    = -Wall -std=gnu11 -pedantic -O3 -g -fno-omit-frame-pointer \
            -Iinclude/ -I../core/include
LDFLAGS   = -Llib -lSDL2 -lSDL2_image -lSDL2_ttf

# Répertoires UI
SRC_DIR   = src
OBJ_DIR   = obj
BIN_DIR   = bin
TEST_DIR  = tests

# Répertoires Core
CORE_DIR       = ../core
CORE_SRC_DIR   = $(CORE_DIR)/src
CORE_OBJ_DIR   = $(CORE_DIR)/obj
CORE_TEST_DIR  = $(CORE_DIR)/tests

# Fichiers UI
SRC_FILES      = $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES      = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC_FILES))
OBJ_NO_MAIN    = $(filter-out $(OBJ_DIR)/main.o,$(OBJ_FILES))

# Fichiers Core (exclut le main.c de Core)
CORE_ALL_SRCS  = $(wildcard $(CORE_SRC_DIR)/*.c)
CORE_APP_SRCS  = $(filter-out $(CORE_SRC_DIR)/main.c,$(CORE_ALL_SRCS))
CORE_OBJ_FILES = $(patsubst $(CORE_SRC_DIR)/%.c,$(CORE_OBJ_DIR)/%.o,$(CORE_APP_SRCS))

# Création des dossiers si besoin
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(CORE_OBJ_DIR):
	mkdir -p $(CORE_OBJ_DIR)

# Compilation des .c UI → .o UI
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Compilation des .c Core → .o Core (hors main.c)
$(CORE_OBJ_DIR)/%.o: $(CORE_SRC_DIR)/%.c | $(CORE_OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Construction de l’exécutable UI
main: $(OBJ_FILES) $(CORE_OBJ_FILES) | $(BIN_DIR)
	$(CC) -o $(BIN_DIR)/main.exe \
	    $(OBJ_FILES) $(CORE_OBJ_FILES) $(LDFLAGS)

# Raccourci pour lancer
run: main
	$(BIN_DIR)/main.exe

# Debugger
debug: main
	gdb $(BIN_DIR)/main.exe

# Construction des tests UI si besoin
test: $(OBJ_NO_MAIN) $(OBJ_DIR)/test_main.o $(CORE_OBJ_FILES) | $(BIN_DIR)
	$(CC) -o $(BIN_DIR)/test.exe \
	    $(OBJ_NO_MAIN) $(OBJ_DIR)/test_main.o $(CORE_OBJ_FILES) $(LDFLAGS)
	$(BIN_DIR)/test.exe

$(OBJ_DIR)/test_main.o: $(TEST_DIR)/test_main.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Nettoyage
clean:
	rm -f $(OBJ_DIR)/*.o $(CORE_OBJ_DIR)/*.o $(BIN_DIR)/main.exe $(BIN_DIR)/test.exe
