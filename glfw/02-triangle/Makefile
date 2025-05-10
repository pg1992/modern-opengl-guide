a.out: main.o
	g++ main.o `pkg-config --libs glew` -lglfw

main.o: main.cpp
	g++ -c -DGLEW_STATIC main.cpp

.PHONY: clean
clean:
	rm a.out main.o
