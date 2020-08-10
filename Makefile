CC=gcc

CFLAGS=-c -Wall

all: task randombin

randombin: randombin.o
	$(CC) randombin.o -o randombin

randombin.o: src/randombin.c
	$(CC) $(CFLAGS) src/randombin.c

task: main.o aes.o crc32.o
	$(CC) main.o aes.o crc32.o -o task

main.o: src/main.c
	$(CC) $(CFLAGS) src/main.c

aes.o: src/aes.c
	$(CC) $(CFLAGS) src/aes.c

crc32.o: src/crc32.c
	$(CC) $(CFLAGS) src/crc32.c

clean:
	rm -rf *.o task randombin