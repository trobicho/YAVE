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
		void	createSurface();
		void	createSwapchain();
		void	createImageView();

	private:
		//InstanceParams has to be still valid for a least the init() call
		bool					m_isInit = false; //XXX probably init() in constructor
		YaveInstanceParams_t	&m_yaveInstanceParams 

		YaveSurfaceHandler		&m_surfaceHandler;
		YaveSwapchainHandler	&m_swapChainHandler;
		YaveImageViewHandler	&m_imageViewHandler;
		YaveRenderPassHandler	&m_renderPassHandler;
		YaveFramebuffersHandler	&m_framebuffersHandler;

		VkInstance				m_instance;
		VkPhysicalDevice		m_physicalDevice;
		YaveViewInfo_t			m_viewInfo;
};
