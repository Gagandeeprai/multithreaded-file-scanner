# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Iinclude

# Linker flags
LDFLAGS = -lpthread -lncurses

# Directories
SRC_DIR = src
OBJ_DIR = build

# Source files
SRCS = \
	$(SRC_DIR)/main.c \
	$(SRC_DIR)/scanner.c \
	$(SRC_DIR)/ui.c \
	$(SRC_DIR)/menu.c \
	$(SRC_DIR)/browser.c

# Object files
OBJS = \
	$(OBJ_DIR)/main.o \
	$(OBJ_DIR)/scanner.o \
	$(OBJ_DIR)/ui.o \
	$(OBJ_DIR)/menu.o \
	$(OBJ_DIR)/browser.o

# Output binary
TARGET = multithreaded-file-scanner

# Default target
all: $(TARGET)

# Link executable
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

# Compile main.c
$(OBJ_DIR)/main.o: $(SRC_DIR)/main.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Compile scanner.c
$(OBJ_DIR)/scanner.o: $(SRC_DIR)/scanner.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Compile ui.c
$(OBJ_DIR)/ui.o: $(SRC_DIR)/ui.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Compile menu.c
$(OBJ_DIR)/menu.o: $(SRC_DIR)/menu.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Compile browser.c  ✅ THIS WAS MISSING
$(OBJ_DIR)/browser.o: $(SRC_DIR)/browser.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Create build directory
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Clean build files
clean:
	rm -rf $(OBJ_DIR) $(TARGET)

# Rebuild everything
rebuild: clean all
