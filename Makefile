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
	@if [ "$(OS)" = "Darwin" ]; then \
		echo "Signing with entitlements for macOS leaks tool..."; \
		for bin in $(BUILD_DIR)/*_tests; do \
			codesign -s - --entitlements entitlements.plist --force $$bin; \
		done; \
	fi

# Run Tests with Verbose Output
test: debug
ifeq ($(OS),Darwin)
	@echo "Running tests with macOS leaks tool..."
	ctest --test-dir $(BUILD_DIR) --output-on-failure -VV
else
	@echo "Running tests with AddressSanitizer..."
	ctest --test-dir $(BUILD_DIR) --output-on-failure
endif

# Create .zip and .tar.gz archives
dist: build
	cd $(BUILD_DIR) && cpack

install: dist
	sudo cmake --install build

uninstall:
	@if [ -f build/install_manifest.txt ]; then \
		echo "Uninstalling files..."; \
		sudo xargs rm < build/install_manifest.txt; \
		echo "Cleanup complete."; \
	else \
		echo "No install manifest found. Nothing to uninstall."; \
	fi

# Wipe the build directory
clean:
	rm -rf $(BUILD_DIR)

# Help Menu
help:
	@echo "afclib Build Shortcuts:"
	@echo "  make        		- Standard release build"
	@echo "  make debug  		- Build with debug symbols & sanitizers"
	@echo "  make test   		- Run all tests (shows leaks on Mac/Linux)"
	@echo "  make dist   		- Create library archives"
	@echo "  make install"		- Build and install the library
	@echo "  make uninstall"	- Removes all installed files from system
	@echo "  make clean  		- Remove all build artifacts"
