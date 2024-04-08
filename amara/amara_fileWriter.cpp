namespace Amara {
    class FileManager {
    public:
        FileManager() {}

		bool fileExists(std::string path) {
			std::ifstream in(path, std::ios::in | std::ios::binary);
			if (in) {
				in.close();
				return true;	
			}
			return false;
		}

        bool writeFile(std::string input, std::string path) {
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
			return false;
        }

		bool deleteFile(std::string path) {
			if (std::remove(path.c_str()) == 0) return true;
			else return false;
		}
    };
}