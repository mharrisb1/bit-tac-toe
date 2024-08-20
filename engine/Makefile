# Compiler
CC = clang

# Compiler flags
CFLAGS = -Wall -Wextra -Iinclude -g -O0

# Directories
INCDIR = include
SRCDIR = src
TESTDIR = tests

# Source files
SRCS = $(SRCDIR)/game.c

# Test files
TEST_SRCS = $(TESTDIR)/test_game.c

# Object files
OBJS = $(SRCS:.c=.o)
TEST_OBJS = $(TEST_SRCS:.c=.o)

# Executables
ENGINE_EXE = game.out
TEST_EXE = test_game.out

# Default target
.PHONY: all
all: $(ENGINE_EXE)

# Build engine executable
$(ENGINE_EXE): $(OBJS)
	$(CC) -o $@ $^

# Build test executable
$(TEST_EXE): $(TEST_OBJS) $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)

# Compile source files
$(SRCDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Compile test files
$(TESTDIR)/%.o: $(TESTDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Run tests
.PHONY: test
test: $(TEST_EXE)
	./$(TEST_EXE)

# Clean up
.PHONY: clean
clean:
	rm -f $(SRCDIR)/*.o $(TESTDIR)/*.o $(ENGINE_EXE) $(TEST_EXE)
