#pragma once
#include "YaveParams.h"

class	YaveBasicSwapchainHandler : public YaveSwapchainHandler
{
	public:
		YaveBasicSwapchainHandler(uint32_t frameCount = 2) : m_frameCount(frameCount){};

		VkResult	createSwapchain(YaveViewInfo_t &viewInfo, gpuInfo_t &gpu);
		VkResult	destroySwapchain(YaveViewInfo_t &viewInfo);

		static VkSurfaceFormatKHR	chooseSurfaceFormat(std::vector<VkSurfaceFormatKHR> &formats);
		static VkPresentModeKHR		choosePresentMode(std::vector<VkPresentModeKHR> &presentModes);
		static VkExtent2D			chooseSurfaceExtent(VkSurfaceCapabilitiesKHR &caps);

	private:
		uint32_t	m_frameCount;
};

class	YaveBasicImageViewHandler: public YaveImageViewHandler
{
	public:
		YaveImageViewHandler() : {};

		VkResult	createImageView(YaveViewInfo_t &viewInfo);
		VkResult	destroyImageView(YaveViewInfo_t &viewInfo);
};

class	YaveBasicRenderPassHandler : public YaveRenderPassHandler
{
	public:
		YaveRenderPassHandler() : {};

		VkResult	createRenderPass(YaveViewInfo_t &viewInfo);
		VkResult	destroyRenderPass(YaveViewInfo_t &viewInfo);
};
