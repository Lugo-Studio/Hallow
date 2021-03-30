//
// Created by Gabriel Lugo on 3/28/2021.
//

#ifndef PROJECT_A_HALLOWAPP_H
#define PROJECT_A_HALLOWAPP_H


#include <engine/window/HallowWindow.h>
#include <engine/time/Time.h>

#include <string>

namespace Hallow {
    class HallowApp {
    public:
        static constexpr int WIDTH = 1024;
        static constexpr int HEIGHT = 576;

        HallowApp() : m_hallow_window{WIDTH, HEIGHT, m_name}, m_time() {}

        void run();
    private:
        std::string m_name = "Hallow App";
        const int m_FIXED_TIME_STEP = 100;

        HallowWindow m_hallow_window;
        Time m_time;

        void start();
        void update();
        void fixedUpdate();
    };
}


#endif //PROJECT_A_HALLOWAPP_H
