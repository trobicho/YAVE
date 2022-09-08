#ifndef CONTEXT_HEADER
#define CONTEXT_HEADER

#include <vulkan/vulkan.h>
#include <vector>

struct	gpuInfo_t
{
  VkPhysicalDevice                      device;
  VkPhysicalDeviceProperties            props;
  VkPhysicalDeviceMemoryProperties      memProps;
  VkPhysicalDeviceFeatures              features;
  VkSurfaceCapabilitiesKHR		surfaceCaps;
  std::vector<VkSurfaceFormatKHR>	surfaceFormats;
  std::vector<VkPresentModeKHR>		presentModes;
  std::vector<VkQueueFamilyProperties>	queueFamilyProps;
  std::vector<VkExtensionProperties>	extensionProps;
};

struct	vulkanContext_t
{
  gpuInfo_t		gpu;

  VkDevice		device;
  int			graphicsFamilyIdx;
  int			presentFamilyIdx;
  VkQueue		graphicsQueue;
  VkQueue		presentQueue;

  VkRenderPass		renderPass;
  VkPipelineCache	pipelineCache;

  VkAllocationCallbacks *allocatorCallbacks = nullptr;
  uint32_t		frameCount;
};

class	DevicePicker
{
  public:
    virtual int  rateDevice(gpuInfo_t &gpu);
};

#endif
