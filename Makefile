#OBJS specifies which files to compile as part of the project
OBJS = "main.c" "math.c" "matrices.c"

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = main

#This is the target that compiles our executable
all : $(OBJS)
gcc $(OBJS) -IC:SDL2\include -LC:SDL2\lib -w -Wl, -lmingw32 -lSDL2main -lSDL2 -o $(OBJ_NAME)

gcc main.c math.c matrices.c SDL_setup.c -IC:SDL2\include -LC:SDL2\lib -w -Wl, -lmingw32 -lSDL2main -lSDL2 -o main