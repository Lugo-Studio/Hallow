echo off
set version=%1
set shaderFilePath=%2
if "%version%" == 32 goto :32bit
C:\VulkanSDK\1.2.170.0\Bin\glslc.exe %shaderFilePath% -o %shaderFilePath%.spv
:: C:\VulkanSDK\1.2.170.0\Bin\glslc.exe %shaderFilePath%.vert -o %shaderFilePath%.vert.spv
:: C:\VulkanSDK\1.2.170.0\Bin\glslc.exe %shaderFilePath%.frag -o %shaderFilePath%.frag.spv
goto :eof
:32bit
C:\VulkanSDK\1.2.170.0\Bin32\glslc.exe %shaderFilePath% -o %shaderFilePath%.spv
:: C:\VulkanSDK\1.2.170.0\Bin32\glslc.exe %shaderFilePath%.vert -o %shaderFilePath%.vert.spv
:: C:\VulkanSDK\1.2.170.0\Bin32\glslc.exe %shaderFilePath%.frag -o %shaderFilePath%.frag.spv