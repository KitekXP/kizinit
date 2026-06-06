build:
	$(CC) -Oz -flto -o kizinit kizinit.c

clean:
	rm kizinit
