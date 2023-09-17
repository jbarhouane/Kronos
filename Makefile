CC = g++
CFLAGS = --std=c++17 -Wall -Werror -pedantic

all: Kronos lint

lint:
	cpplint --recursive --quiet src/
Kronos: Kronos.o main.o
	$(CC) $(CFLAGS) -o Kronos Kronos.o main.o
RandWriter.o: Kronos.cpp Kronos.hpp
	$(CC) $(CFLAGS) -c Kronos.cpp
main.o: main.cpp Kronos.hpp
	$(CC) $(CFLAGS) -c main.cpp
clean:
	rm *.o Kronos





