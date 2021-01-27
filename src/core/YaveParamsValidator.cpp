#include "YaveRendererParamsDef.h"
#include <stdexcept>
#include <cstring>
#include <string>
#include <vector>
#include "../YaveLib/YaveException.h"


void	YaveParamsValidator::instanceParamsChecker(YaveInstanceParams_t &yaveInstanceParams)
{
}

void	YaveParamsValidator::validationLayersCheck(std::vector<const char*> validationLayers)
{
	uint32 instanceLayerCount = 0;
	vkEnumerateInstanceLayerProperties(&instanceLayerCount, nullptr);
	std::vector<VkLayerProperties> instanceLayers;
	instanceLayers.resize(instanceLayerCount);
	vkEnumerateInstanceLayerProperties(&instanceLayerCount, instanceLayers.data());

	bool found = false;
	for (uint32 i = 0; i < validationLayerCount; ++i)
	{
		for (uint32 j = 0; j < instanceLayerCount; ++j)
		{
			if (strcmp(validationLayerNames[i], instanceLayer[j].layerName) == 0)
			{
				found = true
				break;
			}
		}
		if (!found)
		{
			throw YaveLib::YaveInstanceParamError(
				std::string("Cannot find validation layer: ")
				+ validationLayerNames[i]);
		}
	}
}

void	YaveParamsValidator::instanceExtensionsCheck(std::vector<const char*> extensions)
{
	uint32 instanceExtensionCount = 0;
	vkEnumerateInstanceExtensionProperties(&instanceExtensionCount, nullptr);
	std::vector<VkExtensionProperties> instanceExtension;
	instanceExtension.resize(instanceExtensionCount);
	vkEnumerateInstanceExtensionProperties(&instanceExtensionCount, instanceExtension.data());

	bool found = false;
	for (int i = 0; i < extensions.size(); ++i)
	{
		for (int j = 0; j < instanceExtensionCount; ++j)
		{
			if (strcmp(extensions[i], instanceExtension[j].extensionName) == 0)
			{
				found = true
				break;
			}
		}
		if (!found)
		{
			throw YaveLib::YaveInstanceParamError(
				std::string("Cannot find extension: ")
				+ extensions[i]);
		}
	}
}
