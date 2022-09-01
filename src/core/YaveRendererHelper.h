#pragma once
#include "../YaveLib/YaveUtils.h"

namespace   YaveRendererHelper
{
  VkShaderModule  createShaderModule(const std::string &filename) {
    return (createShaderModule(YaveLib::readBinaryFile(filename)));
  }
  VkShaderModule  createShaderModule(std::vector<char> &code);
}
