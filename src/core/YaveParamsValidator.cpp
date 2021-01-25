#include "YaveRendererParamsDef.h"
#include <stdexcept>
#include <cstring>
#include <string>
#include <vector>
#include "../YaveLib/YaveException.h"


void	YaveParamsValidator::instanceParamsChecker(YaveInstanceParams_t &yaveInstanceParams)
{
	if (yaveInstanceParams.applicationName == nullptr)
	{
		throw YaveLib::YaveInvalidInstanceParam(
			"invalid parameter: applicationName");
	}
	if (yaveInstanceParams.validationLayerCount > 0
		&& yaveInstanceParams.validationLayerName == nullptr
	{
		throw YaveLib::YaveInvalidInstanceParam(
			"invalid vulkan parameter: validationLayerName");
	}
	if (yaveInstanceParams.instanceExtensionCount > 0
		&& yaveInstanceParams.instanceExtensionName == nullptr
	{
		throw YaveLib::YaveInvalidInstanceParam(
			"invalid vulkan parameter: instanceExtensionName");
	}
	if (yaveInstanceParams.deviceExtensionCount > 0
		&& yaveInstanceParams.deviceExtensionName == nullptr
	{
		throw YaveLib::YaveInvalidInstanceParam(
			"invalid vulkan parameter: deviceExtensionName");
	}
}

void	YaveParamsValidator::validationLayerValidator(
	uint32_t validationLayerCount, const char* const* validationLayerNames)
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

void	YaveParamsValidator::instanceExtensionCheck(uint32_t extensionCount
	, const char* const* extensionNames);
{
	uint32 instanceExtensionCount = 0;
	vkEnumerateInstanceExtensionProperties(&instanceExtensionCount, nullptr);
	std::vector<VkExtensionProperties> instanceExtension;
	instanceExtension.resize(instanceExtensionCount);
	vkEnumerateInstanceExtensionProperties(&instanceExtensionCount, instanceExtension.data());

	bool found = false;
	for (uint32 i = 0; i < extensionCount; ++i)
	{
		for (uint32 j = 0; j < instanceExtensionCount; ++j)
		{
			if (strcmp(extensionNames[i], instanceExtension[j].extensionName) == 0)
			{
				found = true
				break;
			}
		}
		if (!found)
		{
			throw YaveLib::YaveInstanceParamError(
				std::string("Cannot find extension: ")
				+ extensionNames[i]);
		}
	}
}
