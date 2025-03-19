# chess

In-progress chess engine implemented in C++ using Monte Carlo Tree Search (MCTS) inspired by the AlphaGo and AlphaZero training frameworks. The engine combines MCTS with a neural network for position evaluation and policy generation.

The network produces:
- A valuation of the current position via the value head
- An estimation of move priors via the policy head
  
Both of which are used during search to probe the game tree and prune suboptimal moves.

***
# Structure
```\n.\n
|-chess				#	A shabby chess implementation using bitboards
|-network			#	Where the magic happens
|-mcts				#	Monte Carlo tree search
|-selfplay			#	Multithreaded data generation, currently being heavily refactored
```
***

Todo:
- Rewrite threaded selfplay, separate threads for preprocessing and inference dispatching
- Condense training data
- LRU cache instead of dynamic node allocation
- Bigger net
- Add promotions, castling, and en passant

Specifications:
- C++17 or higher
- Libtorch 2.5.1
- Gnuplot (optional)
