CC = g++
CFLAGS = -arch arm64 -Wall -std=c++23 -target arm64-apple-macos -march=armv8.2-a -O3 -pthread

OBJS = chess.o bitboard.o movegen.o zobrist.o
LIB = chesslib.a
EXEC = chess

all: $(LIB)

$(LIB): $(OBJS)
	ar rcs $(LIB) $(OBJS)

$(EXEC): main.o chess.o bitboard.o movegen.o zobrist.o
	$(CC) $(CFLAGS) -o $(EXEC) main.o chess.o bitboard.o movegen.o zobrist.o $(LDFLAGS) 

main.o: main.cpp chess.h
	$(CC) $(CFLAGS) -c main.cpp -o main.o

chess.o: chess.cpp chess.h move.h piece.h attacks.h constants.h types.h bitboard.h zobrist.h
	$(CC) $(CFLAGS) -c chess.cpp -o chess.o

bitboard.o: bitboard.cpp bitboard.h
	$(CC) $(CFLAGS) -c bitboard.cpp -o bitboard.o

movegen.o: movegen.cpp movegen.h bitboard.h
	$(CC) $(CFLAGS) -c movegen.cpp -o movegen.o

zobrist.o: zobrist.cpp zobrist.h chess.h move.h piece.h
	$(CC) $(CFLAGS) -c zobrist.cpp -o zobrist.o

clean:
	rm -f $(OBJS) $(LIB) $(EXEC) main.o
