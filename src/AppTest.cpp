#include "AppTest.h"

VkResult    MyGLFWSurfaceHandler::createSurface(VkInstance &instance, VkSurfaceKHR &surface)
{
  return (glfwCreateWindowSurface(instance, m_window, vkContext.allocatorCallbacks, &surface));
}

AppTest::AppTest(uint32_t width, uint32_t height) : m_width(width), m_height(height), m_surfaceHandler(nullptr)
{
  glfwInit();
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
  m_window = glfwCreateWindow(width, height, "Yave test", nullptr, nullptr);

  m_instanceParams = (YaveInstanceParams_t){
    .applicationName = "test",
    .validationLayers = {"VK_LAYER_LUNARG_standard_validation"},
    .instanceExtensions = {VK_EXT_DEBUG_UTILS_EXTENSION_NAME},
    .deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME},
    .windowExtent = (VkExtent2D){.width = m_width, .height = m_height},
    .surfaceHandler = &m_surfaceHandler,
    .swapchainHandler = &m_swapchainHandler,
    .imageViewHandler = &m_imageViewsHandler,
    .renderPassHandler = &m_renderPassHandler,
    .framebuffersHandler = &m_framebuffersHandler,
    .allocatorCallbacks = nullptr,
  };
  m_instanceParams.deviceFeatures.push_back((VkPhysicalDeviceFeatures){});

  uint32_t	glfwExtCount;
  const char	**glfwExt = glfwGetRequiredInstanceExtensions(&glfwExtCount);

  for (int i = 0; i < glfwExtCount; ++i)
  {
    m_instanceParams.instanceExtensions.push_back(glfwExt[i]);
  }

  m_surfaceHandler = MyGLFWSurfaceHandler(m_window);
}

AppTest::~AppTest()
{
  glfwDestroyWindow(m_window);
  glfwTerminate();
}

void	AppTest::run()
{
  YaveRenderer    renderer(m_instanceParams);

  m_surfaceHandler.createSurface(renderer.getInstance(), renderer.getSurfaceKHR());
  main_loop(renderer);
}

void	AppTest::main_loop(YaveRenderer &renderer)
{
  while (!glfwWindowShouldClose(m_window))
  {
    glfwPollEvents();
    //m_renderer.drawFrame();
  }
}
