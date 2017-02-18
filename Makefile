all : sweeper clean

#CC specifies which compiler we're using
CC = g++ -std=c++14

#INCLUDE_PATHS specifies the additional include paths we'll need
INCLUDE_PATHS = -IC:\mingw_dev_lib\include\SDL2

#LIBRARY_PATHS specifies the additional library paths we'll need
LIBRARY_PATHS = -LC:\mingw_dev_lib\lib

#COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
# -Wl,-subsystem,windows gets rid of the console window
COMPILER_FLAGS = -w -Wl,-subsystem,windows

SDL_LIBS = sdl-config --libs

#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -lmingw32  -lSDL2main -lSDL2 -lopengl32 -lglu32 -lSDL2_image

#OBJ_NAME specifies the name of our exectuable

#This is the target that compiles our executable

button.o: button.cpp
	$(CC) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $^ -c

sweeper.o: sweeper.cpp
	$(CC) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $^ -c
  
game.o: game.cpp
	$(CC) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $^ -c

records.o: records.cpp
	$(CC) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $^ -c
  
menu.o: menu.cpp
	$(CC) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $^ -c
  
options.o: options.cpp
	$(CC) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $^ -c

sweeper: sweeper.o button.o menu.o records.o game.o options.o
	$(CC) sweeper.o button.o menu.o records.o game.o options.o $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $@
  
clean:
	del *.o