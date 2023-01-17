#OBJS specifies which files to compile as part of the project
OBJS = src/*.cpp src/lib/glad.c

#CC specifies which compiler we're using
CC = g++

#COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
COMPILER_FLAGS = -fsanitize=undefined

#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -ldl -lglfw -lassimp -lstdc++fs

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = out.exe

#This is the target that compiles our executable
default : $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)

run:
	./$(OBJ_NAME)
all:
	make
	./$(OBJ_NAME)