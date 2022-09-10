#pragma once
#include <string>
#include <vulkan/vulkan.h>

namespace   BasicPipelineTest
{
  VkPipeline  createGraphicsPipeline(VkPipelineLayout &pipelineLayout);
  void        destroyPipeline(VkPipeline  &graphicsPipeline
                , VkPipelineLayout &pipelineLayout);
}
