#!/usr/bin/env sh

cd src/shaders
glslc simple_shader.vert -o simple_shader.vert.spv
glslc simple_shader.frag -o simple_shader.frag.spv
