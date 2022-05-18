GFLAGS = -std=c++20 -I./src
LDFLAGS = -lglfw -lvulkan

a.out: src/*.cpp src/*.h
	g++ ${CFLAGS} -o a.out src/*.cpp ${LDFLAGS}

.PHONY: test

test: a.out
	./a.out
