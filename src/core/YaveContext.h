#pragma once

#include <vulkan/vulkan.h>

struct	gpuInfo_t
{
  VkPhysicalDevice					device;
  VkPhysicalDeviceProperties		props;
  VkPhysicalDeviceMemoryProperties	memProps;
  VkPhysicalDeviceFeatures			features;
  VkSurfaceCapabilitiesKHR			surfaceCaps;
};

struct	VulkanContext
{
	gpuInfo_t	gpu;
	VkDevice	device;
	int			graphicsFamilyIdx;
	int			presentFamilyIdx;
	VkQueue		graphicsQueue;
	VkQueue		presentQueue;
};

class	DevicePicker
{
	public:
		virtual int&	rateDevice(gpuInfo_t &gpu);

	private:
		DevicePicker(){};
}
