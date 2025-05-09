main: main.o
	gcc main.o -o main -lSDL2

main.o: main.c
	gcc -c main.c

.PHONY: clean
clean:
	rm main main.o
