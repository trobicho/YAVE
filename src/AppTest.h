#include "core/YAVE.h"
#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"
#include "handlers/YaveBasicHandler.h"

class	MyGLFWSurfaceHandler : public YaveSurfaceHandler
{
  public:
    MyGLFWSurfaceHandler(GLFWwindow *window): m_window(window) {};

    VkResult    createSurface(VkInstance &instance, VkSurfaceKHR &surface);

  private:
    GLFWwindow	*m_window;
};

class	AppTest
{
  public:
    AppTest(int width, int height);
    ~AppTest();

    void    run();

  private:
    void    main_loop(YaveRenderer &renderer);
    //void    createInstanceParam();

    GLFWwindow  *m_window;

    YaveInstanceParams_t  m_instanceParams;

    int   m_width;
    int   m_height;

    MyGLFWSurfaceHandler            m_surfaceHandler;
    YaveBasicSwapchainHandler       m_swapchainHandler;
    YaveBasicImageViewsHandler      m_imageViewsHandler;
    YaveBasicRenderPassHandler      m_renderPassHandler;
    YaveBasicFramebuffersHandler    m_framebuffersHandler;
};
