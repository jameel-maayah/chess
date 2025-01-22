CC = g++
CFLAGS = -arch arm64 -Wall -std=c++23 -target arm64-apple-macos -march=armv8.2-a -O3 -pthread

CHESS_DIR = ../chess
CHESS_LIB = $(CHESS_DIR)/chesslib.a
CHESS_INCLUDE = -I$(CHESS_DIR)

MCTS_DIR = ../mcts
MCTS_LIB = $(MCTS_DIR)/mcts.a
MCTS_INCLUDE = -I$(MCTS_DIR)

NN_DIR = ../network
NN_LIB = $(NN_DIR)/network.a
NN_INCLUDE = -I$(NN_DIR)

LDFLAGS = $(CHESS_LIB) $(MCTS_LIB) $(NN_LIB)

EXEC = train

$(EXEC): montecarlo.o node.o tree.o
	$(CC) $(CFLAGS) -o $(EXEC) montecarlo.o node.o tree.o $(LDFLAGS)

train.o: train.cpp

clean:
	rm -f *.o $(EXEC)
