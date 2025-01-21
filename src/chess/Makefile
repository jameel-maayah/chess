CC = g++
CFLAGS = -arch arm64 -Wall -std=c++23 -target arm64-apple-macos -march=armv8.2-a -O3 -pthread

OBJS = chess.o bitboard.o movegen.o
LIB = chesslib.a

all: $(LIB)

$(LIB): $(OBJS)
	ar rcs $(LIB) $(OBJS)

chess.o: chess.cpp chess.h move.h piece.h attacks.h constants.h types.h bitboard.h
	$(CC) $(CFLAGS) -c chess.cpp -o chess.o

bitboard.o: bitboard.cpp bitboard.h
	$(CC) $(CFLAGS) -c bitboard.cpp -o bitboard.o

movegen.o: movegen.cpp movegen.h bitboard.h
	$(CC) $(CFLAGS) -c movegen.cpp -o movegen.o

clean:
	rm -f $(OBJS) $(LIB)
