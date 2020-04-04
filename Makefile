SRC_FILES = src/main.cpp

BUILD_NAME = amara
BUILD_PATH = build

BUILD_EXECUTABLE_WIN = $(BUILD_PATH)/$(BUILD_NAME).exe
BUILD_EXECUTABLE_LINUX = $(BUILD_PATH)/$(BUILD_NAME).game

COMPILER = g++

SDL_INCLUDE_PATHS_WIN64 = -I ext_lib/SDL2/win64/include/SDL2 -I ext_lib/SDL2_image/win64/include/SDL2 -I ext_lib/SDL2_ttf/win64/include/SDL2 -I ext_lib/SDL2_mixer/win64/include/SDL2 -I ext_lib/SDL_FontCache
SDL_LIBRARY_PATHS_WIN64 = -L ext_lib/SDL2/win64/lib -L ext_lib/SDL2_image/win64/lib -L ext_lib/SDL2_ttf/win64/lib -L ext_lib/SDL2_mixer/win64/lib

SDL_INCLUDE_PATHS_WIN32 = -I ext_lib/SDL2/win32/include/SDL2 -I ext_lib/SDL2_image/win32/include/SDL2 -I ext_lib/SDL2_ttf/win32/include/SDL2 -I ext_lib/SDL2_mixer/win32/include/SDL2 -I ext_lib/SDL_FontCache
SDL_LIBRARY_PATHS_WIN32 = -L ext_lib/SDL2/win32/lib -L ext_lib/SDL2_image/win32/lib -L ext_lib/SDL2_ttf/win32/lib -L ext_lib/SDL2_mixer/win32/lib

SDL_INCLUDE_PATHS_ARCHLINUX64 = -I ext_lib/SDL2/archlinux64 -I ext_lib/SDL_FontCache

LINKER_FLAGS = -l SDL2main -l SDL2 -l SDL2_image -l SDL2_ttf -l SDL2_mixer

OTHER_LIB_PATHS = -I ext_lib/nlohmann/include

AMARA_PATH = -I ./amara

COMPILER_FLAGS = -w

all:
	@echo "Usage: make (option)"
	@echo "Options: win64 win32 archlinux64"
	@echo "Options: playwin playlinux"

clean:
	rm -rf $(BUILD_PATH)/*

win64: $(SRC_FILES)
	cls
	rm -rf build/*
	$(COMPILER) $(SRC_FILES) $(AMARA_PATH) $(OTHER_LIB_PATHS) $(SDL_INCLUDE_PATHS_WIN64) $(SDL_LIBRARY_PATHS_WIN64) $(COMPILER_FLAGS) -l mingw32 $(LINKER_FLAGS) -o $(BUILD_EXECUTABLE_WIN)
	cp dlls/win64/* $(BUILD_PATH)/
	mkdir -p $(BUILD_PATH)/assets
	cp -a assets/. $(BUILD_PATH)/assets/

win32: $(SRC_FILES)
	cls
	rm -rf build/*
	$(COMPILER) $(SRC_FILES) $(AMARA_PATH) $(OTHER_LIB_PATHS) $(SDL_INCLUDE_PATHS_WIN32) $(SDL_LIBRARY_PATHS_WIN32) $(COMPILER_FLAGS) -l mingw32 $(LINKER_FLAGS) -I ext_lib/SDL2-32/bin -o $(BUILD_EXECUTABLE_WIN)
	cp dlls/win32/* $(BUILD_PATH)/
	mkdir -p $(BUILD_PATH)/assets
	cp -a assets/. $(BUILD_PATH)/assets/

archlinux64:
	clear
	rm -rf build/*
	$(COMPILER) $(SRC_FILES) $(AMARA_PATH) $(OTHER_LIB_PATHS) $(SDL_INCLUDE_PATHS_ARCHLINUX64) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(BUILD_EXECUTABLE_LINUX)
	mkdir -p $(BUILD_PATH)/assets
	cp -a assets/. $(BUILD_PATH)/assets/

playwin:
	$(BUILD_EXECUTABLE_WIN)

playlinux:
	./$(BUILD_EXECUTABLE_LINUX)
