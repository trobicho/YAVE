#include "pipelineCreationTest.h"
#include "../core/YaveRenderRenderProgs.h"

//--------------Basic Fixed Function---------------

static VkPipelineVertexInputStateCreateInfo     vertexInputStateInfo_noInput()
{
  VkPipelineVertexInputStateCreateInfo	vertexInput = {};
  vertexInput.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
  vertexInput.vertexBindingDescriptionCount = 0;
  vertexInput.pVertexBindingDescriptions = nullptr; // Optional
  vertexInput.vertexAttributeDescriptionCount = 0;
  vertexInput.pVertexAttributeDescriptions = nullptr; // Optional
  return (vertexInput);
}

static VkPipelineInputAssemblyStateCreateInfo   inputAssemblyStateInfo()
{
  VkPipelineInputAssemblyStateCreateInfo	inputAssembly = {};
  inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
  inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
  inputAssembly.primitiveRestartEnable = VK_FALSE;
  return (inputAssembly);
}

static VkPipelineRasterizationStateCreateInfo   rasterizationStateInfo()
{
  VkPipelineRasterizationStateCreateInfo rasterizer = {};
  rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
  rasterizer.depthClampEnable = VK_FALSE;
  rasterizer.rasterizerDiscardEnable = VK_FALSE;
  rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
  rasterizer.lineWidth = 1.0f;
  rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
  rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
  rasterizer.depthBiasEnable = VK_FALSE;
  rasterizer.depthBiasConstantFactor = 0.0f; // Optional
  rasterizer.depthBiasClamp = 0.0f; // Optional
  rasterizer.depthBiasSlopeFactor = 0.0f; // Optional
  return (rasterizer);
}

static VkPipelineViewportStateCreateInfo        viewportStateInfo()
{
  VkPipelineViewportStateCreateInfo   viewportInfo = {};

  viewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
  viewportInfo.viewportCount = 1;
  viewportInfo.scissorCount = 1;
  return (viewportInfo);
}

static VkPipelineMultisampleStateCreateInfo     multisampleStateInfo()
{
  VkPipelineMultisampleStateCreateInfo multisampling{};
  multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
  multisampling.sampleShadingEnable = VK_FALSE;
  multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
  multisampling.minSampleShading = 1.0f; // Optional
  multisampling.pSampleMask = nullptr; // Optional
  multisampling.alphaToCoverageEnable = VK_FALSE; // Optional
  multisampling.alphaToOneEnable = VK_FALSE; // Optional
  return (multisampling);
}

static VkPipelineColorBlendStateCreateInfo      colorBlendStateInfo_noBlending()
{
  VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
  colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT
    | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
  colorBlendAttachment.blendEnable = VK_FALSE;
  colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE; //Optional
  colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; //Optional
  colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD; // Optional
  colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE; //Optional
  colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; //Optional
  colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD; // Optional

  VkPipelineColorBlendStateCreateInfo colorBlending = {};
  colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
  colorBlending.logicOpEnable = VK_FALSE;
  colorBlending.logicOp = VK_LOGIC_OP_COPY; // Optional
  colorBlending.attachmentCount = 1;
  colorBlending.pAttachments = &colorBlendAttachment;
  colorBlending.blendConstants[0] = 0.0f; // Optional
  colorBlending.blendConstants[1] = 0.0f; // Optional
  colorBlending.blendConstants[2] = 0.0f; // Optional
  colorBlending.blendConstants[3] = 0.0f; // Optional
  return (colorBlending);
}

//-----------------------------------------

VkPipeline  BasicPipelineTest::createGraphicsPipeline(VkPipelineLayout &pipelineLayout)
{
  VkShaderModule	vertShaderModule = YaveRendererHelper::createShaderModule("../../test.vert");
  VkShaderModule	fragShaderModule = YaveRendererHelper::createShaderModule("../../test.frag");

  VkPipelineVertexInputStateCreateInfo    vertexInputInfo = vertexInputStateInfo_noInput();
  VkPipelineInputAssemblyStateCreateInfo	inputAssemblyInfo = inputAssemblyStateInfo();
  VkPipelineRasterizationStateCreateInfo	rasterizationInfo = rasterizationStateInfo();
  VkPipelineMultisampleStateCreateInfo    multisampleInfo = multisampleStateInfo();
  VkPipelineColorBlendStateCreateInfo     colorBlendInfo = colorBlendStateInfo();
  VkPipelineViewportStateCreateInfo       viewportInfo = viewportStateInfo();
  VkDynamicState dynamicStates[] = {
    VK_DYNAMIC_STATE_VIEWPORT,
    VK_DYNAMIC_STATE_SCISSOR
  };
  VkPipelineDynamicStateCreateInfo dynamicStateInfo{};
  dynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
  dynamicStateInfo.dynamicStateCount = 2;
  dynamicStateInfo.pDynamicStates = dynamicStates;

  VkPipelineShaderStageCreateInfo	vertShaderStageInfo = {};
  vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
  vertShaderStageInfo.module = vertShaderModule;
  vertShaderStageInfo.pName = "main"; //TODO: entrypoint
  vertShaderStageInfo.pSpecializationInfo = nullptr;
  VkPipelineShaderStageCreateInfo	fragShaderStageInfo = {};
  vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  vertShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
  vertShaderStageInfo.module = fragShaderModule;
  vertShaderStageInfo.pName = "main"; //TODO: entrypoint
  vertShaderStageInfo.pSpecializationInfo = nullptr;
  VkPipelineShaderStageCreateInfo	shaderStages[] = {vertShaderStageInfo, fragShaderStageInfo};

  VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
  pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  pipelineLayoutInfo.setLayoutCount = 0; // Optional
  pipelineLayoutInfo.pSetLayouts = nullptr; // Optional
  pipelineLayoutInfo.pushConstantRangeCount = 0; // Optional
  pipelineLayoutInfo.pPushConstantRanges = nullptr; // Optional

  if (vkCreatePipelineLayout(vkContext.device, &pipelineLayoutInfo
        , vkContext.allocatorCallbacks, &pipelineLayout) != VK_SUCCESS)
    throw YaveLib::Error("failed to create pipeline layout!");

  VkGraphicsPipelineCreateInfo  pipelineInfo{};
  pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
  pipelineInfo.stageCount = 2;
  pipelineInfo.pStages = shaderStages;
  pipelineInfo.pVertexInputState = &vertexInputInfo;
  pipelineInfo.pInputAssemblyState = &inputAssemblyInfo;
  pipelineInfo.pViewportState = &viewportInfo;
  pipelineInfo.pRasterizationState = &rasterizerInfo;
  pipelineInfo.pMultisampleState = &multisamplingInfo;
  pipelineInfo.pDepthStencilState = nullptr; // Optional
  pipelineInfo.pColorBlendState = &colorBlendInfo;
  pipelineInfo.pDynamicState = &dynamicStateInfo;

  VkPipeline  pipeline;
  if (vkCreateGraphicsPipelines(vkContext.device, VK_NULL_HANDLE, 1,
        &pipelineInfo, vkContext.allocatorCallbacks, &pipeline) != VK_SUCCESS)
    throw YaveLib::Error("failed to create graphics pipeline!");

  vkDestroyShaderModule(vkContext.device, vertShaderModule, vkContext.allocatorCallbacks);
  vkDestroyShaderModule(vkContext.device, fragShaderModule, vkContext.allocatorCallbacks);
  return (pipeline);
}

void  BasicPipelineTest::destroyPipeline(VkPipeline  &pipeline
    , VkPipelineLayout &pipelineLayout);
{
  vkDestroyPipeline(device, pipeline, nullptr);
  vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
}
