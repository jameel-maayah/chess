CC = g++
CFLAGS = -arch arm64 -Wall #-g -fsanitize=address -fno-omit-frame-pointer -Wimplicit-function-declaration -Wno-int-conversion \

CFLAGS += -std=c++20
CFLAGS += -target arm64-apple-macos -march=armv8.2-a
CFLAGS += -O3 -pthread 

EXEC = chess

$(EXEC): main.o chess.o bitboard.o
	$(CC) $(CFLAGS) -o $(EXEC) main.o chess.o bitboard.o $(LDFLAGS)
main.o : main.cpp chess.h
	$(CC) $(CFLAGS) -c main.cpp -o main.o

chess.o: chess.cpp chess.h move.h piece.h constants.h types.h bitboard.h
	$(CC) $(CFLAGS) -c chess.cpp -o chess.o

bitboard.o: bitboard.cpp bitboard.h
	$(CC) $(CFLAGS) -c bitboard.cpp -o bitboard.o


clean:
	rm -f *.o $(EXEC)
