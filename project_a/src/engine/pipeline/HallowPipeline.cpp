//
// Created by Gabriel Lugo on 3/30/2021.
//

#include "HallowPipeline.h"

#include <fstream>
#include <iostream>
#include <stdexcept>

namespace Hallow {
    HallowPipeline::HallowPipeline(const std::string& vertFilePath, const std::string& fragFilePath) {
        createGraphicsPipeline(vertFilePath, fragFilePath);
    }

    std::vector<char> HallowPipeline::readFile(const std::string& filePath) {
        std::cout << "Opening " + filePath + "...";
        std::ifstream file{filePath, std::ios::ate | std::ios::binary}; // ate says to skip to end of file
        std::cout << " ... ";
        if (!file.is_open()) {
            std::cerr << " File not opened!\n";
            throw std::runtime_error("HallowPipeline: Failed to open file: " + filePath);
        }
        std::cout << " ... ";

        std::cout << "\nGetting size of " + filePath + "...\n";
        size_t fileSize = static_cast<size_t>(file.tellg()); // tellg reports position
        std::vector<char> buffer(fileSize);

        std::cout << "Parsing " + filePath + "...\n";
        file.seekg(0); // go back to beginning
        file.read(buffer.data(), fileSize); // read for as many bits as file size

        std::cout << "Closing " + filePath + "...\n";
        file.close();
        return buffer;
    }

    void HallowPipeline::createGraphicsPipeline(const std::string& vertFilePath, const std::string& fragFilePath) {
        std::cout << "Creating graphics pipeline...\n";
        std::cout << "Reading file: " + vertFilePath + "...\n";
        auto vertCode = readFile(vertFilePath);
        std::cout << vertFilePath + " read!\n";
        std::cout << "Reading file: " + fragFilePath + "\n";
        auto fragCode = readFile(fragFilePath);
        std::cout << fragFilePath + " read!\n";

        std::cout << "Vert Shader Code Buffer Size: " << vertCode.size() << "\n"
                  << "Frag Shader Code Buffer Size: " << fragCode.size() << "\n";
        std::cout << "Graphics pipeline created!\n";
    }
}