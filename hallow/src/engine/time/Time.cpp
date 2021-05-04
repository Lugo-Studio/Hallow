//
// Created by Gabriel Lugo on 3/29/2021.
//

#include "Time.hpp"

#include <ctime>


namespace Hallow {
  void Time::tick() {
    m_game_current = glfwGetTime();
    m_delta = m_game_current.seconds() - m_game_last.seconds();
    m_frame = m_delta; // TODO: calculate this via time between renders
    m_game_last = m_game_current;
  }
}