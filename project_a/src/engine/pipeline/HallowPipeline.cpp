//
// Created by Gabriel Lugo on 3/30/2021.
//

#include "HallowPipeline.h"

#include <fstream>
#include <iostream>
#include <stdexcept>


namespace Hallow {
  HallowPipeline::~HallowPipeline() {
    vkDestroyShaderModule(m_device.device(), m_vert_shader_module, nullptr);
    vkDestroyShaderModule(m_device.device(), m_frag_shader_module, nullptr);

    vkDestroyPipeline(m_device.device(), m_graphics_pipeline, nullptr);
  }

  void HallowPipeline::init(HallowDevice& device,
                            const PipelineConfigInfo& pipelineConfigData,
                            const std::string& vertFilePath,
                            const std::string& fragFilePath) {
    try {
      createGraphicsPipeline(pipelineConfigData, vertFilePath,
                             fragFilePath);
    } catch (const std::exception& e) {
      std::cerr << e.what() << "\n";
      std::terminate();
    }
  }

  std::vector<char> HallowPipeline::readFile(const std::string& filePath) {
    std::ifstream file{filePath, std::ios::ate |
                                 std::ios::binary}; // ate says to skip to end of file
    if (!file.is_open()) {
      throw std::runtime_error(
              "HallowPipeline: Failed to open file: " + filePath);
    }

    size_t fileSize = static_cast<size_t>(file.tellg()); // tellg reports position
    std::vector<char> buffer(fileSize);

    file.seekg(0); // go back to beginning
    file.read(buffer.data(),
              fileSize); // read for as many bits as file size

    file.close();
    return buffer;
  }

  void HallowPipeline::createGraphicsPipeline(
          const PipelineConfigInfo& pipelineConfigInfo,
          const std::string& vertFilePath,
          const std::string& fragFilePath) {
    std::cout << "Creating graphics pipeline...\n";

    if (pipelineConfigInfo.pipelineLayout == VK_NULL_HANDLE) {
      throw std::runtime_error(
              "HallowPipeline: Cannot create graphics pipeline. No pipelineLayout was provided!");
    }
    if (pipelineConfigInfo.renderPass == VK_NULL_HANDLE) {
      throw std::runtime_error(
              "HallowPipeline: Cannot create graphics pipeline. No renderPass was provided!");
    }

    auto vertCode = readFile(vertFilePath);
    auto fragCode = readFile(fragFilePath);

    createShaderModule(vertCode, &m_vert_shader_module);
    createShaderModule(fragCode, &m_frag_shader_module);

    const int stageCount = 2;
    VkPipelineShaderStageCreateInfo shaderStages[stageCount];
    shaderStages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
    shaderStages[0].module = m_vert_shader_module;
    shaderStages[0].pName = "main";
    shaderStages[0].flags = 0;
    shaderStages[0].pNext = nullptr;
    shaderStages[0].pSpecializationInfo = nullptr;

    shaderStages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    shaderStages[1].module = m_frag_shader_module;
    shaderStages[1].pName = "main";
    shaderStages[1].flags = 0;
    shaderStages[1].pNext = nullptr;
    shaderStages[1].pSpecializationInfo = nullptr;

    //VkPipelineViewportStateCreateInfo viewportInfo{};

    // how we interpret vertex buffer data
    VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexAttributeDescriptionCount = 0;
    vertexInputInfo.vertexBindingDescriptionCount = 0;
    vertexInputInfo.pVertexAttributeDescriptions = nullptr;
    vertexInputInfo.pVertexBindingDescriptions = nullptr;

    VkGraphicsPipelineCreateInfo pipelineInfo{};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = stageCount;
    pipelineInfo.pStages = shaderStages;

    pipelineInfo.pVertexInputState = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &pipelineConfigInfo.inputAssemblyInfo;
    pipelineInfo.pViewportState = &pipelineConfigInfo.viewportInfo;
    pipelineInfo.pRasterizationState = &pipelineConfigInfo.rasterizationInfo;
    pipelineInfo.pMultisampleState = &pipelineConfigInfo.multisampleInfo;
    pipelineInfo.pColorBlendState = &pipelineConfigInfo.colorBlendInfo;
    pipelineInfo.pDepthStencilState = &pipelineConfigInfo.depthStencilInfo;
    pipelineInfo.pDynamicState = nullptr;

    pipelineInfo.layout = pipelineConfigInfo.pipelineLayout;
    pipelineInfo.renderPass = pipelineConfigInfo.renderPass;
    pipelineInfo.basePipelineIndex = -1;
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

    if (vkCreateGraphicsPipelines(m_device.device(),
                                  VK_NULL_HANDLE,
                                  1,
                                  &pipelineInfo,
                                  nullptr,
                                  &m_graphics_pipeline) != VK_SUCCESS) {
      throw std::runtime_error(
              "HallowPipeline: Failed to create graphics pipeline!");
    }

    std::cout << "Graphics pipeline created!\n";
  }

  void HallowPipeline::createShaderModule(const std::vector<char>& shaderCode,
                                          VkShaderModule* shaderModule) {
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = shaderCode.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(shaderCode.data());

    if (vkCreateShaderModule(m_device.device(), &createInfo, nullptr,
                             shaderModule) != VK_SUCCESS) {
      throw std::runtime_error(
              "HallowPipeline: Failed to create shader module!");
    }
  }

  void HallowPipeline::defaultPipelineConfig(
          PipelineConfigInfo& pipelineConfigInfo, uint32_t width,
          uint32_t height) {
    pipelineConfigInfo.inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    pipelineConfigInfo.inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    pipelineConfigInfo.inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;

    // viewport tells how we want to transform gl_Position values from shader into output image
    pipelineConfigInfo.viewport.x = 0.0f;
    pipelineConfigInfo.viewport.y = 0.0f;
    pipelineConfigInfo.viewport.width = static_cast<float>(width);
    pipelineConfigInfo.viewport.height = static_cast<float>(height);
    pipelineConfigInfo.viewport.minDepth = 0.0f;
    pipelineConfigInfo.viewport.maxDepth = 1.0f;

    // scissor discards any pixels outside of the defined rectangle
    pipelineConfigInfo.scissor.offset = {0, 0};
    pipelineConfigInfo.scissor.extent = {width, height};

    pipelineConfigInfo.viewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    pipelineConfigInfo.viewportInfo.viewportCount = 1;
    pipelineConfigInfo.viewportInfo.pViewports = &pipelineConfigInfo.viewport;
    pipelineConfigInfo.viewportInfo.scissorCount = 1;
    pipelineConfigInfo.viewportInfo.pScissors = &pipelineConfigInfo.scissor;

    // breaks up geometry into pixels
    pipelineConfigInfo.rasterizationInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    pipelineConfigInfo.rasterizationInfo.depthClampEnable = VK_FALSE;
    pipelineConfigInfo.rasterizationInfo.rasterizerDiscardEnable = VK_FALSE;
    pipelineConfigInfo.rasterizationInfo.polygonMode = VK_POLYGON_MODE_FILL;
    pipelineConfigInfo.rasterizationInfo.lineWidth = 1.0f;
    pipelineConfigInfo.rasterizationInfo.cullMode = VK_CULL_MODE_BACK_BIT;
    pipelineConfigInfo.rasterizationInfo.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    pipelineConfigInfo.rasterizationInfo.depthBiasEnable = VK_FALSE;
    pipelineConfigInfo.rasterizationInfo.depthBiasConstantFactor = 0.0f;  // Optional
    pipelineConfigInfo.rasterizationInfo.depthBiasClamp = 0.0f;           // Optional
    pipelineConfigInfo.rasterizationInfo.depthBiasSlopeFactor = 0.0f;     // Optional

    // how rasterizer handles edges of geometry. without it, pixels are either in or out depending on center
    pipelineConfigInfo.multisampleInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    pipelineConfigInfo.multisampleInfo.sampleShadingEnable = VK_FALSE;
    pipelineConfigInfo.multisampleInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    pipelineConfigInfo.multisampleInfo.minSampleShading = 1.0f;           // Optional
    pipelineConfigInfo.multisampleInfo.pSampleMask = nullptr;             // Optional
    pipelineConfigInfo.multisampleInfo.alphaToCoverageEnable = VK_FALSE;  // Optional
    pipelineConfigInfo.multisampleInfo.alphaToOneEnable = VK_FALSE;       // Optional

    // how we combine colors in frame buffer
    pipelineConfigInfo.colorBlendAttachment.colorWriteMask =
            VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
            VK_COLOR_COMPONENT_B_BIT |
            VK_COLOR_COMPONENT_A_BIT;
    pipelineConfigInfo.colorBlendAttachment.blendEnable = VK_FALSE;
    pipelineConfigInfo.colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;   // Optional
    pipelineConfigInfo.colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;  // Optional
    pipelineConfigInfo.colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;              // Optional
    pipelineConfigInfo.colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;   // Optional
    pipelineConfigInfo.colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;  // Optional
    pipelineConfigInfo.colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;              // Optional

    pipelineConfigInfo.colorBlendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    pipelineConfigInfo.colorBlendInfo.logicOpEnable = VK_FALSE;
    pipelineConfigInfo.colorBlendInfo.logicOp = VK_LOGIC_OP_COPY;  // Optional
    pipelineConfigInfo.colorBlendInfo.attachmentCount = 1;
    pipelineConfigInfo.colorBlendInfo.pAttachments = &pipelineConfigInfo.colorBlendAttachment;
    pipelineConfigInfo.colorBlendInfo.blendConstants[0] = 0.0f;  // Optional
    pipelineConfigInfo.colorBlendInfo.blendConstants[1] = 0.0f;  // Optional
    pipelineConfigInfo.colorBlendInfo.blendConstants[2] = 0.0f;  // Optional
    pipelineConfigInfo.colorBlendInfo.blendConstants[3] = 0.0f;  // Optional

    // depth testing https://youtu.be/ecMcXW6MSYU?t=524
    pipelineConfigInfo.depthStencilInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    pipelineConfigInfo.depthStencilInfo.depthTestEnable = VK_TRUE;
    pipelineConfigInfo.depthStencilInfo.depthWriteEnable = VK_TRUE;
    pipelineConfigInfo.depthStencilInfo.depthCompareOp = VK_COMPARE_OP_LESS;
    pipelineConfigInfo.depthStencilInfo.depthBoundsTestEnable = VK_FALSE;
    pipelineConfigInfo.depthStencilInfo.minDepthBounds = 0.0f;  // Optional
    pipelineConfigInfo.depthStencilInfo.maxDepthBounds = 1.0f;  // Optional
    pipelineConfigInfo.depthStencilInfo.stencilTestEnable = VK_FALSE;
    pipelineConfigInfo.depthStencilInfo.front = {};  // Optional
    pipelineConfigInfo.depthStencilInfo.back = {};   // Optional
  }

  void HallowPipeline::bind(VkCommandBuffer commandBuffer) {
    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
                      m_graphics_pipeline);
  }
}