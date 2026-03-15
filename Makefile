#
# Convenience wrapper for MacOS/Linux users
#

# --- Settings ---
BUILD_DIR := build
# Detect OS for specific test commands
OS := $(shell uname -s)

.PHONY: all build debug test clean dist help

# Default target: standard build
all: build

# Standard Build
build:
	cmake -B $(BUILD_DIR) -DCMAKE_BUILD_TYPE=Release
	cmake --build $(BUILD_DIR)

# Debug Build (Enables your ASan/Leak logic)
debug:
	cmake -B $(BUILD_DIR) -DCMAKE_BUILD_TYPE=Debug
	cmake --build $(BUILD_DIR)

# Run Tests with Verbose Output
test: debug
ifeq ($(OS),Darwin)
	@echo "Running tests with macOS leaks tool..."
	ctest --test-dir $(BUILD_DIR) -VV
else
	@echo "Running tests with AddressSanitizer..."
	ctest --test-dir $(BUILD_DIR) --output-on-failure
endif

# Create .zip and .tar.gz archives
dist: build
	cd $(BUILD_DIR) && cpack

# Wipe the build directory
clean:
	rm -rf $(BUILD_DIR)

# Help Menu
help:
	@echo "afclib Build Shortcuts:"
	@echo "  make        - Standard release build"
	@echo "  make debug  - Build with debug symbols & sanitizers"
	@echo "  make test   - Run all tests (shows leaks on Mac/Linux)"
	@echo "  make dist   - Create library archives"
	@echo "  make clean  - Remove all build artifacts"
