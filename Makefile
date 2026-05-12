CC = gcc
CFLAGS = -Wall -Wextra -std=c99
SRC = src/main.c src/preprocessador.c src/assembler.c src/simulador.c
TARGET = montador

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET) tests/*.obj tests/*.pre tests/*.pen