//
// Created by Gabriel Lugo on 3/30/2021.
//

#include "HallowPipeline.hpp"

#include <fstream>
#include <iostream>
#include <stdexcept>
#include <engine/model/HallowModel.hpp>


namespace Hallow {
  HallowPipeline::~HallowPipeline() {
    vkDestroyShaderModule(m_device.device(), m_vert_shader_module, nullptr);
    vkDestroyShaderModule(m_device.device(), m_frag_shader_module, nullptr);

    vkDestroyPipeline(m_device.device(), m_graphics_pipeline, nullptr);
  }

  void HallowPipeline::init(
      HallowDevice& device,
      const PipelineConfigInfo& pipelineConfigData,
      const std::string& vertFilePath,
      const std::string& fragFilePath) {
    try {
      createGraphicsPipeline(pipelineConfigData, vertFilePath, fragFilePath);
    } catch (const std::exception& e) {
      std::cerr << e.what() << "\n";
      std::terminate();
    }
  }

  std::vector<char> HallowPipeline::readFile(const std::string& filePath) {
    std::ifstream file{
        filePath, std::ios::ate | std::ios::binary
    }; // ate says to skip to onEnd of file
    if (!file.is_open()) {
      throw std::runtime_error("HallowPipeline: Failed to open file: " + filePath);
    }

    size_t fileSize = static_cast<size_t>(file.tellg()); // tellg reports position
    std::vector<char> buffer(fileSize);

    file.seekg(0); // go back to beginning
    file.read(buffer.data(), fileSize); // read for as many bits as file size

    file.close();
    return buffer;
  }

  void HallowPipeline::createGraphicsPipeline(
      const PipelineConfigInfo& pipelineConfigInfo, const std::string& vertFilePath, const std::string& fragFilePath) {
    std::cout << "Creating graphics pipeline...\n";

    if (pipelineConfigInfo.pipeline_layout == VK_NULL_HANDLE) {
      throw std::runtime_error("HallowPipeline: Cannot create graphics pipeline. No pipeline_layout was provided!");
    }
    if (pipelineConfigInfo.render_pass == VK_NULL_HANDLE) {
      throw std::runtime_error("HallowPipeline: Cannot create graphics pipeline. No m_render_pass was provided!");
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

    //VkPipelineViewportStateCreateInfo viewport_info{};

    // how we interpret vertex buffer data
    auto attribute_descriptions = HallowModel::Vertex::attributeDescriptions();
    auto binding_descriptions = HallowModel::Vertex::bindingDescriptions();
    VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attribute_descriptions.size());
    vertexInputInfo.vertexBindingDescriptionCount = static_cast<uint32_t>(binding_descriptions.size());
    vertexInputInfo.pVertexAttributeDescriptions = attribute_descriptions.data();
    vertexInputInfo.pVertexBindingDescriptions = binding_descriptions.data();

    VkGraphicsPipelineCreateInfo pipelineInfo{};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = stageCount;
    pipelineInfo.pStages = shaderStages;

    pipelineInfo.pVertexInputState = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &pipelineConfigInfo.input_assembly_info;
    pipelineInfo.pViewportState = &pipelineConfigInfo.viewport_info;
    pipelineInfo.pRasterizationState = &pipelineConfigInfo.rasterization_info;
    pipelineInfo.pMultisampleState = &pipelineConfigInfo.multisample_info;
    pipelineInfo.pColorBlendState = &pipelineConfigInfo.color_blend_info;
    pipelineInfo.pDepthStencilState = &pipelineConfigInfo.depth_stencil_info;
    pipelineInfo.pDynamicState = nullptr;

    pipelineInfo.layout = pipelineConfigInfo.pipeline_layout;
    pipelineInfo.renderPass = pipelineConfigInfo.render_pass;
    pipelineInfo.basePipelineIndex = -1;
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

    if (vkCreateGraphicsPipelines(m_device.device(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_graphics_pipeline) !=
        VK_SUCCESS) {
      throw std::runtime_error("HallowPipeline: Failed to create graphics pipeline!");
    }

    std::cout << "Graphics pipeline created!\n";
  }

  void HallowPipeline::createShaderModule(
      const std::vector<char>& shaderCode, VkShaderModule* shaderModule) {
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = shaderCode.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(shaderCode.data());

    if (vkCreateShaderModule(m_device.device(), &createInfo, nullptr, shaderModule) != VK_SUCCESS) {
      throw std::runtime_error("HallowPipeline: Failed to create shader module!");
    }
  }

  void HallowPipeline::defaultPipelineConfig(
      PipelineConfigInfo& pipelineConfigInfo, uint32_t width, uint32_t height) {
    pipelineConfigInfo.input_assembly_info.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    pipelineConfigInfo.input_assembly_info.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    pipelineConfigInfo.input_assembly_info.primitiveRestartEnable = VK_FALSE;

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

    pipelineConfigInfo.viewport_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    pipelineConfigInfo.viewport_info.viewportCount = 1;
    pipelineConfigInfo.viewport_info.pViewports = &pipelineConfigInfo.viewport;
    pipelineConfigInfo.viewport_info.scissorCount = 1;
    pipelineConfigInfo.viewport_info.pScissors = &pipelineConfigInfo.scissor;

    // breaks up geometry into pixels
    pipelineConfigInfo.rasterization_info.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    pipelineConfigInfo.rasterization_info.depthClampEnable = VK_FALSE;
    pipelineConfigInfo.rasterization_info.rasterizerDiscardEnable = VK_FALSE;
    pipelineConfigInfo.rasterization_info.polygonMode = VK_POLYGON_MODE_FILL;
    pipelineConfigInfo.rasterization_info.lineWidth = 1.0f;
    pipelineConfigInfo.rasterization_info.cullMode = VK_CULL_MODE_BACK_BIT;
    pipelineConfigInfo.rasterization_info.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    pipelineConfigInfo.rasterization_info.depthBiasEnable = VK_FALSE;
    pipelineConfigInfo.rasterization_info.depthBiasConstantFactor = 0.0f;  // Optional
    pipelineConfigInfo.rasterization_info.depthBiasClamp = 0.0f;           // Optional
    pipelineConfigInfo.rasterization_info.depthBiasSlopeFactor = 0.0f;     // Optional

    // how rasterizer handles edges of geometry. without it, pixels are either in or out depending on center
    pipelineConfigInfo.multisample_info.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    pipelineConfigInfo.multisample_info.sampleShadingEnable = VK_FALSE;
    pipelineConfigInfo.multisample_info.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    pipelineConfigInfo.multisample_info.minSampleShading = 1.0f;           // Optional
    pipelineConfigInfo.multisample_info.pSampleMask = nullptr;             // Optional
    pipelineConfigInfo.multisample_info.alphaToCoverageEnable = VK_FALSE;  // Optional
    pipelineConfigInfo.multisample_info.alphaToOneEnable = VK_FALSE;       // Optional

    // how we combine colors in frame buffer
    pipelineConfigInfo.color_blend_attachment_state.colorWriteMask = VK_COLOR_COMPONENT_R_BIT |
                                                                     VK_COLOR_COMPONENT_G_BIT |
                                                                     VK_COLOR_COMPONENT_B_BIT |
                                                                     VK_COLOR_COMPONENT_A_BIT;
    pipelineConfigInfo.color_blend_attachment_state.blendEnable = VK_FALSE;
    pipelineConfigInfo.color_blend_attachment_state.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;   // Optional
    pipelineConfigInfo.color_blend_attachment_state.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;  // Optional
    pipelineConfigInfo.color_blend_attachment_state.colorBlendOp = VK_BLEND_OP_ADD;              // Optional
    pipelineConfigInfo.color_blend_attachment_state.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;   // Optional
    pipelineConfigInfo.color_blend_attachment_state.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;  // Optional
    pipelineConfigInfo.color_blend_attachment_state.alphaBlendOp = VK_BLEND_OP_ADD;              // Optional

    pipelineConfigInfo.color_blend_info.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    pipelineConfigInfo.color_blend_info.logicOpEnable = VK_FALSE;
    pipelineConfigInfo.color_blend_info.logicOp = VK_LOGIC_OP_COPY;  // Optional
    pipelineConfigInfo.color_blend_info.attachmentCount = 1;
    pipelineConfigInfo.color_blend_info.pAttachments = &pipelineConfigInfo.color_blend_attachment_state;
    pipelineConfigInfo.color_blend_info.blendConstants[0] = 0.0f;  // Optional
    pipelineConfigInfo.color_blend_info.blendConstants[1] = 0.0f;  // Optional
    pipelineConfigInfo.color_blend_info.blendConstants[2] = 0.0f;  // Optional
    pipelineConfigInfo.color_blend_info.blendConstants[3] = 0.0f;  // Optional

    // depth testing https://youtu.be/ecMcXW6MSYU?t=524
    pipelineConfigInfo.depth_stencil_info.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    pipelineConfigInfo.depth_stencil_info.depthTestEnable = VK_TRUE;
    pipelineConfigInfo.depth_stencil_info.depthWriteEnable = VK_TRUE;
    pipelineConfigInfo.depth_stencil_info.depthCompareOp = VK_COMPARE_OP_LESS;
    pipelineConfigInfo.depth_stencil_info.depthBoundsTestEnable = VK_FALSE;
    pipelineConfigInfo.depth_stencil_info.minDepthBounds = 0.0f;  // Optional
    pipelineConfigInfo.depth_stencil_info.maxDepthBounds = 1.0f;  // Optional
    pipelineConfigInfo.depth_stencil_info.stencilTestEnable = VK_FALSE;
    pipelineConfigInfo.depth_stencil_info.front = {};  // Optional
    pipelineConfigInfo.depth_stencil_info.back = {};   // Optional
  }

  void HallowPipeline::bind(VkCommandBuffer commandBuffer) {
    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_graphics_pipeline);
  }
}