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
	VkSurfaceKHR				surface;
	VkSwapchainKHR				swapchain;
	VkFormat					swapchainImageFormat;
	VkExtent2D					swapchainExtent;
	std::vector<VkImage>		swapchainImages;
	std::vector<VkImageViews>	swapchainViews
	std::vector<VkFramebuffer>	frameBuffers;
}

class	YaveSurfaceHandler
{
	public:
		virtual	YaveSurfaceHandler() = 0;

		virtual VkResult	createSurface(VkInstance&, VkAllocationCallbacks*, VkSurfaceKHR&) = 0;
		virtual VkResult	destroySurface(VkSurfaceKHR&);
}

class	YaveSwapchainHandler
{
	public:
		virtual	YaveSwapchainHandler() = 0;

		virtual VkResult	createSwaphain(YaveViewInfo_t &viewInfo) = 0;
		virtual VkResult	destroySwapchain(YaveViewInfo_t &viewInfo) = 0;
}

class	YaveImageViewHandler
{
	public:
		virtual	YaveImageViewHandler() = 0;

		virtual VkResult	createImageView(YaveViewInfo_t &viewInfo) = 0;
		virtual VkResult	destroyImageView(YaveViewInfo_t &viewInfo) = 0;
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

struct	YaveInstanceParams_t
{
	const char* const*			applicationName;
	uint32_t					validationLayerCount = 0; //TODO: vector or something
	const char* const*			validationLayerNames;
	uint32_t					instanceExtensionCount = 0;
	const char* const*			instanceExtensionNames;
	uint32_t					deviceExtensionCount = 0;
	const char* const*			deviceExtensionNames;

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
	void	validationLayerCheck(uint32_t validationLayerCount
		, const char* const* validationLayerNames);
	void	instanceExtensionCheck(uint32_t extensionCount
		, const char* const* extensionNames);
}
