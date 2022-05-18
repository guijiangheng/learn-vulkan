#include "pipeline.h"

#include <fstream>
#include <iostream>
#include <stdexcept>

namespace lve {

Pipeline::Pipeline(std::string vertFilepath, std::string fragFilepath) {
  create(vertFilepath, fragFilepath);
}

void Pipeline::create(std::string vertFilepath, std::string fragFilepath) {
  auto vertCode = readFile(vertFilepath);
  auto fragCode = readFile(fragFilepath);

  std::cout << "Vertex shader code size: " << vertCode.size() << std::endl;
  std::cout << "Fragment shader code size: " << fragCode.size() << std::endl;
}

std::vector<char> Pipeline::readFile(std::string filepath) {
  std::ifstream file{filepath, std::ios::ate | std::ios::binary};

  if (!file.is_open()) {
    throw std::runtime_error("Failed to open file: " + filepath);
  }

  size_t fileSize = (size_t)file.tellg();
  std::vector<char> buffer(fileSize);

  file.seekg(0);
  file.read(buffer.data(), fileSize);
  file.close();

  return buffer;
}

}  // namespace lve
