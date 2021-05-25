//
// Created by galex on 5/20/2021.
//

#ifndef HALLOW_HALLOWSHADER_HPP
#define HALLOW_HALLOWSHADER_HPP

#include <string>
#include <array>

namespace Hallow {
  class HallowShader {
  public:
    HallowShader(const std::string& file_name);
  private:
    std::string m_shader_file_name{};
    std::array<std::string, 2> m_shader_code{};
  };
}


#endif //HALLOW_HALLOWSHADER_HPP
