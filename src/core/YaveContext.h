#pragma once

#include <vulkan/vulkan.h>
#include <vector>

struct	gpuInfo_t
{
  VkPhysicalDevice						device;
  VkPhysicalDeviceProperties			props;
  VkPhysicalDeviceMemoryProperties		memProps;
  VkPhysicalDeviceFeatures				features;
  VkSurfaceCapabilitiesKHR				surfaceCaps;
  std::vector<VkSurfaceFormatKHR>		surfaceFormats;
  std::vector<VkPresentModeKHR>			presentModes;
  std::vector<VkQueueFamilyProperties>	queueFamilyProps;
  std::vector<VkExtensionProperties>	extensionProps;
};

struct	VulkanContext
{
	gpuInfo_t				gpu;
	VkDevice				device;
	int						graphicsFamilyIdx;
	int						presentFamilyIdx;
	VkQueue					graphicsQueue;
	VkQueue					presentQueue;
	VkAllocationCallbacks	*allocatorCallbacks = nullptr;
};

class	DevicePicker
{
	public:
		virtual int&	rateDevice(gpuInfo_t &gpu);

	private:
		DevicePicker(){};
}
