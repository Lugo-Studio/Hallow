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
    std::vector<VkDynamicState> dynamic_state_enables;
    VkPipelineDynamicStateCreateInfo dynamic_state_info;

    PipelineConfigInfo() = default;
    ~PipelineConfigInfo() = default;

    PipelineConfigInfo(const PipelineConfigInfo&) = delete;
    PipelineConfigInfo& operator=(const PipelineConfigInfo&) = delete;
  };

  class HallowPipeline {
  public:
    HallowPipeline(
      HallowDevice& device, const PipelineConfigInfo& pipeline_config_info, const std::string& shader_file_path)
      : m_device{device} {
      init(device, pipeline_config_info, shader_file_path + ".vert.spv", shader_file_path + ".frag.spv");
    }

    HallowPipeline(
      HallowDevice& device,
      const PipelineConfigInfo& pipeline_config_info,
      const std::string& vert_file_path,
      const std::string& frag_file_path) : m_device{device} {
      init(device, pipeline_config_info, vert_file_path, frag_file_path);
    }

    ~HallowPipeline();
    HallowPipeline(const HallowPipeline&) = delete;
    HallowPipeline& operator=(const HallowPipeline&) = delete;

    static void defaultPipelineConfig(PipelineConfigInfo& pipeline_config_info);
    void bind(VkCommandBuffer command_buffer);
  private:
    HallowDevice& m_device;
    VkPipeline m_graphics_pipeline;
    // TODO: migrate these to collections of shaders?
    VkShaderModule m_vert_shader_module;
    VkShaderModule m_frag_shader_module;

    void init(
      HallowDevice& device,
      const PipelineConfigInfo& pipeline_config_data,
      const std::string& vert_file_path,
      const std::string& frag_file_path);

    void init2(
      HallowDevice& device,
      const PipelineConfigInfo& pipeline_config_data,
      const std::string& vert_file_path,
      const std::string& frag_file_path);

    static std::vector<char> readFile(const std::string& file_path);

    void createGraphicsPipeline(
      const PipelineConfigInfo& pipeline_config_info,
      const std::string& vert_file_path,
      const std::string& frag_file_path);

    void createShaderModule(
      const std::vector<char>& shader_code, VkShaderModule* shader_module);
    void createShaderModule2(
      const std::vector<char>& shader_code, VkShaderModule* shader_module);
  };
}


#endif //PROJECT_A_HALLOWPIPELINE_H
