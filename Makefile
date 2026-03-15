# --- OS Detection and Tool Setup ---
ifeq ($(OS),Windows_NT)
    # Windows Settings
    # Use 'cmd /c' to ensure we use standard Windows shell commands
    RM = cmd /C del /Q /S "$(1)"
    # Windows 'mkdir' creates parent directories by default if extensions are on
    MKDIR = cmd /C if not exist "$(subst /,\,$(1))" mkdir "$(subst /,\,$(1))"
    CP = copy /Y
    TARGET_EXT = .exe
    # Zip is not native to Windows CMD; we use a PowerShell wrapper
    ZIP = powershell -Command "Compress-Archive -Path $(1)/* -DestinationPath $(2) -Force"
    FIX_PATH = $(subst /,\,$1)
else
    # Unix/Mac Settings
    RM = rm -rf
    MKDIR = mkdir -p $(1)
    CP = cp
    TARGET_EXT = 
    ZIP = cd $(1) && zip -r ../$(notdir $(2)) .
    FIX_PATH = $1
endif

# --- Variables ---
CC = gcc
CFLAGS = -Wall -Wextra -g -Iinclude -g
SRC_DIR = src
OBJ_DIR = build
DIST_DIR = dist
BASE_TARGET = program
TARGET = $(BASE_TARGET)$(TARGET_EXT)
LIB_NAME = afclib
LIB_ARCHIVE_NAME = libafclib.a
LIB_DIR = $(DIST_DIR)/$(LIB_NAME)

LIB_SRC_FILES = src/af_string.c
DIST_HEADERS = include/af_string.h

LIB_OBJS = $(LIB_SRC_FILES:src/%.c=$(OBJ_DIR)/%.o)

APP_SRC = tests/af_string_tests.c
APP_OBJS = $(APP_SRC:tests/%.c=$(OBJ_DIR)/%.o)

# --- Rules ---
all: $(TARGET) run

archive: $(LIB_OBJS)
	@$(call MKDIR, $(LIB_DIR))
	@$(call MKDIR, $(LIB_DIR)/include)
	@ar rcs $(LIB_DIR)/$(LIB_ARCHIVE_NAME) $(LIB_OBJS)
	@$(CP) $(call FIX_PATH, $(DIST_HEADERS)) $(call FIX_PATH, $(LIB_DIR)/include/)
	@$(call ZIP, $(LIB_DIR), $(DIST_DIR)/$(LIB_NAME).zip)
	@echo Library created at $(LIB_DIR)

install: archive
ifeq (($OS),Windows_NT)
	@echo "Manual install on Windows: copy headers and .a to your preferred location"
else
	@sudo mkdir -p /usr/local/lib
	@sudo mkdir -p /usr/local/include
	@sudo cp $(LIB_DIR)/libafclib.a /usr/local/lib/
	@sudo cp $(LIB_DIR)/include/*.h /usr/local/include/
	@echo "Library installed to /usr/local/lib and headers to /usr/local/include"
endif

# Run the target
run: $(TARGET)
ifeq ($(OS),Windows_NT)
	@$(TARGET)
else
	@./$(TARGET)
endif

# Run with leaks
debug: $(TARGET)
ifeq ($(OS),Darwin)
	@leaks -quiet -atExit -- ./$(TARGET)
else
	@valgrind ./$(TARGET)
endif

# The Linking Rule
$(TARGET): $(APP_OBJS) $(LIB_OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Rule for all library files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@$(call MKDIR, $(dir $@))
	$(CC) $(CFLAGS) -c $< -o $@

# Rule for test files
$(OBJ_DIR)/%.o: tests/%.c
	@$(call MKDIR, $(dir $@))
	$(CC) $(CFLAGS) -c $< -o $@

clean:
ifeq ($(OS),Windows_NT)
	@if exist $(OBJ_DIR) $(RM) $(OBJ_DIR) >nul 2>&1
	@if exist $(DIST_DIR) $(RM) $(DIST_DIR) >nul 2>&1
	@if exist $(TARGET) del /Q $(TARGET) >nul 2>&1
else
	@$(RM) $(OBJ_DIR) $(DIST_DIR) $(TARGET)
endif

.PHONY: all clean run archive debug
