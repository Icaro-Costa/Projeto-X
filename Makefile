CC = gcc
CFLAGS = -Wall -g
INCLUDE_DIR = include
BUILD_DIR = build
SRC_DIR = src
TARGET = $(BUILD_DIR)/testes # Nome do executável final

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
INCLUDES = -I$(INCLUDE_DIR)
all: $(TARGET)

$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

$(TARGET): $(OBJS) $(BUILD_DIR)
	@echo "Linking $@"
	$(CC) $(OBJS) -o $@ -lm #
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c $(BUILD_DIR)
	@echo "Compiling $<"
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	@echo "Cleaning build directory..."
	rm -rf $(BUILD_DIR)/*
	@echo "Build directory cleaned."

.PHONY: all clean $(BUILD_DIR)
