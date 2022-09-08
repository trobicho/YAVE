#include "YaveRendererHelper.h"
#include "YaveContext.h"
#include "YaveContextGlobal.h"

VkShaderModule	YaveRendererHelper::createShaderModuleFromCode(std::vector<char> code)
{
  VkShaderModuleCreateInfo  info{};
  VkShaderModule            shaderModule;

  info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
  info.codeSize = code.size();
  info.pCode = reinterpret_cast<const uint32_t*>(code.data());
  if (vkCreateShaderModule(vkContext.device, &info
        , vkContext.allocatorCallbacks, &shaderModule) != VK_SUCCESS)
    throw YaveLib::YaveHelperError("unable to create shader module!");
  return (shaderModule);
}
