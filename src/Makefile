.PHONY: all chess network mcts selfplay versus clean

LIBTORCH_DIR ?= $(shell brew --prefix pytorch 2>/dev/null)

ifeq ($(LIBTORCH_DIR),)
    $(error "LIBTORCH_DIR is not set. Please specify it manually, e.g., 'make LIBTORCH_DIR=/path/to/libtorch'")
endif

export LIBTORCH_DIR

all: chess network mcts

chess:
	$(MAKE) -C chess

network:
	$(MAKE) -C network

mcts: chess
	$(MAKE) -C mcts

selfplay: chess network mcts
	$(MAKE) -C selfplay

versus: chess network mcts
	$(MAKE) -C selfplay versus

clean:
	$(MAKE) -C chess clean
	$(MAKE) -C network clean
	$(MAKE) -C mcts clean
	$(MAKE) -C selfplay clean
