//
// Created by Gabriel Lugo on 3/30/2021.
//

#ifndef PROJECT_A_HALLOWPIPELINE_H
#define PROJECT_A_HALLOWPIPELINE_H

#include "engine/device/HallowEngineDevice.h"

#include <string>
#include <vector>

namespace Hallow {
    class HallowPipeline {
    public:
        HallowPipeline(const std::string& vertFilePath, const std::string& fragFilePath);

    private:
        static std::vector<char> readFile(const std::string& filePath);

        void createGraphicsPipeline(const std::string& vertFilePath, const std::string& fragFilePath);
    };
}


#endif //PROJECT_A_HALLOWPIPELINE_H
