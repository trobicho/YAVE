#pragma once

#include "YaveParams.h"

class YaveRenderer
{
	public:
		YaveRenderer(YaveInstanceParams_t &yaveInstanceParams);
		~YaveRenderer();

		void	init();
		void	cleanup();

	private:
		void	createInstance();
		void	choosePhysicalDevice();
		void	createLogicalDeviceAndQueue();
		void	createCommandPool();
		void	createCommandBuffers();
		void	createSemaphores();

		void	createSurface();
		void	createSwapchain();
		void	createImageView();
		void	createRenderPass();
		void	createFramebuffers();

	private:
		//InstanceParams has to be still valid for a least the init() call
		bool					m_isInit = false; //XXX probably init() in constructor
		uint32_t				m_frameCount;
		YaveInstanceParams_t	&m_yaveInstanceParams 

		YaveSurfaceHandler		&m_surfaceHandler;
		YaveSwapchainHandler	&m_swapChainHandler;
		YaveImageViewsHandler	&m_imageViewsHandler;
		YaveRenderPassHandler	&m_renderPassHandler;
		YaveFramebuffersHandler	&m_framebuffersHandler;

		YaveViewInfo_t			m_viewInfo;

		VkInstance						m_instance;
		VkPhysicalDevice				m_physicalDevice;
		VkCommandPool					m_commandPool;
		std::vector<VkCommandBuffer>	m_commandBuffers;
		std::vector<VkFences>			m_commandBufferFences;
		std::vector<VkSemaphore>		m_acquireSemaphores;
		std::vector<VkSemaphore>		m_renderCompleteSemaphores;
};
