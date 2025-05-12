main: main.o
	g++ main.o -o main -lsfml-system -lsfml-window -lGL

main.o: main.cpp
	g++ -c main.cpp

.PHONY: clean
clean:
	rm main main.o
