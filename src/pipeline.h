#pragma once

#include <string>
#include <vector>

#include "device.h"

namespace lve {

struct PipelineConfig {};

class Pipeline {
 public:
  Pipeline(Device& device,
           std::string vertFilepath,
           std::string fragFilepath,
           const PipelineConfig& config);

  ~Pipeline() {}

  Pipeline(const Pipeline&) = delete;
  Pipeline& operator=(const Pipeline&) = delete;

  static PipelineConfig makeDefaultPipelineConfig(uint32_t width,
                                                  uint32_t height);

 private:
  static std::vector<char> readFile(std::string filepath);

  void createGraphicsPipeline(std::string vertFilepath,
                              std::string fragFilepath,
                              const PipelineConfig& config);

  void createShaderModule(const std::vector<char>& code,
                          VkShaderModule& shaderModule);

  Device& device;
  VkPipeline graphicsPipeline;
  VkShaderModule vertShaderModule;
  VkShaderModule fragShaderModule;
};

}  // namespace lve
