CC = g++
CFLAGS = -arch arm64 -Wall -std=c++23 -target arm64-apple-macos -march=armv8.2-a -O3 -pthread -flto

CHESS_DIR = ../chess
CHESS_LIB = $(CHESS_DIR)/chesslib.a
CHESS_INCLUDE = -I$(CHESS_DIR)

MCTS_DIR = ../mcts
MCTS_LIB = $(MCTS_DIR)/mctslib.a
MCTS_INCLUDE = -I$(MCTS_DIR)

NN_DIR = ../network
NN_LIB = $(NN_DIR)/netlib.a
NN_INCLUDE = -I$(NN_DIR)

LDFLAGS = $(CHESS_LIB) $(MCTS_LIB) $(NN_LIB)
INCLUDES = $(CHESS_INCLUDE) $(MCTS_INCLUDE) $(NN_INCLUDE)

LIBTORCH_DIR = /opt/homebrew/Cellar/pytorch/2.5.1_1

LDFLAGS += -L$(LIBTORCH_DIR)/lib -ltorch -ltorch_cpu -lc10
CFLAGS += -I$(LIBTORCH_DIR)/include -I$(LIBTORCH_DIR)/include/torch/csrc/api/include


EXEC = selfplay
SRCS = train.cpp
OBJS = $(SRCS:.cpp=.o)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $@

%.o: %.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f *.o $(EXEC)
