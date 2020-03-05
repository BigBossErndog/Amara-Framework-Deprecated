SRC_FILESS = src/main.cpp

BUILD_NAME = amara
BUILD_PATH = build

BUILD_EXECUTABLE_WIN = $(BUILD_PATH)/$(BUILD_NAME).exe
BUILD_EXECUTABLE_LINUX = $(BUILD_PATH)/$(BUILD_NAME).game

COMPILER = g++

INCLUDE_PATHS_64 = -I lib\SDL2\win64\include\SDL2 -I lib\SDL2_image\win64\include\SDL2 -I lib\SDL2_ttf\win64\include\SDL2 -I lib\SDL2_mixer\win64\include\SDL2 -I lib/SDL_FontCache
LIBRARY_PATHS_64 = -L lib\SDL2\win64\lib -L lib\SDL2_image\win64\lib -L lib\SDL2_ttf\win64\lib -L lib\SDL2_mixer\win64\lib

INCLUDE_PATHS_32 = -I lib\SDL2\win32\include\SDL2 -I lib\SDL2_image\win32\include\SDL2 -I lib\SDL2_ttf\win32\include\SDL2 -I lib\SDL2_mixer\win32\include\SDL2 -I lib/SDL_FontCache
LIBRARY_PATHS_32 = -L lib\SDL2\win32\lib -L lib\SDL2_image\win32\lib -L lib\SDL2_ttf\win32\lib -L lib\SDL2_mixer\win32\lib

AMARA_PATH = -I lib\Amara -I lib\nlohmann\include

COMPILER_FLAGS = -w

LINKER_FLAGS = -l mingw32 -l SDL2main -l SDL2 -l SDL2_image -l SDL2_ttf -l SDL2_mixer

win64: $(SRC_FILESS)
	cls
	rm -rf build/*
	$(COMPILER) $(SRC_FILESS) $(AMARA_PATH) $(INCLUDE_PATHS_64) $(LIBRARY_PATHS_64) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(BUILD_EXECUTABLE_WIN)
	cp dlls/win64/* $(BUILD_PATH)/
	mkdir -p $(BUILD_PATH)/assets
	cp -a assets/. $(BUILD_PATH)/assets/

win32: $(SRC_FILESS)
	cls
	rm -rf build/*
	$(COMPILER) $(SRC_FILESS) $(AMARA_PATH) $(INCLUDE_PATHS_32) $(LIBRARY_PATHS_32)  $(COMPILER_FLAGS) $(LINKER_FLAGS) -I lib\SDL2-32\bin -o $(BUILD_EXECUTABLE_WIN)
	cp dlls/win32/* $(BUILD_PATH)/
	mkdir -p $(BUILD_PATH)/assets
	cp -a assets/. $(BUILD_PATH)/assets/
	
	
clean:
	rm -rf $(BUILD_PATH)/*

play:
	$(BUILD_EXECUTABLE_WIN)

playwin:
	$(BUILD_EXECUTABLE_WIN)

playlinux:
	./$(BUILD_EXECUTABLE_LINUX)