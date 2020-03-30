SRC_FILES = src/main.cpp

BUILD_NAME = amara
BUILD_PATH = build

BUILD_EXECUTABLE_WIN = $(BUILD_PATH)/$(BUILD_NAME).exe
BUILD_EXECUTABLE_LINUX = $(BUILD_PATH)/$(BUILD_NAME).game

COMPILER = g++

SDL_INCLUDE_PATHS_64 = -I ext_lib\SDL2\win64\include\SDL2 -I ext_lib\SDL2_image\win64\include\SDL2 -I ext_lib\SDL2_ttf\win64\include\SDL2 -I ext_lib\SDL2_mixer\win64\include\SDL2 -I ext_lib/SDL_FontCache
SDL_LIBRARY_PATHS_64 = -L ext_lib\SDL2\win64\lib -L ext_lib\SDL2_image\win64\lib -L ext_lib\SDL2_ttf\win64\lib -L ext_lib\SDL2_mixer\win64\lib

SDL_INCLUDE_PATHS_32 = -I ext_lib\SDL2\win32\include\SDL2 -I ext_lib\SDL2_image\win32\include\SDL2 -I ext_lib\SDL2_ttf\win32\include\SDL2 -I ext_lib\SDL2_mixer\win32\include\SDL2 -I ext_lib/SDL_FontCache
SDL_LIBRARY_PATHS_32 = -L ext_lib\SDL2\win32\lib -L ext_lib\SDL2_image\win32\lib -L ext_lib\SDL2_ttf\win32\lib -L ext_lib\SDL2_mixer\win32\lib

LINKER_FLAGS = -l mingw32 -l SDL2main -l SDL2 -l SDL2_image -l SDL2_ttf -l SDL2_mixer

OTHER_LIB_PATHS = -I ext_lib\nlohmann\include -I ext_lib\tileson -I ext_lib/tileson/tileson/external_libs

AMARA_PATH = -I .\amara

COMPILER_FLAGS = -w -mwindows

win64: $(SRC_FILES)
	cls
	rm -rf build/*
	$(COMPILER) $(SRC_FILES) $(AMARA_PATH) $(OTHER_LIB_PATHS) $(SDL_INCLUDE_PATHS_64) $(SDL_LIBRARY_PATHS_64) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(BUILD_EXECUTABLE_WIN)
	cp dlls/win64/* $(BUILD_PATH)/
	mkdir -p $(BUILD_PATH)/assets
	cp -a assets/. $(BUILD_PATH)/assets/

win32: $(SRC_FILES)
	cls
	rm -rf build/*
	$(COMPILER) $(SRC_FILES) $(AMARA_PATH) $(OTHER_LIB_PATHS) $(SDL_INCLUDE_PATHS_32) $(SDL_LIBRARY_PATHS_32)  $(COMPILER_FLAGS) $(LINKER_FLAGS) -I ext_lib\SDL2-32\bin -o $(BUILD_EXECUTABLE_WIN)
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