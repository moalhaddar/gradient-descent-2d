BUILD_DIR := build
SRC_DIR := src

all: $(BUILD_DIR) | main

main: $(SRC_DIR)/main.c
	gcc -o $(BUILD_DIR)/main $(SRC_DIR)/main.c

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)