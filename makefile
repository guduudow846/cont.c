CC = gcc
CFLAGS = -std=c11 -Wall -Wextra
SRC = main.c
OUT = door_admin

all:
	$(CC) $(CFLAGS) $(SRC) -o $(OUT)

clean:
	del $(OUT).exe 2> NUL
