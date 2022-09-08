#pragma once
#include "../core/YaveParams.h"

class	YaveBasicSwapchainHandler : public YaveSwapchainHandler
{
  public:
    YaveBasicSwapchainHandler(uint32_t frameCount = 2) : m_frameCount(frameCount){};

    VkResult  createSwapchain(YaveViewInfo_t &viewInfo, gpuInfo_t &gpu);
    VkResult  destroySwapchain(YaveViewInfo_t &viewInfo);

    static VkSurfaceFormatKHR   chooseSurfaceFormat(std::vector<VkSurfaceFormatKHR> &formats);
    static VkPresentModeKHR     choosePresentMode(std::vector<VkPresentModeKHR> &presentModes);
    static VkExtent2D           chooseSurfaceExtent(VkSurfaceCapabilitiesKHR &caps, VkExtent2D winExtent);

  private:
    uint32_t    m_frameCount;
};

class	YaveBasicImageViewsHandler : public YaveImageViewsHandler
{
  public:
    YaveBasicImageViewsHandler() {};

    VkResult	createImageViews(YaveViewInfo_t &viewInfo);
    VkResult	destroyImageViews(YaveViewInfo_t &viewInfo);
};

class	YaveBasicRenderPassHandler : public YaveRenderPassHandler
{
  public:
    YaveBasicRenderPassHandler() {};

    VkResult	createRenderPass(YaveViewInfo_t &viewInfo);
    VkResult	destroyRenderPass(YaveViewInfo_t &viewInfo);
};

class	YaveBasicFramebuffersHandler : public YaveFramebuffersHandler
{
  public:
    YaveBasicFramebuffersHandler() {};

    VkResult	createFramebuffers(YaveViewInfo_t &viewInfo);
    VkResult	destroyFramebuffers(YaveViewInfo_t &viewInfo);
};
