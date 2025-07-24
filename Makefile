# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -Iinclude
LDFLAGS = -lm

# Source and build directories
SRC_DIR = src
BUILD_DIR = build

# Source files
SRCS = $(SRC_DIR)/main.c \
       $(SRC_DIR)/user.c \
       $(SRC_DIR)/student.c \
       $(SRC_DIR)/input_utils.c \
       $(SRC_DIR)/logger.c \
       $(SRC_DIR)/system_utils.c \
       $(SRC_DIR)/panels.c \
       $(SRC_DIR)/exam.c
OBJS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))

# Executable name
TARGET = ems

# Create build directory if it doesn't exist
$(shell mkdir -p $(BUILD_DIR))

# Default target
all: $(TARGET)

# Link the executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Compile source files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build files
clean:
	rm -rf $(BUILD_DIR) $(TARGET)

# Run the program
run: $(TARGET)
	./$(TARGET)

# Create necessary directories
init:
	mkdir -p data backups reports

.PHONY: all clean run init
