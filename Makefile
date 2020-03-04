OBJS = src/main.cpp

CC = g++

INCLUDE_PATHS_64 = -I lib\SDL2\win64\include\SDL2 -I lib\SDL2_image\win64\include\SDL2 -I lib\SDL2_ttf\win64\include\SDL2 -I lib\SDL2_mixer\win64\include\SDL2 -I lib/SDL_FontCache
LIBRARY_PATHS_64 = -L lib\SDL2\win64\lib -L lib\SDL2_image\win64\lib -L lib\SDL2_ttf\win64\lib -L lib\SDL2_mixer\win64\lib

INCLUDE_PATHS_32 = -I lib\SDL2\win32\include\SDL2 -I lib\SDL2_image\win32\include\SDL2 -I lib\SDL2_ttf\win32\include\SDL2 -I lib\SDL2_mixer\win32\include\SDL2 -I lib/SDL_FontCache
LIBRARY_PATHS_32 = -L lib\SDL2\win32\lib -L lib\SDL2_image\win32\lib -L lib\SDL2_ttf\win32\lib -L lib\SDL2_mixer\win32\lib

AMARA_PATH = -I lib\Amara -I lib\nlohmann\include

COMPILER_FLAGS = -w

LINKER_FLAGS = -l mingw32 -l SDL2main -l SDL2 -l SDL2_image -l SDL2_ttf -l SDL2_mixer

OBJ_NAME = amara
OBJ_PATH = bin/$(OBJ_NAME).exe

debug_amara:
	cls
	rm -rf debug/*
	$(CC) $(OBJS) $(AMARA_PATH) $(INCLUDE_PATHS_64) $(LIBRARY_PATHS_64) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o debug/amara_build.exe
	cp dlls/win64/* debug/

win64: $(OBJS)
	cls
	rm -rf bin/*
	$(CC) $(OBJS) $(AMARA_PATH) $(INCLUDE_PATHS_64) $(LIBRARY_PATHS_64) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_PATH)
	cp dlls/win64/* bin/
	mkdir -p bin/assets
	cp -a assets/. bin/assets/
	mkdir -p bin/user_data
	cp -a user_data/. bin/user_data/

win32: $(OBJS)
	cls
	rm -rf bin/*
	$(CC) $(OBJS) $(AMARA_PATH) $(INCLUDE_PATHS_32) $(LIBRARY_PATHS_32)  $(COMPILER_FLAGS) $(LINKER_FLAGS) -I lib\SDL2-32\bin -o $(OBJ_PATH)
	cp dlls/win32/* bin/
	mkdir -p bin/assets
	cp -a assets/. bin/assets/
	mkdir -p bin/user_data
	cp -a user_data/. bin/user_data/
	
	
clean:
	rm -rf bin/*
	
play:
	$(OBJ_PATH)

playlinux:
	./$(OBJ_PATH)

playdebug:
	debug/amara_build.exe