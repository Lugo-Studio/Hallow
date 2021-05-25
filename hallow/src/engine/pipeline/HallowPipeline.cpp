//
// Created by Gabriel Lugo on 3/30/2021.
//

#include "HallowPipeline.hpp"

#include <fstream>
#include <iostream>
#include <stdexcept>
#include <engine/model/HallowModel.hpp>
#include "glslang/Include/ShHandle.h"
#include "glslang/Public/ShaderLang.h"
#include "glslang/SPIRV/GlslangToSpv.h"
#include "glslang/Include/BaseTypes.h"
#include "glslang/Include/Common.h"


namespace Hallow {
  HallowPipeline::~HallowPipeline() {
    vkDestroyShaderModule(m_device.device(), m_vert_shader_module, nullptr);
    vkDestroyShaderModule(m_device.device(), m_frag_shader_module, nullptr);

    vkDestroyPipeline(m_device.device(), m_graphics_pipeline, nullptr);
  }

  void HallowPipeline::init(HallowDevice& device,
                            const PipelineConfigInfo& pipeline_config_data,
                            const std::string& vert_file_path,
                            const std::string& frag_file_path) {
    try {
      createGraphicsPipeline(pipeline_config_data, vert_file_path, frag_file_path);
    } catch (const std::exception& e) {
      std::cerr << e.what() << "\n";
      std::terminate();
    }
  }

  void HallowPipeline::init2(HallowDevice& device,
                             const PipelineConfigInfo& pipeline_config_data,
                             const std::string& vert_file_path,
                             const std::string& frag_file_path) {
    try {
      createGraphicsPipeline(pipeline_config_data, vert_file_path, frag_file_path);
    } catch (const std::exception& e) {
      std::cerr << e.what() << "\n";
      std::terminate();
    }

  }

  std::vector<char> HallowPipeline::readFile(const std::string& file_path) {
    std::ifstream file{
      file_path, std::ios::ate | std::ios::binary
    }; // ate says to skip to onEnd of file
    if (!file.is_open()) {
      throw std::runtime_error("HallowPipeline: Failed to open file: " + file_path);
    }

    size_t fileSize = static_cast<size_t>(file.tellg()); // tellg reports position
    std::vector<char> buffer(fileSize);

    file.seekg(0); // go back to beginning
    file.read(buffer.data(), fileSize); // read for as many bits as file size

    file.close();
    return buffer;
  }

  void HallowPipeline::createGraphicsPipeline(
    const PipelineConfigInfo& pipeline_config_info,
    const std::string& vert_file_path,
    const std::string& frag_file_path) {
    std::cout << "Creating graphics pipeline...\n";

    if (pipeline_config_info.pipeline_layout == VK_NULL_HANDLE) {
      throw std::runtime_error("HallowPipeline: Cannot create graphics pipeline. No pipeline_layout was provided!");
    }
    if (pipeline_config_info.render_pass == VK_NULL_HANDLE) {
      throw std::runtime_error("HallowPipeline: Cannot create graphics pipeline. No m_render_pass was provided!");
    }

    auto vert_code = readFile(vert_file_path);
    auto frag_code = readFile(frag_file_path);

    createShaderModule(vert_code, &m_vert_shader_module);
    createShaderModule(frag_code, &m_frag_shader_module);

    const int stage_count = 2;
    VkPipelineShaderStageCreateInfo shader_stages[stage_count];
    shader_stages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shader_stages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
    shader_stages[0].module = m_vert_shader_module;
    shader_stages[0].pName = "main";
    shader_stages[0].flags = 0;
    shader_stages[0].pNext = nullptr;
    shader_stages[0].pSpecializationInfo = nullptr;

    shader_stages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shader_stages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    shader_stages[1].module = m_frag_shader_module;
    shader_stages[1].pName = "main";
    shader_stages[1].flags = 0;
    shader_stages[1].pNext = nullptr;
    shader_stages[1].pSpecializationInfo = nullptr;

    // how we interpret vertex buffer data
    auto attribute_descriptions = HallowModel::Vertex::attributeDescriptions();
    auto binding_descriptions = HallowModel::Vertex::bindingDescriptions();
    VkPipelineVertexInputStateCreateInfo vertex_input_info{};
    vertex_input_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertex_input_info.vertexAttributeDescriptionCount = static_cast<uint32_t>(attribute_descriptions.size());
    vertex_input_info.vertexBindingDescriptionCount = static_cast<uint32_t>(binding_descriptions.size());
    vertex_input_info.pVertexAttributeDescriptions = attribute_descriptions.data();
    vertex_input_info.pVertexBindingDescriptions = binding_descriptions.data();

    VkGraphicsPipelineCreateInfo pipeline_info{};
    pipeline_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipeline_info.stageCount = stage_count;
    pipeline_info.pStages = shader_stages;

    pipeline_info.pVertexInputState = &vertex_input_info;
    pipeline_info.pInputAssemblyState = &pipeline_config_info.input_assembly_info;
    pipeline_info.pViewportState = &pipeline_config_info.viewport_info;
    pipeline_info.pRasterizationState = &pipeline_config_info.rasterization_info;
    pipeline_info.pMultisampleState = &pipeline_config_info.multisample_info;
    pipeline_info.pColorBlendState = &pipeline_config_info.color_blend_info;
    pipeline_info.pDepthStencilState = &pipeline_config_info.depth_stencil_info;
    pipeline_info.pDynamicState = &pipeline_config_info.dynamic_state_info;

    pipeline_info.layout = pipeline_config_info.pipeline_layout;
    pipeline_info.renderPass = pipeline_config_info.render_pass;
    pipeline_info.basePipelineIndex = -1;
    pipeline_info.basePipelineHandle = VK_NULL_HANDLE;

    if (
      vkCreateGraphicsPipelines(m_device.device(), VK_NULL_HANDLE, 1, &pipeline_info, nullptr, &m_graphics_pipeline) !=
      VK_SUCCESS) {
      throw std::runtime_error("HallowPipeline: Failed to create graphics pipeline!");
    }

    std::cout << "Graphics pipeline created!\n";
  }

  void HallowPipeline::createShaderModule(const std::vector<char>& shader_code,
                                          VkShaderModule* shader_module) {
    VkShaderModuleCreateInfo create_info{};
    create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    create_info.codeSize = shader_code.size();
    create_info.pCode = reinterpret_cast<const uint32_t*>(shader_code.data());

    if (vkCreateShaderModule(m_device.device(), &create_info, nullptr, shader_module) != VK_SUCCESS) {
      throw std::runtime_error("HallowPipeline: Failed to create shader module!");
    }
  }

  void HallowPipeline::createShaderModule2(const std::vector<char>& vert_code,
                                           const std::vector<char>& frag_code,
                                           VkShaderModule* shader_module) {



    auto essl_version = glslang::GetEsslVersionString();
    auto glsl_version = glslang::GetGlslVersionString();


    glslang::TShader shader{EShLangVertex};
    shader.setStrings(reinterpret_cast<const char* const*>(vert_code.data()), 1);

    bool initialized = glslang::InitializeProcess();



    glslang::FinalizeProcess();







    VkShaderModuleCreateInfo create_info{};
    create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    create_info.codeSize = shader_code.size();
    create_info.pCode = reinterpret_cast<const uint32_t*>(shader_code.data());

    if (vkCreateShaderModule(m_device.device(), &create_info, nullptr, shader_module) != VK_SUCCESS) {
      throw std::runtime_error("HallowPipeline: Failed to create shader module!");
    }
  }

  void HallowPipeline::defaultPipelineConfig(PipelineConfigInfo& pipeline_config_info) {
    pipeline_config_info.input_assembly_info.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    pipeline_config_info.input_assembly_info.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    pipeline_config_info.input_assembly_info.primitiveRestartEnable = VK_FALSE;

    // viewport tells how we want to transform gl_Position values from shader into output image
    // scissor discards any pixels outside of the defined rectangle

    pipeline_config_info.viewport_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    pipeline_config_info.viewport_info.viewportCount = 1;
    pipeline_config_info.viewport_info.pViewports = nullptr;
    pipeline_config_info.viewport_info.scissorCount = 1;
    pipeline_config_info.viewport_info.pScissors = nullptr;

    // breaks up geometry into pixels
    pipeline_config_info.rasterization_info.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    pipeline_config_info.rasterization_info.depthClampEnable = VK_FALSE;
    pipeline_config_info.rasterization_info.rasterizerDiscardEnable = VK_FALSE;
    pipeline_config_info.rasterization_info.polygonMode = VK_POLYGON_MODE_FILL;
    pipeline_config_info.rasterization_info.lineWidth = 1.0f;
    pipeline_config_info.rasterization_info.cullMode = VK_CULL_MODE_BACK_BIT;
    pipeline_config_info.rasterization_info.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    pipeline_config_info.rasterization_info.depthBiasEnable = VK_FALSE;
    pipeline_config_info.rasterization_info.depthBiasConstantFactor = 0.0f;  // Optional
    pipeline_config_info.rasterization_info.depthBiasClamp = 0.0f;           // Optional
    pipeline_config_info.rasterization_info.depthBiasSlopeFactor = 0.0f;     // Optional

    // how rasterizer handles edges of geometry. without it, pixels are either in or out depending on center
    pipeline_config_info.multisample_info.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    pipeline_config_info.multisample_info.sampleShadingEnable = VK_FALSE;
    pipeline_config_info.multisample_info.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    pipeline_config_info.multisample_info.minSampleShading = 1.0f;           // Optional
    pipeline_config_info.multisample_info.pSampleMask = nullptr;             // Optional
    pipeline_config_info.multisample_info.alphaToCoverageEnable = VK_FALSE;  // Optional
    pipeline_config_info.multisample_info.alphaToOneEnable = VK_FALSE;       // Optional

    // how we combine colors in frame buffer
    pipeline_config_info.color_blend_attachment_state.colorWriteMask =
      VK_COLOR_COMPONENT_R_BIT |
      VK_COLOR_COMPONENT_G_BIT |
      VK_COLOR_COMPONENT_B_BIT |
      VK_COLOR_COMPONENT_A_BIT;
    pipeline_config_info.color_blend_attachment_state.blendEnable = VK_FALSE;
    pipeline_config_info.color_blend_attachment_state.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;   // Optional
    pipeline_config_info.color_blend_attachment_state.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;  // Optional
    pipeline_config_info.color_blend_attachment_state.colorBlendOp = VK_BLEND_OP_ADD;              // Optional
    pipeline_config_info.color_blend_attachment_state.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;   // Optional
    pipeline_config_info.color_blend_attachment_state.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;  // Optional
    pipeline_config_info.color_blend_attachment_state.alphaBlendOp = VK_BLEND_OP_ADD;              // Optional

    pipeline_config_info.color_blend_info.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    pipeline_config_info.color_blend_info.logicOpEnable = VK_FALSE;
    pipeline_config_info.color_blend_info.logicOp = VK_LOGIC_OP_COPY;  // Optional
    pipeline_config_info.color_blend_info.attachmentCount = 1;
    pipeline_config_info.color_blend_info.pAttachments = &pipeline_config_info.color_blend_attachment_state;
    pipeline_config_info.color_blend_info.blendConstants[0] = 0.0f;  // Optional
    pipeline_config_info.color_blend_info.blendConstants[1] = 0.0f;  // Optional
    pipeline_config_info.color_blend_info.blendConstants[2] = 0.0f;  // Optional
    pipeline_config_info.color_blend_info.blendConstants[3] = 0.0f;  // Optional

    // depth testing https://youtu.be/ecMcXW6MSYU?t=524
    pipeline_config_info.depth_stencil_info.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    pipeline_config_info.depth_stencil_info.depthTestEnable = VK_TRUE;
    pipeline_config_info.depth_stencil_info.depthWriteEnable = VK_TRUE;
    pipeline_config_info.depth_stencil_info.depthCompareOp = VK_COMPARE_OP_LESS;
    pipeline_config_info.depth_stencil_info.depthBoundsTestEnable = VK_FALSE;
    pipeline_config_info.depth_stencil_info.minDepthBounds = 0.0f;  // Optional
    pipeline_config_info.depth_stencil_info.maxDepthBounds = 1.0f;  // Optional
    pipeline_config_info.depth_stencil_info.stencilTestEnable = VK_FALSE;
    pipeline_config_info.depth_stencil_info.front = {};  // Optional
    pipeline_config_info.depth_stencil_info.back = {};   // Optional

    // configure pipeline to expect dynamic viewport and scissor to be provided later
    pipeline_config_info.dynamic_state_enables = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};
    pipeline_config_info.dynamic_state_info.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    pipeline_config_info.dynamic_state_info.pDynamicStates = pipeline_config_info.dynamic_state_enables.data();
    pipeline_config_info.dynamic_state_info.dynamicStateCount = static_cast<uint32_t >(pipeline_config_info
      .dynamic_state_enables.size());
    pipeline_config_info.dynamic_state_info.flags = 0;
  }

  void HallowPipeline::bind(VkCommandBuffer command_buffer) {
    vkCmdBindPipeline(command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_graphics_pipeline);
  }
}