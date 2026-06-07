CC=klcc
CFLAGS=-static

build:
	$(CC) $(CFLAGS) -o kizinit kizinit.c

clean:
	rm kizinit
