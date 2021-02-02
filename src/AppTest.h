#include "core/YAVE.h"

class	MyGLFWSurfaceHandler : public YaveSurfaceHandler
{
	public:
		MyGLFWSurfaceHandler(GLFWwindow	*window): m_window(window);

		VkResult	createSurface(VkInstance &instance, VkSurfaceKHR &surface);

	private:
		GLFWwindow	*m_window;
};

class	AppTest
{
	public:
		AppTest(){};
		~AppTest(){};

		void	run();

	private:
		void	main_loop();
		void	createInstanceParam();

		GLFWwindow		*m_window;

		YaveInstanceParams_t	m_instanceParams;
		YaveRenderer			m_renderer;
		int						m_width;
		int						m_height;

		MyGLFWSurfaceHandler			m_surfaceHandler;
		YaveBasicSwapchainHandler		m_swapchainHandler;
		YaveBasicImageViewsHandler		m_imageViewsHandler;
		YaveBasicRenderPassHandler 		m_renderPassHandler;
		YaveBasicFramebuffersHandler	m_framebuffersHandler;
}
