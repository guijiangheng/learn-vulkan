GFLAGS = -std=c++20 -I./src
LDFLAGS = -lglfw -lvulkan

vertSources = $(shell find ./src/shaders -type f -name "*.vert")
vertObjFiles = $(patsubst %.vert, %.vert.spv, $(vertSources))
fragSources = $(shell find ./src/shaders -type f -name "*.frag")
fragObjFiles = $(patsubst %.frag, %.frag.spv, $(fragSources))

TARGET = a.out
$(TARGET): $(vertObjFiles) $(fragObjFiles)
$(TARGET): src/*.cpp src/*.h
	g++ ${CFLAGS} -o ${TARGET} src/*.cpp ${LDFLAGS}

%.spv: %
	glslc $< -o $@

.PHONY: test clean

test: a.out
	./a.out

clean:
	rm -f a.out
	find . -name \*.spv -type f -delete
