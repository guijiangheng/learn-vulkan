#include "pipeline.h"

#include <fstream>
#include <iostream>
#include <stdexcept>

namespace lve {

Pipeline::Pipeline(Device& device,
                   std::string vertFilepath,
                   std::string fragFilepath,
                   const PipelineConfig& config)
    : device{device} {
  createGraphicsPipeline(vertFilepath, fragFilepath, config);
}

void Pipeline::createGraphicsPipeline(std::string vertFilepath,
                                      std::string fragFilepath,
                                      const PipelineConfig& config) {
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

void Pipeline::createShaderModule(const std::vector<char>& code,
                                  VkShaderModule& shaderModule) {
  VkShaderModuleCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
  createInfo.codeSize = code.size();
  createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

  if (vkCreateShaderModule(
          device.device(), &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
    throw std::runtime_error("failed to create shader module");
  }
}

PipelineConfig Pipeline::makeDefaultPipelineConfig(uint32_t width,
                                                   uint32_t height) {
  return {};
}

}  // namespace lve
