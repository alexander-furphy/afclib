# --- OS Detection and Tool Setup ---
ifeq ($(OS),Windows_NT)
    # Windows Settings
    # Use 'cmd /c' to ensure we use standard Windows shell commands
    RM = cmd /C del /Q /S
    # Windows 'mkdir' creates parent directories by default if extensions are on
    MKDIR = cmd /C if not exist $(subst /,\,$(1)) mkdir $(subst /,\,$(1))
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
LIB_NAME = strlib
LIB_ARCHIVE_NAME = libstrlib.a
LIB_DIR = $(DIST_DIR)/$(LIB_NAME)

LIB_SRC_FILES = src/string_lib.c src/string_wrappers.c
DIST_HEADERS = include/string_lib.h include/string_wrappers.h

# Automatically generate the object paths for these library files
LIB_OBJS = $(LIB_SRC_FILES:src/%.c=$(OBJ_DIR)/%.o)

# Keep your "Main" or "Test" file separate
APP_SRC = src/main.c src/string_tests.c
APP_OBJS = $(APP_SRC:src/%.c=$(OBJ_DIR)/%.o)

# --- Rules ---
all: $(TARGET) run

archive: $(LIB_OBJS)
	@$(call MKDIR, $(LIB_DIR))
	@ar rcs $(LIB_DIR)/$(LIB_ARCHIVE_NAME) $(LIB_OBJS)
	@$(CP) $(call FIX_PATH, $(DIST_HEADERS)) $(call FIX_PATH, $(LIB_DIR)/)
	@$(call ZIP, $(LIB_DIR), $(DIST_DIR)/$(LIB_NAME).zip)
	@echo Library created at $(LIB_DIR)

# Run the target
run: $(TARGET)
	@./$(TARGET)

# Run with leaks
debug: $(TARGET)
	@leaks -quiet -atExit -- ./$(TARGET)

# The Linking Rule
$(TARGET): $(APP_OBJS) $(LIB_OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Rule for all library files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
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
