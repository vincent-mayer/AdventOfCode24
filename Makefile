
ROOT_DIR	:=$(shell pwd)
BUILD_DIR	:=$(ROOT_DIR)/$(DAY)/build
BIN_NAME	:=sol$(DAY)
BIN			:=$(BUILD_DIR)/$(BIN_NAME)
SRC			:=$(ROOT_DIR)/$(DAY)/Main.cpp

CXX_FLAGS :=-g -O0 -std=c++20

ifeq ($(DEBUG),NO)
	CXX_FLAGS :=-O3 -std=c++20
endif

$(BUILD_DIR):
	mkdir -p $@

$(BIN): $(SRC) | $(BUILD_DIR)
	clang++ $< $(CXX_FLAGS) -o $@

run: $(BIN)
	$<

.PHONY: run
