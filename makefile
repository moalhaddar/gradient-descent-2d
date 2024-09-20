BUILD_DIR := build
SRC_DIR := src

CFLAGS := $(shell pkg-config --cflags raylib) -I$(SRC_DIR)/external
LIBS := $(shell pkg-config --libs raylib) -lm

all: $(BUILD_DIR) | main

main: $(SRC_DIR)/main.c
	gcc $(CFLAGS) -o $(BUILD_DIR)/main $(SRC_DIR)/main.c $(LIBS)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)
