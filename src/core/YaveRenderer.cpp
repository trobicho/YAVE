#include "YaveRenderer.h"
#include "YaveParams.h"
#include "YaveParamsValidator.h"
#include "../YaveLib/YaveException.h"
#include <vector>

YaveRenderer::YaveRenderer(YaveInstanceParams_t &yaveInstanceParams):
  m_yaveInstanceParams(yaveInstanceParams);
{
  YaveParamsValidator::instanceParamsChecker(m_yaveInstanceParams);
  YaveParamsValidator::validationLayersCheck(
    m_yaveInstanceParams.validationLayers
  );
  YaveParamsValidator::instanceExtensionsCheck(
    m_yaveInstanceParams.instanceExtensions
  );

  m_surfaceHandler = m_yaveInstanceParams.surfaceHandler;
  m_swapchainHandler = m_yaveInstanceParams.swapchainHandler;
  m_renderPassHandler = m_yaveInstanceParams.renderPassHandler;
  m_framebuffersHandler = m_yaveInstanceParams.framebuffersHandler;
  m_viewInfo.winExtent = m_yaveInstanceParams.windowExtent;
}

YaveRenderer::~YaveRenderer()
{
  if (m_isInit)
    cleanup();
}

void	YaveRenderer::cleanup()
{
  for (int i = 0; i < m_frameCount; ++i)
  {
    vkDestroyFence(vkContext.device, m_commandBufferFences[i]
      , vkContext.allocatorCallbacks);
  }
  vkDestroyCommandPool(vkContext.device, m_commandPool, vkContext.allocatorCallbacks);
  m_renderPassHandler.destroyRenderPass(m_viewInfo);
  m_swapchainHandler.destroySwapchain(m_viewInfo);
  m_imageViewHandler.destroyImageView(m_viewInfo);
  m_frambuffersHandler.destroyFrameBuffer(m_viewInfo);
  vkDestroySurfaceKHR(m_instance, m_viewInfo.surface, vkContext.allocatorCallbacks);
  vkDestroyInstance(m_instance);
  vkDestroyDevice(vkContext.device, vkContext.allocatorCallbacks);

  m_isInit = false;
}

void	YaveRenderer::init()
{
  createInstance();
  //TODO Debug messenger page 55 VkTuto
  createSurface();
  choosePhysicalDevice();
  createLogicalDeviceAndQueue();
  createSwapChain();
  createImageViews();
  createRenderPass();

  createFramebuffers();
  createCommandPool();
  createCommandBuffers();

  m_isInit = true;//XXX probably init() in constructor, probably not
}

void	YaveRenderer::drawFrame()
{
  uint32_t          imageIndex;
  VkCommandBuffer   commandBuffer = m_commandBuffer[m_currentFrameData];

  vkAcquireNextImageKHR(device, swapChain, UINT64_MAX
      , m_acquireSemaphores[m_currentFrame], VK_NULL_HANDLE, &imageIndex);

  VkSubmitInfo	submitInfo{};
  submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

  VkSemaphore waitSemaphores[] = {m_acquireSemaphores[m_currentFrame]};
  VkPipelineStageFlags waitStages[] =
  {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};

  submitInfo.waitSemaphoreCount = 1;
  submitInfo.pWaitSemaphores = waitSemaphores;
  submitInfo.pWaitDstStageMask = waitStages;
  submitInfo.commandBufferCount = 1;
  submitInfo.pCommandBuffers = &commandBuffer;
  VkSemaphore signalSemaphores[] = {m_renderCompleteSemaphores};
  submitInfo.signalSemaphoreCount = 1;
  submitInfo.pSignalSemaphores = signalSemaphores;

  if (vkQueueSubmit(vkContext.graphicsQueue, 1, &submitInfo
        , m_commandBufferFences[m_currentFrame]) != VK_SUCCESS)
  {
    throw YaveLib::YaveRendererError("failed to submit draw command buffer!");
  }

  VkPresentInfoKHR	presentInfo = {};
  presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
  presentInfo.waitSemaphoreCount = 1;
  presentInfo.pWaitSemaphores = m_renderCompleteSemaphores;
  VkSwapchainKHR	swapChains[] = {m_viewInfo.swapchain};
  presentInfo.swapchainCount = 1;
  presentInfo.pSwapchains = swapChains;
  presentInfo.pImageIndices = &imageIndex;
  presentInfo.pResults = nullptr;
  vkQueuePresentKHR(vkContext.presentQueue, &presentInfo);

  m_counterFrame++:
    m_currentFrame = m_counterFrame % m_frameCount;
}

void	YaveRenderer::commandBufferRecord()
{
  VkClearValue			clearColor = (VkClearValue){0.0f, 0.0f, 0.0f, 1.0f};
  VkRenderPassBeginInfo	renderPassInfo;

  for (size_t i = 0; i < m_commandBuffers.size(); i++)
  {
    VkCommandBufferBeginInfo	beginInfo = {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = 0; // Optional
    beginInfo.pInheritanceInfo = nullptr; // Optional
    if (vkBeginCommandBuffer(m_commandBuffers[i], &beginInfo) != VK_SUCCESS)
      throw YaveLib::YaveRendererError("failed to begin recording command buffer!");
    renderPassInfo = (VkRenderPassBeginInfo){};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = vkContext.renderPass;
    renderPassInfo.framebuffer = m_viewInfo.framebuffer[i];
    renderPassInfo.renderArea.offset = (VkOffset2D){0, 0};
    renderPassInfo.renderArea.extent = m_viewInfo.swapchainExtent;
    renderPassInfo.clearValueCount = 1;
    renderPassInfo.pClearValues = &clearColor;

    auto	&renderProg = renderProgManager.getCurrentRenderProg();
    vkCmdBeginRenderPass(m_commandBuffers[i], &renderPassInfo
        , VK_SUBPASS_CONTENTS_INLINE);
    vkCmdBindPipeline(m_command_buffer[i]
        , VK_PIPELINE_BIND_POINT_GRAPHICS, renderProg.pipelines[0]);

    vkCmdDraw(m_commandBuffers[i], 3, 1, 0, 0);

    vkCmdEndRenderPass(m_commandBuffers[i]);
    if (vkEndCommandBuffer(m_commandBuffers[i]) != VK_SUCCESS)
      throw YaveLib::YaveRendererError("failed to record command buffer!");
  }
}

void	YaveRenderer::createInstance()
{
  VkApplicationInfo appInfo = {};
  appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.pApplicationName = m_yaveInstanceParams.applicationName.c_str();
  appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.pEngineName = "Yave";
  appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.apiVersion = VK_API_VERSION_1_0;

  VkInstanceCreateInfo createInfo = {};
  createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  createInfo.pApplicationInfo = &appInfo;

  createInfo.enabledExtensionCount = static_cast<uint32_t>
    m_yaveInstanceParams.instanceExtensions.size()
  createInfo.ppEnabledExtensionNames = m_yaveInstanceParams.instanceExtensions.data();
  createInfo.enabledLayerCount = static_cast<uint32_t>
    m_yaveInstanceParams.validationLayers.size();
  createInfo.ppEnabledLayerNames = m_yaveInstanceParams.validationLayers.data();

  if (vkCreateInstance(&createInfo, nullptr, &m_instance) != VK_SUCCESS)
    throw YaveLib::FatalVulkanInitError("failed to create instance!");
}

void	YaveRenderer::choosePhysicalDevice() //TODO: Implementation (page 63)
{
  int                 i;
  uint32_t            deviceCount;
  VkPhysicalDevice    *device;

  vkEnumeratePhysicalDevices(m_instance, &deviceCount, NULL);
  if (deviceCount == 0)
    YaveLib::FatalVulkanInitError("failed to find GPU with Vulkan support!");

  std::multimap<int, gpuInfo_t>	gpuCanditates;
  std::vector<VkPhysicalDevice>	devices(deviceCount);

  vkEnumeratePhysicalDevices(m_instance, &device_count, devices.data());
  if (device_count == 0)
    YaveLib::FatalVulkanInitError("failed to find GPU with Vulkan support!");

  for (i = 0; i < deviceCount; ++i)
  {
  }

  m_dev_phy = VK_NULL_HANDLE;
  while (i < device_count && m_dev_phy == NULL)
  {
    if(is_device_suitable(device[i], m_viewInfo.surface))
      m_dev_phy = device[i];
    i++;
  }
  if (m_dev_phy == VK_NULL_HANDLE)
  {
    printf("failed to find a suitable GPU!");
    return (-1);
  }
}

void	YaveRenderer::createLogicalDeviceAndQueue()
{
  std::vector<int>                      queueIdx;
  std::vector<VkDeviceQueueCreateInfo>  queueInfo;

  queueIdx.push_back(vkContext.presentFamilyIdx);
  queueIdx.push_back(vkContext.graphicsFamilyIdx);
  const float	priority = 1.0f;
  for (int i = 0; i < queueIdx.size(); ++i)
  {
    VkDeviceQueueCreateInfo	qInfo = {};
    qInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    qInfo.FamilyIndex = queueIdx[i];
    qInfo.queueCount = 1;
    qInfo.pQueuePriorities = &priority;
    devqInfo.push_back(qInfo);
  }

  VkDeviceCreateInfo	createInfo;
  createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
  createInfo.queueCreateInfoCount = devqInfo.size();
  createInfo.pQueueCreateInfos = devqInfo.data();
  createInfo.pEnabledFeatures = m_yaveInstanceParams.deviceFeatures;
  createInfo.enabledExtensionCount = static_cast<uint32_t>
    m_yaveInstanceParams.deviceExtensions.size();
  createInfo.ppEnabledExtensionCount = m_yaveInstanceParams.deviceExtensions.data();
  createInfo.enabledLayerCount = static_cast<uint32_t>
    m_yaveInstanceParams.validationLayers.size();
  createInfo.enabledLayerCount = m_yaveInstanceParams.validationLayers.data();

  if (vkCreateDevice(m_physicalDevice, &createInfo
        , m_yaveInstanceParams.allocatorCallbacks, &vkContext.device) != VK_SUCCESS
    throw YaveLib::FatalVulkanInitError("failed to create logical device and queues!");

  vkGetDeviceQueue(vkContext.device, vkContext.graphicsFamilyIdx, 0, &vkContext.graphicsQueue);
  vkGetDeviceQueue(vkContext.device, vkContext.presentFamilyIdx, 0, &vkContext.presentQueue);
}

void	YaveRenderer::createCommandPool()
{
  VkCommandPoolCreateInfo		commandPoolCreateInfo = {};
  commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
  commandPoolCreateInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
  commandPoolCreateInfo.queueFamilyIndex = vkContext.graphicsFamilyIdx;

  if (vkCreateCommandPool(vkContext.device, &commandPoolCreateInfo
        , vkContext.allocatorCallbacks, &m_commandPool) != VK_SUCCESS)
  throw YaveLib::FatalVulkanInitError("failed to create command pool!");
}

void	YaveRenderer::createCommandBuffers()
{
  VkCommandBufferAllocateInfo commandBufferAllocateInfo = {};
  commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  commandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  commandBufferAllocateInfo.commandPool = m_commandPool;
  commandBufferAllocateInfo.commandBufferCount = vkContext.frameCount;

  if (vkAllocateCommandBuffers(vkContext.device, &commandBufferAllocateInfo
        , m_commandBuffers.data()) != VK_SUCCESS)
    throw YaveLib::FatalVulkanInitError("failed to allocate command buffer!");

  VkFenceCreateInfo fenceCreateInfo = {};
  fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;

  for (int i = 0; i < vkContext.frameCount; ++i)
  {
    if (vkCreateFence(vkContext.device, &fenceCreateInfo
          , vkContext.allocatorCallbacks, &m_commandBufferFences[ i ]) != VK_SUCCESS)
      throw YaveLib::FatalVulkanInitError("failed to allocate command buffer fences!");
  }
}

void	YaveRenderer::createSemaphores()
{
  VkSemaphoreCreateInfo semaphoreCreateInfo = {};
  semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

  for (int i = 0; i < vkContext.frameCount; ++i)
  {
    if (vkCreateSemaphore(vkContext.device, &semaphoreCreateInfo
          , vkContext.allocatorCallbacks, &m_acquireSemaphores[i]) != VK_SUCCESS)
      throw YaveLib::FatalVulkanInitError("failed to creates semaphores!");
    if (vkCreateSemaphore(vkContext.device, &semaphoreCreateInfo
          , vkContext.allocatorCallbacks, &m_renderCompleteSemaphores[i]) != VK_SUCCESS)
      throw YaveLib::FatalVulkanInitError("failed to creates semaphores!");
  }
}

/*
   void YaveRenderer::CreateQueryPool()
   {
   VkQueryPoolCreateInfo createInfo = {};
   createInfo.sType = VK_STRUCTURE_TYPE_QUERY_POOL_CREATE_INFO;
   createInfo.queryType = VK_QUERY_TYPE_TIMESTAMP;
   createInfo.queryCount = NUM_TIMESTAMP_QUERIES;

   for (int i = 0; i < vkContext.frameCount; ++i)
   {
   if (vkCreateQueryPool( vkContext.device, &createInfo
   , vkContext.allocatorCallbacks, &m_queryPools[i]) != VK_SUCCESS)
   throw YaveLib::FatalVulkanInitError("failed to creates query pools!");
   }
   }
   */

void	YaveRenderer::createSurface()
{
  if (m_surfaceHandler.createSurface(m_instance,
        m_yaveInstanceParams.allocatorCallbacks, m_viewInfo.surface) != VK_SUCCESS)
    throw YaveLib::FatalVulkanInitError("failed to create surface!");
}

void	YaveRenderer::createSwapchain()
{
  if (m_swapchainHandler.createSwapchain(m_viewInfo) != VK_SUCCESS)
    throw YaveLib::FatalVulkanInitError("failed to create swapchain!");
  vkContext.frameCount = m_viewInfo.frameCount
}

void	YaveRenderer::createImageViews()
{
  if (m_imageViewsHandler.createImageViews(m_viewInfo) != VK_SUCCESS)
    throw YaveLib::FatalVulkanInitError("failed to create image views!");
}

void	YaveRenderer::createRenderPass()
{
  if (m_renderPassHandler.createRenderPass(m_viewInfo) != VK_SUCCESS)
    throw YaveLib::FatalVulkanInitError("failed to create render pass!");
}

void	YaveRenderer::createFramebuffers()
{
  if (m_framebuffersHandler.createFramebuffers(m_viewInfo) != VK_SUCCESS)
    throw YaveLib::FatalVulkanInitError("failed to create frame buffers!");
}
