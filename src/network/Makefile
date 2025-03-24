CC = g++
CFLAGS = -arch arm64 -Wall

CFLAGS += -std=c++23
CFLAGS += -target arm64-apple-macos -march=armv8.2-a
CFLAGS += -O3 -pthread 

#LIBTORCH_DIR = /opt/homebrew/Cellar/pytorch/2.5.1_1

LDFLAGS = -Wl,-rpath,$(LIBTORCH_DIR)/lib
CFLAGS += -I$(LIBTORCH_DIR)/include -I$(LIBTORCH_DIR)/include/torch/csrc/api/include
LDFLAGS += -L$(LIBTORCH_DIR)/lib -ltorch -ltorch_cpu -lc10

OBJS = network.o
LIB = netlib.a

all: $(LIB)

$(LIB): $(OBJS)
	ar rcs $(LIB) $(OBJS)

network.o: network.cpp network.h
	$(CC) $(CFLAGS) -c network.cpp -o network.o

clean:
	rm -f $(OBJS) $(LIB)
