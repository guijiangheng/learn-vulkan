#pragma once

#include <string>
#include <vector>

#include "device.h"

namespace lve {

struct PipelineConfigInfo {
  VkViewport viewport;
  VkRect2D scissor;
  VkPipelineViewportStateCreateInfo viewportInfo;
  VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
  VkPipelineRasterizationStateCreateInfo rasterizationInfo;
  VkPipelineMultisampleStateCreateInfo multisampleInfo;
  VkPipelineColorBlendAttachmentState colorBlendAttachment;
  VkPipelineColorBlendStateCreateInfo colorBlendInfo;
  VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
  VkPipelineLayout pipelineLayout = nullptr;
  VkRenderPass renderPass = nullptr;
  uint32_t subpass = 0;
};

class Pipeline {
 public:
  Pipeline(Device& device,
           std::string vertFilepath,
           std::string fragFilepath,
           const PipelineConfigInfo& config);

  ~Pipeline();

  Pipeline(const Pipeline&) = delete;
  Pipeline& operator=(const Pipeline&) = delete;

  static PipelineConfigInfo makeDefaultPipelineConfigInfo(uint32_t width,
                                                          uint32_t height);

 private:
  static std::vector<char> readFile(std::string filepath);

  void createGraphicsPipeline(std::string vertFilepath,
                              std::string fragFilepath,
                              const PipelineConfigInfo& config);

  void createShaderModule(const std::vector<char>& code,
                          VkShaderModule& shaderModule);

  Device& device;
  VkPipeline graphicsPipeline;
  VkShaderModule vertShaderModule;
  VkShaderModule fragShaderModule;
};

}  // namespace lve
