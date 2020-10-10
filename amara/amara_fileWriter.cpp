#pragma once

#include "amara.h"

namespace Amara {
    class FileWriter {
    public:
        FileWriter() {}

        bool write(std::string input, std::string path) {
            std::ofstream file(path);
            file << input;
            file.close();
            return true;
        }
    };
}