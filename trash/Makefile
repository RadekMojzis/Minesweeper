#OBJS specifies which files to compile as part of the project
OBJS = sweeper.o sdl_must.o timer.o

#CC specifies which compiler we're using
CC = g++  

#INCLUDE_PATHS specifies the additional include paths we'll need
INCLUDE_PATHS = -IC:\mingw_dev_lib\include\SDL2

#LIBRARY_PATHS specifies the additional library paths we'll need
LIBRARY_PATHS = -LC:\mingw_dev_lib\lib

#COMPILER_FLAGS specifies the additional compilation options we're using

COMPILER_FLAGS = -Wl,-subsystem,windows

#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -lmingw32 -lSDL2main -lSDL2

FLAGS = $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS)


#This is the target that compiles our executable
all : $(OBJS)
	$(CC) $(FLAGS) $(OBJS) -o Minesweeper
  
  
sweeper.o: sweeper.cpp sdl_must.h timer.h
	$(CC) $(FLAGS) sweeper.cpp sdl_must.h timer.h

sdl_must.o: sdl_must.cpp sdl_must.h
	$(CC) $(FLAGS) sdl_must.cpp sdl_must.h

timer.o: timer.cpp timer.h
	$(CC) $(FLAGS) timer.cpp timer.h