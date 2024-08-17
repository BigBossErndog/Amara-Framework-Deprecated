SRC_FILES = src/main.cpp

BUILD_NAME = amara
BUILD_PATH = build

BUILD_EXECUTABLE_WIN = $(BUILD_PATH)/$(BUILD_NAME).exe
BUILD_EXECUTABLE_LINUX = $(BUILD_PATH)/$(BUILD_NAME).game
BUILD_HTML = $(BUILD_PATH)/index.html

COMPILER = g++
WSL_COMPILER = x86_64-w64-mingw32-g++

SDL_INCLUDE_PATHS_WIN64 = -I ext_lib/SDL2/win64/include/SDL2 -I ext_lib/SDL2_image/win64/include/SDL2 -I ext_lib/SDL2_ttf/win64/include/SDL2 -I ext_lib/SDL2_mixer/win64/include/SDL2 -I ext_lib/SDL2_net/win64/include/SDL2 -I ext_lib/SDL_FontCache
SDL_LIBRARY_PATHS_WIN64 = -L ext_lib/SDL2/win64/lib -L ext_lib/SDL2_image/win64/lib -L ext_lib/SDL2_ttf/win64/lib -L ext_lib/SDL2_mixer/win64/lib -L ext_lib/SDL2_net/win64/lib

SDL_INCLUDE_PATHS_WIN32 = -I ext_lib/SDL2/win32/include/SDL2 -I ext_lib/SDL2_image/win32/include/SDL2 -I ext_lib/SDL2_ttf/win32/include/SDL2 -I ext_lib/SDL2_mixer/win32/include/SDL2 -I ext_lib/SDL2_net/win32/include/SDL2 -I ext_lib/SDL_FontCache
SDL_LIBRARY_PATHS_WIN32 = -L ext_lib/SDL2/win32/lib -L ext_lib/SDL2_image/win32/lib -L ext_lib/SDL2_ttf/win32/lib -L ext_lib/SDL2_mixer/win32/lib -L ext_lib/SDl2_net/win32/lib

SDL_INCLUDE_PATHS_LINUX = `sdl2-config --cflags` -I ext_lib/SDL_FontCache

LINKER_FLAGS = -l SDL2main -l SDL2 -l SDL2_image -l SDL2_ttf -l SDL2_mixer -l SDL2_net

THEORA_INCLUDE_PATHS = -I ext_lib/ogg/include -I ext_lib/vorbis/include -I ext_lib/theora/include -I ext_lib/sdlogv
THEORA_LIBRARY_PATHS = -L ext_lib/ogg/lib -L ext_lib/vorbis/lib
THEORA_LINKER_FLAGS =  -l vorbisenc -l vorbisfile -l vorbis -l ogg

THEORA = $(THEORA_INCLUDE_PATHS) $(THEORA_LIBRARY_PATHS) $(THEORA_LINKER_FLAGS)

OTHER_LIB_PATHS = -I ext_lib/nlohmann/include -I ./src -I ext_lib/murmurhash3 $(THEORA)

AMARA_PATH = -I ./amara

COMPILER_FLAGS = -w -Wall
# COMPILER_FLAGS = -w -Wl,-subsystem,windows

WEB_FLAGS = -O2 -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS='["png"]' -s USE_SDL_IMAGE=2 -s USE_SDL_MIXER=2 -s USE_SDL_TTF=2 --preload-file $(BUILD_PATH)/assets -I ext_lib/SDL_FontCache -I ./amaraWeb -w

all:
	@echo "Usage: make (option)"
	@echo "Options: win64 win32 archlinux"
	@echo "Options: playwin playlinux"

clean:
	rm -rf $(BUILD_PATH)/*

wsl:
	rm -rf build/*.dll
	rm -rf build/assets/*
	cp -R assets/ build/
	$(WSL_COMPILER) $(SRC_FILES) $(AMARA_PATH) $(OTHER_LIB_PATHS) $(SDL_INCLUDE_PATHS_WIN64) $(SDL_LIBRARY_PATHS_WIN64) $(COMPILER_FLAGS) -l mingw32 -static-libgcc -static-libstdc++ -Wl,-Bstatic -lstdc++ -lpthread -Wl,-Bdynamic $(LINKER_FLAGS) -o $(BUILD_EXECUTABLE_WIN)
	cp dlls/win64/* $(BUILD_PATH)/

win64: $(SRC_FILES)
	rm -rf build/*.dll
	rm -rf build/assets/*
	cp -R assets/ build/
	$(COMPILER) $(SRC_FILES) $(AMARA_PATH) $(OTHER_LIB_PATHS) $(SDL_INCLUDE_PATHS_WIN64) $(SDL_LIBRARY_PATHS_WIN64) $(COMPILER_FLAGS) -l mingw32 -static-libgcc -static-libstdc++ -Wl,-Bstatic -lstdc++ -lpthread -Wl,-Bdynamic $(LINKER_FLAGS) -o $(BUILD_EXECUTABLE_WIN)
	cp dlls/win64/* $(BUILD_PATH)/


win32: $(SRC_FILES)
	rm -rf build/*.dll
	rm -rf build/assets/*
	cp -R assets/ build/
	$(COMPILER) $(SRC_FILES) $(AMARA_PATH) $(OTHER_LIB_PATHS) $(SDL_INCLUDE_PATHS_WIN32) $(SDL_LIBRARY_PATHS_WIN32) $(COMPILER_FLAGS) -l mingw32 -static-libgcc -static-libstdc++ -Wl,-Bstatic -lstdc++ -lpthread -Wl,-Bdynamic $(LINKER_FLAGS) -I ext_lib/SDL2-32/bin -o $(BUILD_EXECUTABLE_WIN)
	cp dlls/win32/* $(BUILD_PATH)/

linux:
	clear
	rm -rf build/*.dll
	rm -rf build/assets/*
	cp -R assets/ build/
	$(COMPILER) $(SRC_FILES) $(AMARA_PATH) $(OTHER_LIB_PATHS) $(SDL_INCLUDE_PATHS_LINUX) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(BUILD_EXECUTABLE_LINUX)

web:
	cls
	rm -rf build/*.dll
	rm -rf build/assets/*
	cp -R assets/ build/
	emcc $(SRC_FILES) $(AMARA_PATH) $(OTHER_LIB_PATHS) $(WEB_FLAGS) -o $(BUILD_HTML)

playwin:
	rm -rf build/assets/*
	cp -R assets/ build/
	$(BUILD_EXECUTABLE_WIN)

playlinux:
	rm -rf build/assets/*
	cp -R assets/ build/
	./$(BUILD_EXECUTABLE_LINUX)

valgrind:
	rm -rf build/assets/*
	cp -R assets/ build/
	valgrind --leak-check=yes ./$(BUILD_EXECUTABLE_LINUX)

setup-wsl:
	make setup-apt64
	sudo apt-get install mingw-w64

setup-apt64:
	sudo apt-get install libsdl2-2.0-0
	sudo apt-get install libsdl2-dev
	sudo apt-get install libsdl2-image-dev
	sudo apt-get install libsdl2-ttf-dev
	sudo apt-get install libsdl2-mixer-dev
	sudo apt-get install libsdl2-net-dev

setup-yum64:
	sudo yum install SDL2
	sudo yum install SDL2_image
	sudo yum install SDL2_ttf
	sudo yum install SDL2_mixer
	sudo yum install SDL2_net
	sudo yum install SDL2_image-devel
	sudo yum install SDL2_ttf-devel
	sudo yum install SDL2_mixer-devel
	sudo yum install SDL2_net-devel

setup-pacman64:
	sudo pacman -S sdl2
	sudo pacman -S sdl2_image
	sudo pacman -S sdl2_ttf
	sudo pacman -S sdl2_mixer
	sudo pacman -S sdl2_net
