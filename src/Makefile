.PHONY: all chess network mcts clean

all: chess network mcts

chess:
	$(MAKE) -C chess

network:
	$(MAKE) -C network

mcts: chess
	$(MAKE) -C mcts

clean:
	$(MAKE) -C chess clean
	$(MAKE) -C network clean
	$(MAKE) -C mcts clean
