GFLAGS = -std=c++20 -I./src
LDFLAGS = -lglfw -lvulkan

a.out: src/*.cpp
	g++ ${CFLAGS} -o a.out src/*.cpp ${LDFLAGS}

.PHONY: test

test: a.out
	./a.out
