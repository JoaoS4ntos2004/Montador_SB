CC = gcc
CFLAGS = -Wall -Wextra -std=c99
SRC = src/main.c src/preprocessor.c src/assembler.c src/simulator.c
TARGET = montador

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET) *.obj *.pre *.pen