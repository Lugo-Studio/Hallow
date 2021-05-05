//
// Created by Gabriel Lugo on 3/30/2021.
//

#ifndef PROJECT_A_HALLOWPIPELINE_H
#define PROJECT_A_HALLOWPIPELINE_H

#include "engine/device/HallowDevice.hpp"
#include "helpers/RootDir.h"

#include <string>
#include <vector>


namespace Hallow {
  struct PipelineConfigInfo {
    VkViewport viewport{};
    VkRect2D scissor{};
    VkPipelineInputAssemblyStateCreateInfo input_assembly_info{};
    VkPipelineViewportStateCreateInfo viewport_info{};
    VkPipelineRasterizationStateCreateInfo rasterization_info{};
    VkPipelineMultisampleStateCreateInfo multisample_info{};
    VkPipelineColorBlendAttachmentState color_blend_attachment_state{};
    VkPipelineColorBlendStateCreateInfo color_blend_info{};
    VkPipelineDepthStencilStateCreateInfo depth_stencil_info{};
    VkPipelineLayout pipeline_layout = (VkPipelineLayout) nullptr;
    VkRenderPass render_pass = (VkRenderPass) nullptr;
    uint32_t subpass = 0;

    PipelineConfigInfo() = default;
    ~PipelineConfigInfo() = default;

    PipelineConfigInfo(const PipelineConfigInfo&) = delete;
    PipelineConfigInfo& operator=(const PipelineConfigInfo&) = delete;
  };

  class HallowPipeline {
  public:
    HallowPipeline(
        HallowDevice& device, const PipelineConfigInfo& pipelineConfigInfo, const std::string& shaderFilePath)
        : m_device{device} {
      init(device, pipelineConfigInfo, shaderFilePath + ".vert.spv", shaderFilePath + ".frag.spv");
    }

    HallowPipeline(
        HallowDevice& device,
        const PipelineConfigInfo& pipelineConfigInfo,
        const std::string& vertFilePath,
        const std::string& fragFilePath) : m_device{device} {
      init(device, pipelineConfigInfo, vertFilePath, fragFilePath);
    }

    ~HallowPipeline();

    static void defaultPipelineConfig(
        PipelineConfigInfo& pipelineConfigInfo, uint32_t width, uint32_t height);
    void bind(VkCommandBuffer commandBuffer);

    HallowPipeline(const HallowPipeline&) = delete;
    HallowPipeline& operator=(const HallowPipeline&) = delete;
  private:
    HallowDevice& m_device;
    VkPipeline m_graphics_pipeline;
    // TODO: migrate these to collections of shaders?
    VkShaderModule m_vert_shader_module;
    VkShaderModule m_frag_shader_module;

    void init(
        HallowDevice& device,
        const PipelineConfigInfo& pipelineConfigData,
        const std::string& vertFilePath,
        const std::string& fragFilePath);

    static std::vector<char> readFile(const std::string& filePath);

    void createGraphicsPipeline(
        const PipelineConfigInfo& pipelineConfigInfo, const std::string& vertFilePath, const std::string& fragFilePath);

    void createShaderModule(
        const std::vector<char>& shaderCode, VkShaderModule* shaderModule);
  };
}


#endif //PROJECT_A_HALLOWPIPELINE_H
