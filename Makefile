BIN_NAME := sol$(DAY)

CXX_FLAGS :=-g -O0 -std=c++20

ifeq ($(DEBUG),NO)
	CXX_FLAGS :=-O3 -std=c++20
endif

SRC := $(DAY)/Main.cpp

$(DAY)/$(BIN_NAME): $(SRC)
	clang++ $< $(CXX_FLAGS) -o $@

run: $(DAY)/$(BIN_NAME)
	./$<

.PHONY: run
