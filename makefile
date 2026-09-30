CC = gcc
CFLAGS = -std=c11 -Wall -g
SRC = door_admin.c safeinput.c
OUT = door_admin.exe

all:
	$(CC) $(CFLAGS) $(SRC) -o $(OUT)

clean:
	rm -f $(OUT)