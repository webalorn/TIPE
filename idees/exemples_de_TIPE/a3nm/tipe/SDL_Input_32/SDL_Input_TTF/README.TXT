This is the readme file for the SDL_Input_TTF library. Currently, the only
	tested compiler is gcc (GNU Compiler Collection) which can be used on every
	platform (needs MinGW under Windows though).

NOTE: Should any mistake, error or bug be found in any of SDL_Input_TTF files,
	email me at vhann3000 at gmail dot com

Index:
1. Compiling (GCC)
2. Update Log

======
1. GCC
======
*** Compiling as a static library ***
Note: This step is unnecessary if you use the libSDL_Input_TTF.a file provided in
	the archive file (zip or tar depending on your system)

1- Create a subfolder named (for example) SDL_Input_TTF in the source files
	directory and extract the SDL_Input_TTF source files there.
2- Enter that subfolder and compile SDL_Input_TTF source files as object files like
	this:
gcc -Wall -Wextra -ansi -pedantic -c *.c
3- Now, use "ar" to produce the static library:
ar rcs libSDL_Input_TTF.a *.o
4- You're done now.

*** Compiling your programs with the static library ***

1- Place yourself in your program's sources folder
2- Compile all source files like this:
gcc -Wall -Wextra -ansi -pedantic -lSDL -lSDL_ttf *.c -L[libSDL_Input.a folder]
	-lSDL_Input -L[libSDL_Input_TTF.a folder] -lSDL_Input_TTF
Note: replace "[libSDL_Input_TTF.a folder]" by the relative path to where you put
	the libSDL_Input_TTF.a file. Theorically, this should be "SDL_Input_TTF".
3- Shall everything work fine, the compilation should have produced a file named
	a.out (under Linux) which is your executable.
Note: you may add a "-o [executableName]" to the above compilation line to
	choose the name of the generated executable (so it won't be named a.out).

=============
2. Update log:
=============

