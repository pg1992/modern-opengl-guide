main: src/main.c
	gcc src/main.c -o bin/main glad/src/gl.c -Iglad/include -Iinclude -lGL -lglfw -ldl -lm

.PHONY: clean
clean:
	rm bin/main
