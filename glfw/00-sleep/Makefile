a.out: main.o
	g++ main.o -lglfw

main.o: main.cpp
	g++ -c main.cpp

.PHONY: clean
clean:
	rm a.out main.o
