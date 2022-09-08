#pragma once
#include <cstdint>
#include <vector>
#include <vulkan/vulkan.h>

struct	renderProgLimitInfo_t
{
  const static uint32_t maxDescSets = 16384;
  const static uint32_t maxDescSetWrites = 32;
  const static uint32_t maxDescSetUniforms = 48;
};

struct	renderProg_t
{
  struct	pipelineState_t
  {
    pipelineState_t(): stateBits(0), pipeline(VK_NULL_HANDLE) {};

    uint64_t	stateBits;
    VkPipeline	pipeline;
  };

  std::vector<pipelineState_t>  pipelines;
  VkPipelineLayout              pipelineLayout;
};

class	YaveRenderProgManager
{
  public:
    YaveRenderProgManager(){};
    ~YaveRenderProgManager(){};

    void    Init();

    const renderProg_t&   getCurrentRenderProg() const {
      return m_renderProgs[m_currentRenderProg];
    }

  private:
    std::vector<renderProg_t>	m_renderProgs;

    int m_currentRenderProg;

    std::vector<VkDescriptorPool>               m_descriptorPools;
    std::vector<std::vector<VkDescriptorSet>>   m_descriptorSets;
};

extern YaveRenderProgManager	renderProgManager;
