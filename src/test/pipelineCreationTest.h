#pragma once
#include <string>

namespace   BasicPipelineTest
{
  VkPipeline  createGraphicsPipeline(VkPipelineLayout &pipelineLayout);
  void        destroyPipeline(VkPipeline  &graphicsPipeline
                , VkPipelineLayout &pipelineLayout);
};
