//
// Created by Gabriel Lugo on 3/28/2021.
//

#include "HallowApp.h"

#include <iostream>
#include <stdexcept>

namespace Hallow {

    HallowApp::HallowApp() {
        createPipelineLayout();
        createPipeline();
        createCommandBuffers();
    }

    HallowApp::~HallowApp() {
        vkDestroyPipelineLayout(m_hallow_device.device(), m_pipeline_layout, nullptr);

    }

    void HallowApp::run() {
        startPre();
        start();
        startPost();

        while (!m_hallow_window.shouldClose()) {
            m_time.tick(); // start with tick to get newest time
            m_time.printOnInterval(1);
            glfwPollEvents(); // poll for user events and such

            updatePre();
            update();
            updatePost();
        }

        endPre();
        end();
        endPost();
    }

    void HallowApp::createPipelineLayout() {
        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 0;
        pipelineLayoutInfo.pSetLayouts = nullptr;
        pipelineLayoutInfo.pushConstantRangeCount = 0;
        pipelineLayoutInfo.pPushConstantRanges = nullptr;
        if (vkCreatePipelineLayout(m_hallow_device.device(), &pipelineLayoutInfo, nullptr, &m_pipeline_layout) != VK_SUCCESS) {
            throw std::runtime_error("HallowApp: Failed to create pipeline layout!");
        }
    }

    void HallowApp::createPipeline() {
        PipelineConfigInfo pipelineConfig{};

        HallowPipeline::defaultPipelineConfig(
                pipelineConfig,
                m_hallow_swap_chain.width(),
                m_hallow_swap_chain.height());

        // render pass describes the structure and format of frame buffer objects and their attachments
        pipelineConfig.renderPass = m_hallow_swap_chain.getRenderPass();
        pipelineConfig.pipelineLayout = m_pipeline_layout;

        m_hallow_pipeline = std::make_unique<HallowPipeline>(m_hallow_device, pipelineConfig, "res/shaders/simple_shader");
    }

    void HallowApp::createCommandBuffers() {

    }

    void HallowApp::drawFrame() {

    }

    // lifetime class?
    void HallowApp::startPre() { }

    void HallowApp::start() { }

    void HallowApp::startPost() { }

    void HallowApp::updatePre() { }

    void HallowApp::update() { }

    void HallowApp::updatePost() { }

    void HallowApp::endPre() { }

    void HallowApp::end() { }

    void HallowApp::endPost() { }
}