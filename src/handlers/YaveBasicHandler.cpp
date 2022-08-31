#include "YaveBasicHandler.h"

//-------------------Swapchain------------------

VKResult	YaveBasicSwapchainHandler::createSwapchain(YaveViewInfo_t &viewInfo, gpuInfo_t &gpu)
{
	VkSurfaceFormatKHR	surfaceFormat = chooseSurfaceFormat(gpu.surfaceFormats);
	VkPresentModeKHR		presentMode = choosePresentMode(gpu.presentModes);
	VkExtent2D 					surfaceExtent = chooseSurfaceExtent(gpu.surfaceCaps, viewInfo.winExtent);

	uint32_t imageCount = detail.capability.minImageCount + 1;
	if (gpu.surfaceCaps.maxImageCount > 0 && m_frameCount > gpu.surfaceCaps.maxImageCount)
		m_frameCount = detail.capability.maxImageCount;
	viewInfo.frameCount = m_frameCount;

	VkSwapchainCreateInfoKHR info = {};
	info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	info.surface = viewInfo.surface;
	info.minImageCount = m_frameCount;
	info.imageFormat = surfaceFormat.format;
	info.imageColorSpace = surfaceFormat.colorSpace;
	info.imageExtent = surfaceExtent;
	info.imageArrayLayers = 1;
	info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
	if (vkContext.graphicsFamilyIdx != vkContext.presentFamilyIdx)
	{
		uint32_t indices[] = {(uint32_t)vkContext.graphicsFamilyIdx
			, (uint32_t)vkContext.presentFamilyIdx};
		info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		info.queueFamilyIndexCount = 2;
		info.pQueueFamilyIndices = indices;
	}
	else
		info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	info.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
	info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	info.presentMode = presentMode;
	info.clipped = VK_TRUE;
	info.oldSwapchain = VK_NULL_HANDLE;

	if (vkCreateSwapchainKHR(vkContext.device, &info
		, vkContext.allocatorCallbacks, &viewInfo.swapchain) != VK_SUCCESS)
		throw YaveLib::YaveHandlerError("failed to create swap chain!");
	imageCount = 0;
	vkGetSwapchainImagesKHR(vkContext.device, viewInfo.swapchain, &imageCount, NULL);
	if (imageCount <= 0)
		throw YaveLib::YaveHandlerError("vkGetSwapchainImagesKHR returned a zero image count.");
	viewInfo.swapchainImages.resize(imageCount);
	vkGetSwapchainImagesKHR(vkContext.device, viewInfo.swapchain
			, &imageCount, viewInfo.swapchainImages.data());
	if (imageCount <= 0)
		throw YaveLib::YaveHandlerError("vkGetSwapchainImagesKHR returned a zero image count.");
	viewInfo.swapchainImageFormat = surfaceFormat.format;
	viewInfo.swapchainExtent = surfaceExtent;
	return (VK_SUCCESS);
}

VkResult	YaveBasicSwapchainHandler::destroySwapchain(YaveViewInfo_t &viewInfo)
{
	vkDestroySwapchainKHR(vkContext.device, viewInfo.swapChain, vkContext.allocatorCallbacks);
	return (VK_SUCCESS);
}

VkSurfaceFormatKHR	YaveBasicSwapchainHandler::chooseSurfaceFormat(
	std::vector<VkSurfaceFormatKHR> &formats)
{
	if (formats.size() == 1 && formats[0].format == VK_FORMAT_UNDEFINED)
	{
		return ((VkSurfaceFormatKHR) {
			.format = VK_FORMAT_B8G8R8A8_UNORM,
			.colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR
		});
	}
	for (const auto &format : formats)
	{
		if (format.format == VK_FORMAT_B8G8R8A8_UNORM
			&& format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
			return (format);
	}
	return (formats[0]);
}

VkPresentModeKHR	YaveBasicSwapchainHandler::choosePresentMode(
	std::vector<VkPresentModeKHR> &presentModes)
{
	for (const auto &presentMode: presentModes)
	{
		if (presentMode == VK_PRESENT_MODE_MAILBOX_KHR)
			return (presentMode);
	}
	return (VK_PRESENT_MODE_FIFO_KHR);
}

#include <algorithm>

VkPresentModeKHR	YaveBasicSwapchainHandler::chooseSurfaceExtent(
	VkSurfaceCapabilitiesKHR &caps, VkExtent2D winExtent)
{
	VkExtent2D	extent;

	if (caps.currentExtent.width == -1)
	{
		extent.width = std::max(caps.minImageExtent.width
			, std::min(caps.maxImageExtent.witdh, winExtent.width));
		extent.height = std::max(caps.minImageExtent.height
			, std::min(caps.maxImageExtent.height, winExtent.height));
		return (extent);
	}
	else
		return (caps.currentExtent);
}

//-------------------ImageView------------------

VkResult	YaveBasicImageViewsHandler::createImageViews(YaveViewInfo_t &viewInfo)
{
	viewInfo.imageViews.resize(viewInfo.swapchainImages.size());
	for (int i = 0; i < viewInfo.frameCount; ++i)
	{
		VkImageViewCreateInfo info = {};
		info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		info.image = viewInfo.swapchainImages[i];
		info.viewType = VK_IMAGE_VIEW_TYPE_2D;
		info.format = viewInfo.swapchainImageFormat;
		info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
		info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		info.subresourceRange.baseMipLevel = 0;
		info.subresourceRange.levelCount = 1;
		info.subresourceRange.baseArrayLayer = 0;
		info.subresourceRange.layerCount = 1;
		info.flags = 0;
		if (vkCreateImageView(vkContext.device, &info
			, vkContext.allocatorCallbacks, &viewInfo.imageViews[i]) != VK_SUCCESS)
			throw YaveLib::YaveHandlerError("failed to create image views!");
	}
	return (VK_SUCCESS);
}

VkResult	YaveBasicImageViewHandler::destroyImageViews(YaveViewInfo_t &viewInfo)
{
	for (auto &imageView: viewInfo.imageViews)
	{
		vkDestroyImageView(vkContext.device, imageView, vkContext.allocatorCallbacks);
	}
	viewInfo.imageViews.clear();
	return (VK_SUCCESS);
}

//-------------------RenderPass-----------------

VkResult	YaveBasicRenderPassHandler::createRenderPass(YaveViewInfo_t &viewInfo)
{
	static VkAttachmentDescription	colorAttachment = {};
	static VkAttachmentReference	colorAttachmentRef = {};
	static VkAttachmentDescription	depthAttachment = {};
	static VkAttachmentReference	depthAttachmentRef = {};
	static VkSubpassDescription		subpass = {};
	static VkSubpassDependency		subpassDep = {};
	VkRenderPassCreateInfo			info = {};

	colorAttachment.format = viewInfo.swapchainImageFormat;
	colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	colorAttachmentRef.attachment = 0;
	colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colorAttachmentRef;
	subpass.pDepthStencilAttachment = nullptr;

	subpassDep.srcSubpass = VK_SUBPASS_EXTERNAL;
	subpassDep.dstSubpass = 0;
	subpassDep.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	subpassDep.srcAccessMask = 0;
	subpassDep.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	subpassDep.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT
		| VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

	std::array<VkAttachmentDescription, 1> attachments =
		{colorAttachment};

	std::array<VkAttachmentDescription, 1> attachments =
		{colorAttachment};

	info.dependencyCount = 1;
	info.pDependencies = &subpassDep;
	info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	info.attachmentCount =
		static_cast<uint32_t>(attachments.size());
	info.pAttachments = attachments.data();
	info.subpassCount = 1;
	info.pSubpasses = &subpass;
	if (vkCreateRenderPass(vkContext.device, &info, vkContext.allocatorCallbacks
		, &vkContext.renderPass) != VK_SUCCESS)
		throw YaveLib::YaveHandlerError("failed to create render pass!");
	return (VK_SUCCESS);
}

VkResult	YaveBasicRenderPassHandler::destroyRenderPass(YaveViewInfo_t &viewInfo)
{
	vkDestroyRenderPass(vkContext.device, vkContext.renderPass);
	return (VK_SUCCESS);
}

//------------------Framebuffers----------------

VkResult	YaveBasicFramebuffersHandler::createFramebuffers(YaveViewInfo_t &viewInfo)
{
	viewInfo.framebuffers.resize(viewInfo.imageViews.size());
	for (int i = 0; i < viewInfo.imageViews[i]; ++i)
	{
		VkFramebufferCreateInfo	info = {};
		std::array<VkImageView, 1> attachments =
			{viewInfo.imageViews[i]};
		framebuffer_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebuffer_info.renderPass = m_render_pass;
		framebuffer_info.attachmentCount =
			static_cast<uint32_t>(attachments.size());
		framebuffer_info.pAttachments = attachments.data();
		framebuffer_info.width = viewInfo.swapchainExtent.width;
		framebuffer_info.height = viewInfo.swapchainExtent.height;
		framebuffer_info.layers = 1;
		if (vkCreateFramebuffer(m_device, &framebuffer_info, NULL
			, &m_framebuffer[i]) != VK_SUCCESS)
			throw YaveLib::YaveHandlerError("failed to create framebuffers!");
	}
	return (VK_SUCCESS);
}

VkResult	YaveBasicFramebuffersHandler::destroyFramebuffers(YaveViewInfo_t &viewInfo)
{
	for (auto &framebuffer: viewInfo.framebuffers)
	{
		vkDestroyFramebuffer(vkContext.device, framebuffer, vkContext.allocatorCallbacks);
	}
	viewInfo.framebuffers.clear();
	return (VK_SUCCESS);
}
