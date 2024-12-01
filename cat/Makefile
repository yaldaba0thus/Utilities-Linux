CC = clang
SRC = src/cat.c
OUT_DIR_BUILD = build
OUT_DIR_RELEASE = release
OUT_NAME = cat

CFLAGS = -Wall -Wextra
CFLAGS_DEBUG = $(CFLAGS) -g -O0
CFLAGS_RELEASE = $(CFLAGS) -O2 -DNDEBUG

.PHONY: all build release debug tests clean

all: build

# Build (default target)
build: $(OUT_DIR_BUILD)/$(OUT_NAME)

$(OUT_DIR_BUILD)/$(OUT_NAME): $(SRC)
	mkdir -p $(OUT_DIR_BUILD)
	$(CC) $(CFLAGS) -o $@ $^

# Release build
release: $(OUT_DIR_RELEASE)/$(OUT_NAME)

$(OUT_DIR_RELEASE)/$(OUT_NAME): $(SRC)
	mkdir -p $(OUT_DIR_RELEASE)
	$(CC) $(CFLAGS_RELEASE) -o $@ $^

# Debug build
debug: $(OUT_DIR_BUILD)/$(OUT_NAME)_debug

$(OUT_DIR_BUILD)/$(OUT_NAME)_debug: $(SRC)
	mkdir -p $(OUT_DIR_BUILD)
	$(CC) $(CFLAGS_DEBUG) -o $@ $^

# Run tests
tests: ./tests/run.sh
	./tests/run.sh

./tests/run.sh:
	chmod +x ./tests/run.sh || true

# Clean up
clean:
	rm -rf $(OUT_DIR_BUILD) $(OUT_DIR_RELEASE)
