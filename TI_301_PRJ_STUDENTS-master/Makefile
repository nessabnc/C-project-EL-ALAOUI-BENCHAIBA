CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g -Iinclude
SRC = src/adjlist.c src/hasse.c src/matrix.c src/tarjan.c main.c


TARGET = markov

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET) *.o
