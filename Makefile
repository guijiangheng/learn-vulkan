GFLAGS = -std=c++20 -I./src
LDFLAGS = -lglfw -lvulkan

vertSource = $(shell find ./src/shaders -type f -name "*.vert")
vertObjFiles = $(pathsubst %.vert, $.vert.spv, $(vertSource))
fragSource = $(shell find ./src/shaders -type f -name "*.frag")
fragObjFiles = $(pathsubst %.frag, $.frag.spv, $(fragSource))

TARGET = a.out
$(TARGET): $(vertObjFiles) $(fragObjFiles)
$(TARGET): src/*.cpp src/*.h
	g++ ${CFLAGS} -o ${TARGET} src/*.cpp ${LDFLAGS}

*.spv: %
	glslc $< -o $@

.PHONY: test clean

test: a.out
	./a.out

clean:
	rm -f a.out
	rm -f *.spv
