#include "AppTest.h"

VkResult    MyGLFWSurfaceHandler::createSurface(VkInstance &instance, VkSurfaceKHR &surface)
{
  return (glfwCreateWindowSurface(instance, m_win, vkContext.allocatorCallbacks, &surface));
}

void        AppTest::createInstanceParam()
{
  YaveInstanceParams_t	instanceParams = {
    .applicationName = "test",
    .validationLayers = {"VK_LAYER_LUNARG_standard_validation"},
    .instanceExtensions = {VK_EXT_DEBUG_UTILS_EXTENSION_NAME},
    .deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME},
    .windowExtent = (VkExtent2D){.witdh = m_width, .height = height},
    .surfaceHandler = m_surfaceHandler,
    .swapchainHandler = m_swapchainHandler,
    .imageViewHandler = m_imageViewsHandler,
    .renderPassHandler = m_renderPassHandler,
    .framebuffersHandler = m_framebuffersHandler,
    .deviceFeatures = VkPhysicalDeviceFeatures{},
    .allocatorCallbacks = nullptr,
  };

  uint32_t	glfwExtCount;
  const char	**glfwExt = glfwGetRequiredInstanceExtensions(&glfwExtCount);

  for (int i = 0; i < glfwExtCount; ++i)
  {
    instanceParams.instanceExtensions.push_back(glfwExt[i]);
  }
}

AppTest::AppTest(width, height) : m_width(width), m_height(height)
{
  glfwInit();
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZEABLE, GLFW_FALSE);
  m_window = glfwCreateWindow(width, height, "Yave test", nullptr, nullptr);
}

AppTest::~AppTest()
{
  glfwDestroyWindow(m_window);
  glfwTerminate();
}

void	AppTest::run()
{
  main_loop();
}

void	AppTest::main_loop()
{
  while (!glfwWindowShouldClose(m_window))
  {
    glfwPollEvents();
    m_renderer.drawFrame();
  }
}
