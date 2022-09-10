#pragma once
#include "../YaveLib/YaveUtils.h"
#include "../YaveLib/YaveException.h"
#include <vulkan/vulkan.h>
#include <vector>
#include <string>

namespace   YaveRendererHelper
{
  VkShaderModule  createShaderModuleFromCode(std::vector<char> code);
  inline VkShaderModule  createShaderModuleFromFile(const std::string &filename) {
    return (createShaderModuleFromCode(YaveLib::readBinaryFile(filename)));
  }
}
