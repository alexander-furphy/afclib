# Variables
CC = gcc
CFLAGS = -Wall -Wextra -g -Iinclude
SRC_DIR = src
OBJ_DIR = build
DIST_DIR = dist
TARGET = program
LIB_NAME = strlib
LIB_ARCHIVE_NAME = libstrlib.a
LIB_DIR = $(DIST_DIR)/$(LIB_NAME)

# Find all source files
SRCS := $(wildcard $(SRC_DIR)/*.c)
# Substitution: change src/main.c -> build/main.o
OBJS := $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
# Filter out the main object file from the full list
LIB_OBJS = $(filter-out $(OBJ_DIR)/main.o, $(OBJS))

# Main rule
all: $(TARGET) run

archive: $(LIB_OBJS)
	@mkdir -p $(LIB_DIR)
	@ar rcs $(LIB_DIR)/$(LIB_ARCHIVE_NAME) $(LIB_OBJS)
	@cp include/*.h $(LIB_DIR)/
	@cd $(LIB_DIR) && zip -r ../$(LIB_NAME).zip .
	@echo "Library created at $(LIB_DIR)"

# Run the target
run: $(TARGET)
	@./$(TARGET)

# The Linking Rule
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# The Compilation Rule - FOLDER CREATION HAPPENS HERE
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -rf $(OBJ_DIR) $(DIST_DIR) $(TARGET)

.PHONY: all clean run archive dist
