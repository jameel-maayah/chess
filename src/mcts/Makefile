CC = g++
CFLAGS = -arch arm64 -Wall -std=c++23 -target arm64-apple-macos -march=armv8.2-a -O3 -pthread

CHESS_DIR = ../chess
CHESS_LIB = $(CHESS_DIR)/chesslib.a
CHESS_INCLUDE = -I$(CHESS_DIR)

LDFLAGS = $(CHESS_LIB)

EXEC = montecarlo

$(EXEC): montecarlo.o node.o tree.o
	$(CC) $(CFLAGS) -o $(EXEC) montecarlo.o node.o tree.o $(LDFLAGS)

montecarlo.o: montecarlo.cpp montecarlo.h tree.h node.h
	$(CC) $(CFLAGS) $(CHESS_INCLUDE) -c montecarlo.cpp -o montecarlo.o

node.o: node.cpp node.h
	$(CC) $(CFLAGS) $(CHESS_INCLUDE) -c node.cpp -o node.o
tree.o: tree.cpp tree.h
	$(CC) $(CFLAGS) $(CHESS_INCLUDE) -c tree.cpp -o tree.o

clean:
	rm -f *.o $(EXEC)
