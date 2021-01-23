#include "YaveRenderer.h"
#include "YaveParams.h"
#include "YaveParamsValidator.h"
#include <stdexcept>
#include "../YaveLib/YaveException.h"

YaveRenderer::YaveRenderer(YaveInstanceParams_t yaveInstanceParams):
	m_yaveInstanceParams(yaveInstanceParams);
{
	YaveParamsValidator::instanceParamsChecker(m_yaveInstanceParams);
	YaveParamsValidator::validationLayerCheck(
		m_yaveInstanceParams.validationLayerCount,
		m_yaveInstanceParams.validationLayerNames
	);
	YaveParamsValidator::extensionCheck(
		m_yaveInstanceParams.extensionCount,
		m_yaveInstanceParams.extensionNames
	);
}

YaveRenderer::~YaveRenderer()
{
	vkDestroyInstance(m_);
}

void	YaveRenderer::init()
{
	create_instance();

	//TODO Debug messenger page 55 VkTuto

	m_wasInit = true;//XXX probably init() in constructor
}

void	YaveRenderer::createInstance()
{
	VkApplicationInfo appInfo = {};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = m_yaveInstanceParams.applicationName;
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "Yave";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_0;

	VkInstanceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;

	createInfo.enabledExtensionCount = m_yaveInstanceParams.extensionCount;
	createInfo.ppEnabledExtensionNames = m_yaveInstanceParams.extensionNames;
	createInfo.enabledLayerCount = m_yaveInstanceParams.validationLayerCount;
	createInfo.ppEnabledLayerNames = m_yaveInstanceParams.validationLayerNames;

	if (vkCreateInstance(&createInfo, nullptr, &m_instance) != VK_SUCCESS)
		throw YaveLib::FatalVulkanInitError("failed to create instance!\n");

	return (0);
}

int		My_vulkan::physical_device()
{
	int					i;
	uint32_t			device_count;
	VkPhysicalDevice	*device;

	vkEnumeratePhysicalDevices(m_instance, &device_count, NULL);
	if (device_count == 0)
		YaveLib::FatalVulkanInitError("failed to find GPU with Vulkan support!");
	std::multimap<int, VkPhysicalDevice>
	if ((device = (VkPhysicalDevice*)
		malloc(sizeof(*device) * device_count)) == NULL)
		return (-1);
	vkEnumeratePhysicalDevices(m_instance, &device_count, device);
	i = 0;
	m_dev_phy = VK_NULL_HANDLE;
	while (i < device_count && m_dev_phy == NULL)
	{
		if(is_device_suitable(device[i], m_surface))
			m_dev_phy = device[i];
		i++;
	}
	if (m_dev_phy == VK_NULL_HANDLE)
	{
		printf("failed to find a suitable GPU!\n");
		return (-1);
	}
	return (0);
}
