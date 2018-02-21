CC = g++

CFLAGS = -std=c++0x -lpthread

all: main

main: main.cpp
	$(CC) `pkg-config --cflags opencv` main.cpp -o main `pkg-config --libs opencv` $(CFLAGS)

clean:
	rm -f main
