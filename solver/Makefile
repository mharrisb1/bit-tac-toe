# Compiler
CC = clang

# Compiler flags
CFLAGS = -Wall -Wextra -Iinclude -I../engine/include -g -O0

# Directories
INCDIR = include
SRCDIR = src
ENGINEDIR = ../engine/src
TESTDIR = tests

# Source files
SRCS = $(SRCDIR)/solver.c

# Engine source files
ENGINE_SRCS = $(ENGINEDIR)/game.c

# Test files
TEST_SRCS = $(TESTDIR)/test_solver.c

# Object files
OBJS = $(SRCS:.c=.o) $(ENGINE_SRCS:.c=.o)
TEST_OBJS = $(TEST_SRCS:.c=.o)

# Executables
SOLVER_EXE = solver.out
TEST_EXE = test_solver.out

# Default target
.PHONY: all
all: $(SOLVER_EXE)

# Build solver executable
$(SOLVER_EXE): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Build test executable
$(TEST_EXE): $(TEST_OBJS) $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Compile source files
$(SRCDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Compile engine files
$(ENGINEDIR)/%.o: $(ENGINEDIR)/%.c
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
	rm -f $(SRCDIR)/*.o $(ENGINEDIR)/*.o $(TESTDIR)/*.o $(SOLVER_EXE) $(TEST_EXE)
