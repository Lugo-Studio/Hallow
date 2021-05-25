//
// Created by galex on 5/20/2021.
//

#include "HallowShader.hpp"

/*#include "glslang/Include/ShHandle.h"
#include "glslang/Public/ShaderLang.h"
#include "glslang/SPIRV/GlslangToSpv.h"*/


namespace Hallow {
  HallowShader::HallowShader(const std::string& file_name)
    : m_shader_file_name{file_name} {

    /*auto essl_version = glslang::GetEsslVersionString();
    auto glsl_version = glslang::GetGlslVersionString();
    bool initialized = glslang::InitializeProcess();
    glslang::FinalizeProcess();*/
  }
}