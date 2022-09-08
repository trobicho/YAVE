#pragma once
#include "../YaveLib/YaveUtils.h"
#include "../YaveLib/YaveException.h"
#include <vulkan/vulkan.h>

namespace   YaveRendererHelper
{
  VkShaderModule  createShaderModuleFromCode(std::vector<char> code);
  VkShaderModule  createShaderModuleFromFile(const std::string &filename) {
    return (createShaderModuleFromCode(YaveLib::readBinaryFile(filename)));
  }
}
