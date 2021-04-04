//
// Created by Gabriel Lugo on 3/29/2021.
//

#ifndef PROJECT_A_TIME_H
#define PROJECT_A_TIME_H

#include "TimePoint.h"
#include "Stopwatch.h"

#include <GLFW/glfw3.h>
#include <chrono>


namespace Hallow {
    class Time {
        // duration TYPES
        using Nanoseconds = std::chrono::duration<double, std::nano>;
        using Microseconds = std::chrono::duration<double, std::micro>;
        using Milliseconds = std::chrono::duration<double, std::milli>;
        using Seconds = std::chrono::duration<double>;
        using Minutes = std::chrono::duration<double, std::ratio<60>>;
        using Hours = std::chrono::duration<double, std::ratio<3600>>;
        // chrono
        using ClockSystem = std::chrono::system_clock;
        using ClockSteady = std::chrono::steady_clock;
        using ClockAccurate = std::chrono::high_resolution_clock;
    public:
        void tick();

        TimePoint game() const { return m_game_current; }

        TimePoint delta() const { return m_delta; }

        TimePoint frame() const { return m_frame; }

        void printOnInterval(double seconds) {
            if (m_print_interval.seconds() >= seconds) {
                std::cout << *this;
                m_print_interval.reset();
            }
        }

        friend std::ostream&
        operator<<(std::ostream& output, const Time& time) {
            time.display(output);
            return output;
        }

    private:
        TimePoint m_game_last{};
        TimePoint m_game_current{glfwGetTime()};
        //time_t m_wall{};
        TimePoint m_delta{glfwGetTime()};
        TimePoint m_frame{};

        Stopwatch m_print_interval{};

        void display(std::ostream& output) const {
            output << "FPS: " << (int) (1.0 / m_frame.seconds()) << "\n"
                   << "Frametime: " << m_frame.seconds() << " s\n"
                   << "Deltatime: " << m_delta.seconds() << " s\n"
                   << "Lasttime: " << m_game_last.seconds() << " s\n"
                   << "Currtime: " << m_game_current.seconds() << " s\n";
        }
    };
}


#endif //PROJECT_A_TIME_H
