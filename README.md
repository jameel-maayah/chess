# chess

In-progress chess engine implemented in C++ using Monte Carlo Tree Search (MCTS) inspired by the AlphaGo and AlphaZero training frameworks. The engine combines MCTS with a neural network for position evaluation and policy prediction.

The network produces:
- A valuation of the current position via the value head
- An estimation of move priors via the policy head
  
Both of which are used during search to probe the game tree.

***
# Structure
```\n.\n
|-chess				#	A shabby chess implementation using bitboards
|-network			#	Where the magic happens
|-mcts				#	Monte Carlo tree search
|-selfplay			#	Multithreaded data generation, currently being heavily refactored
```
***
# Installation

This project requires ```Make``` and ```g++```, as well as LibTorch 2.5.1, which can be installed here: https://pytorch.org/get-started/locally/

And of course, clone this repository:
```\n.\n
git clone https://github.com/jameel-maayah/chess.git
cd /src
```

As the training script is still being reworked, I have not yet pushed it to this repository. However, I have included a file of (somewhat) satisfactory model weights I have produced with it, located in the ```/weights``` folder. To try out the engine, run the following from the ```/src``` directory:

```\n.\n
make versus
cd selfplay
./versus weights/pv_6b96.pt [black/white] [cpu_num_iterations]
```

If ```make``` is unable to find the location of your LibTorch installation, specify it manually:

```\n.\n
make LIBTORCH_DIR=path_to_libtorch versus 
```
Todo:
- Rewrite threaded selfplay, separate threads for preprocessing and inference dispatching
- Condense training data
- LRU cache instead of dynamic node allocation
- Bigger net
- Add promotions, castling, and en passant

Specifications:
- C++23 or higher
- Libtorch 2.5.1
- Gnuplot (optional)
