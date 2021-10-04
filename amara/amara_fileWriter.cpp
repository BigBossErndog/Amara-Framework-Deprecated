#pragma once
#include "amara.h"

namespace Amara {
    class FileWriter {
    public:
        FileWriter() {}

        bool write(std::string input, std::string path) {
            std::ofstream file(path);
			if (file.fail()) {
				SDL_Log("Failed to write to path: %s", path.c_str());
				return false;
			}
			else {
				SDL_Log("Written file: %s", path.c_str());
				file << input;
				file.close();
				return true;
			}
        }
    };
}