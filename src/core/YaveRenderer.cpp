#include "YaveRenderer.h"
#include "YaveParams.h"
#include "YaveParamsValidator.h"
#include "../YaveLib/YaveException.h"
#include <vector>

YaveRenderer::YaveRenderer(YaveInstanceParams_t &yaveInstanceParams):
	m_yaveInstanceParams(yaveInstanceParams);
{
	YaveParamsValidator::instanceParamsChecker(m_yaveInstanceParams);
	YaveParamsValidator::validationLayerCheck(
		m_yaveInstanceParams.validationLayerCount,
		m_yaveInstanceParams.validationLayerNames
	);
	YaveParamsValidator::instanceExtensionCheck(
		m_yaveInstanceParams.instanceExtensionCount,
		m_yaveInstanceParams.instanceExtensionNames
	);

	m_surfaceHandler = m_yaveInstanceParams.surfaceHandler;
	m_swapchainHandler = m_yaveInstanceParams.swapchainHandler;
	m_renderPassHandler = m_yaveInstanceParams.renderPassHandler;
	m_framebuffersHandler = m_yaveInstanceParams.framebuffersHandler;
}

YaveRenderer::~YaveRenderer()
{
	if (m_isInit)
		cleanup();
}

void	YaveRenderer::cleanup()
{
	m_renderPassHandler.destroyRenderPass(m_viewInfo);
	m_swapchainHandler.destroySwapchain(m_viewInfo);
	m_imageViewHandler.destroyImageView(m_viewInfo);
	m_frambuffersHandler.destroyFrameBuffer(m_viewInfo);
	m_surfaceHandler.destroySurface(m_viewInfo.surface);
	vkDestroyInstance(m_instance);
	vkDestroyDevice(vkContext.device, m_yaveInstanceParams.allocatorCallbacks);

	m_isInit = false;
}

void	YaveRenderer::init()
{
	createInstance();

	//TODO Debug messenger page 55 VkTuto

	createSurface();

	choosePhysicalDevice();

	createLogicalDeviceAndQueue();

	createSwapChain();

	m_isInit = true;//XXX probably init() in constructor
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

	createInfo.enabledExtensionCount = m_yaveInstanceParams.instanceExtensionCount;
	createInfo.ppEnabledExtensionNames = m_yaveInstanceParams.instanceExtensionNames;
	createInfo.enabledLayerCount = m_yaveInstanceParams.validationLayerCount;
	createInfo.ppEnabledLayerNames = m_yaveInstanceParams.validationLayerNames;

	if (vkCreateInstance(&createInfo, nullptr, &m_instance) != VK_SUCCESS)
		throw YaveLib::FatalVulkanInitError("failed to create instance!\n");

	return (0);
}

void	YaveRenderer::choosePhysicalDevice() //TODO: Implementation (page 63)
{
	int					i;
	uint32_t			deviceCount;
	VkPhysicalDevice	*device;

	vkEnumeratePhysicalDevices(m_instance, &deviceCount, NULL);
	if (deviceCount == 0)
		YaveLib::FatalVulkanInitError("failed to find GPU with Vulkan support!");

	std::multimap<int, gpuInfo_t>	gpuCanditates;
	std::vector<VkPhysicalDevice>	devices(deviceCount);

	vkEnumeratePhysicalDevices(m_instance, &device_count, devices.data());
	if (device_count == 0)
		YaveLib::FatalVulkanInitError("failed to find GPU with Vulkan support!");
	i = 0;
	for (i = 0; i < deviceCount; ++i)
	{
	}
	m_dev_phy = VK_NULL_HANDLE;
	while (i < device_count && m_dev_phy == NULL)
	{
		if(is_device_suitable(device[i], m_viewInfo.surface))
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

void	YaveRenderer::createLogicalDeviceAndQueue()
{
	std::vector<int>						queueIdx;
	std::vector<VkDeviceQueueCreateInfo>	queueInfo;

	queueIdx.push_back(vkContext.presentFamilyIdx);
	queueIdx.push_back(vkContext.graphicsFamilyIdx);
	const float	priority = 1.0f;
	for (int i = 0; i < queueIdx.size(); ++i)
	{
		VkDeviceQueueCreateInfo	qInfo = {};
		qInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		qInfo.FamilyIndex = queueIdx[i];
		qInfo.queueCount = 1;
		qInfo.pQueuePriorities = &priority;
		devqInfo.push_back(qInfo);
	}

	VkDeviceCreateInfo	createInfo;
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	createInfo.queueCreateInfoCount = devqInfo.size();
	createInfo.pQueueCreateInfos = devqInfo.data();
	createInfo.pEnabledFeatures = m_yaveInstanceParams.deviceFeatures;
	createInfo.enabledExtensionCount = m_yaveInstanceParams.deviceExtensionCount;
	createInfo.ppEnabledExtensionCount = m_yaveInstanceParams.deviceExtensionNames;
	createInfo.enabledLayerCount = m_yaveInstanceParams.validationLayerCount;
	createInfo.enabledLayerCount = m_yaveInstanceParams.validationLayerNames;

	if (vkCreateDevice(m_physicalDevice, &createInfo
		, m_yaveInstanceParams.allocatorCallbacks, &vkContext.device) != VK_SUCCESS
		throw YaveLib::FatalVulkanInitError("failed to create logical device and queues!\n");

	vkGetDeviceQueue(vkContext.device, vkContext.graphicsFamilyIdx, 0, &vkContext.graphicsQueue);
	vkGetDeviceQueue(vkContext.device, vkContext.presentFamilyIdx, 0, &vkContext.presentQueue);
}

void	YaveRenderer::createSurface()
{
	if (m_surfaceHandler.createSurface(m_instance,
		m_yaveInstanceParams.allocatorCallbacks, m_viewInfo.surface) != VK_SUCCESS)
		throw YaveLib::FatalVulkanInitError("failed to create surface!\n");
}

void	YaveRenderer::createSwapchain()
{
	if (m_swapchainHandler.createSwapchain(m_viewInfo) != VK_RESULT) //probably try catch
		throw YaveLib::FatalVulkanInitError("failed to create swapchain!\n");
}

void	YaveRenderer::createImageView()
{
	if (m_imageViewHandler.createImageView(m_viewInfo) != VK_RESULT) //probably try catch
		throw YaveLib::FatalVulkanInitError("failed to create imageView!\n");
}

void	YaveRenderer::createRenderPass()
{
	if (m_renderPassHandler.createRenderPass(m_viewInfo) != VK_RESULT) //probably try catch
		throw YaveLib::FatalVulkanInitError("failed to create renderPass!\n");
}

void	YaveRenderer::createFramebuffers()
{
	if (m_framebuffersHandler.createFramebuffers(m_viewInfo) != VK_RESULT) //probably try catch
		throw YaveLib::FatalVulkanInitError("failed to create frameBuffers!\n");
}
