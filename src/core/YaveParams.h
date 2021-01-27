#pragma once
#include <vector>

/*
static const int	g_numValidationLayers = 1;
static const char	*g_validationLayers[g_numValidatinLayers] = {
	"VK_LAYER_LUNARG_standard_validation"
};
*/

//TODO: YaveSetupBasic YaveSetupGeometryShader ... other (class?, file)

//TODO: yaveGetMandatoryInstanceExtension
//TODO: yaveGetDebugInstanceExtension
//TODO: yaveGetBasicValidationLayer

struct	YaveViewInfo_t
{
	VkExtent					winExtent;
	VkSurfaceKHR				surface;
	VkSwapchainKHR				swapchain;
	VkFormat					swapchainImageFormat;
	VkExtent2D					swapchainExtent;
	std::vector<VkImage>		swapchainImages;
	std::vector<VkImageView>	imageViews;
	std::vector<VkFramebuffer>	frameBuffers;
	uint32_t					frameCount;
}

/*
=============
Fully virtual handler class
to handle creation of vulkan object
=============
*/

class	YaveSurfaceHandler
{
	public:
		virtual	YaveSurfaceHandler() = 0;

		virtual VkResult	createSurface(VkInstance&, VkSurfaceKHR&) = 0;
		virtual VkResult	destroySurface(VkSurfaceKHR&);
}

class	YaveSwapchainHandler
{
	public:
		virtual	YaveSwapchainHandler() = 0;

		virtual VkResult	createSwaphain(YaveViewInfo_t &viewInfo, gpuInfo_t &gpu) = 0;
		virtual VkResult	destroySwapchain(YaveViewInfo_t &viewInfo) = 0;
}

class	YaveImageViewsHandler
{
	public:
		virtual	YaveImageViewsHandler() = 0;

		virtual VkResult	createImageViews(YaveViewInfo_t &viewInfo) = 0;
		virtual VkResult	destroyImageViews(YaveViewInfo_t &viewInfo) = 0;
}

class	YaveRenderPassHandler
{
	public:
		virtual	YaveRenderPassHandler() = 0;

		virtual VkResult	createRenderPass(YaveViewInfo_t &viewInfo) = 0;
		virtual VkResult	destroyRenderPass(YaveViewInfo_t &viewInfo) = 0;
}

class	YaveFramebuffersHandler
{
	public:
		virtual	YaveFramebuffersHandler() = 0;

		virtual VkResult	createFramebuffers(YaveViewInfo_t &viewInfo) = 0;
		virtual VkResult	destroyFramebuffers(YaveViewInfo_t &viewInfo) = 0;
}

/*
=============
Yave instance creation parameter used by YaveRenderer::init()
and swapchain recreation
=============
*/

struct	YaveInstanceParams_t
{
	std::string					applicationName;
	std::vector<const char*>	validationLayers;
	std::vector<const char*>	instanceExtensions;
	std::vector<const char*>	deviceExtensions;
	VkExtent2D					windowExtent;

	YaveSurfaceHandler			&surfaceHandler;
	YaveSwapchainHandler		&swapchainHandler;
	YaveImageViewHandler		&imageViewHandler;
	YaveRenderPassHandler		&renderPassHandler;
	YaveFramebuffersHandler		&framebuffersHandler;

	VkPhysicalDeviceFeatures	deviceFeatures;
	VkAllocationCallbacks		*allocatorCallbacks = nullptr; //TODO: wrap this thing with YaveAllocator or something probably going to be NULL a lot of time but idc
};

namespace	YaveParamsValidator
{
	void	instanceParamsChecker(YaveInstanceParams_t &yaveInstanceParams);
	void	validationLayersCheck(std::vector<const char*> validationLayers);
	void	instanceExtensionsCheck(std::vector<const char*> extensions);
}
