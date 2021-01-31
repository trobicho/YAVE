#pragma once
#include <string>

namespace	BasicPipelineTest
{
	VkPipeline  createGraphicsPipeline(vkPipelineLayout &pipelineLayout);
	void        destroyPipeline(VkPipeline  &graphicsPipeline
					, vkPipelineLayout &pipelineLayout);
};
