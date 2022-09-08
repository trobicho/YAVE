#pragma once
#include <string>
#include "../core/YaveRendererHelper.h"

namespace   BasicPipelineTest
{
  VkPipeline  createGraphicsPipeline(VkPipelineLayout &pipelineLayout);
  void        destroyPipeline(VkPipeline  &graphicsPipeline
                , VkPipelineLayout &pipelineLayout);
};
