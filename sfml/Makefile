sfml-app: main.o
	g++ main.o -o sfml-app -lsfml-system -lsfml-window -lGL

main.o: main.cpp
	g++ -c main.cpp

.PHONY: clean
clean:
	rm sfml-app main.o
