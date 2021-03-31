//
// Created by Gabriel Lugo on 3/28/2021.
//

#ifndef PROJECT_A_HALLOWAPP_H
#define PROJECT_A_HALLOWAPP_H

#include "engine/time/Time.h"
#include "engine/window/HallowWindow.h"
#include "engine/pipeline/HallowPipeline.h"
#include "helpers/RootDir.h"

#include <string>

namespace Hallow {
    class HallowApp {
    public:
        static constexpr int WIDTH = 1024;
        static constexpr int HEIGHT = 576;

        void run();
    private:
        std::string m_name = "Hallow Engine";

        Time m_time{};
        HallowWindow m_hallow_window{WIDTH, HEIGHT, m_name};
        HallowPipeline m_hallow_pipeline{
            ROOT_DIR"res/shaders/simple_shader.vert.spv",
            ROOT_DIR"res/shaders/simple_shader.frag.spv"
        };

        void startPre();
        void start();
        void startPost();

        void updatePre();
        void update();
        void updatePost();

        void endPre();
        void end();
        void endPost();
    };
}


#endif //PROJECT_A_HALLOWAPP_H
