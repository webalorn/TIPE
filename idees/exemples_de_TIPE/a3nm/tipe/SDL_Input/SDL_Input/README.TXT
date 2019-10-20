This is the readme file. Currently, the only tested compiler is gcc (GNU
	Compiler Collection) which can be used on every platform (needs MinGW under
	Windows though).

NOTE: Should any mistake, error or bug be found in any of SDL_Input or
	SDL_Input_TTF files, email me at vhann3000 at gmail dot com

Index:
1. Compiling (GCC)
2. Update Log

======
1. GCC
======
*** Compiling as a static library ***
Note: This step is unnecessary if you use the libSDL_Input.a file provided in
	the archive file (zip or tar depending on your system)

1- Create a subfolder named (for example) SDL_Input in the source files
	directory and extract the SDL_Input source files there.
2- Enter that subfolder and compile SDL_Input source files as object files like
	this:
gcc -Wall -Wextra -ansi -pedantic -c *.c
3- Now, use "ar" to produce the static library:
ar rcs libSDL_Input.a *.o
4- You're done now.

*** Compiling your programs with the static library ***

1- Place yourself in your program's sources folder
2- Compile all source files like this:
gcc -Wall -Wextra -ansi -pedantic -lSDL -lSDL_ttf *.c -L[libSDL_Input.a folder]
	-lSDL_Input
Note: replace "[libSDL_Input.a folder]" by the relative path to where you put
	the libSDL_Input.a file. Theorically, this should be "SDL_Input".
3- If there are no error in your program, the compilation should have produced
	a file named a.out (under Linux) which is your executable.
Note: you may add a "-o [executableName]" to the above compilation line to
	choose the name of the generated executable (so it won't be named a.out).

=============
2. Update log:
=============

(version 4.0.1) 
---------------
-Added p_b_exit parameter to function SDL_Input_InputString();
-Added supported keys in SDL_Input_Definitions documentation page;

(version 4.0.0)
---------------
-Splitted SDL_Input and SDL_Input_TTF;
-Splitting those means that SDL_Input is now licenced under GNU LGPL
	(SDL_Input_TTF is still under GNU GPL since it requires SDL_TTF);
-SDL_INPUT_NOTHING macro removed (no more useful since SDL_Input_EmptyString()
	exists);
-SDL_Input->string may now be empty (all '\0');
-Removed SDL_Input->f() and SDL_Input->params (no more useful since unlocked
	input can be achieved via p_event parameter (see examples for more infos));
-Finally made those documentation pages. These are plain text files until
	someone has the courage to make HTML pages out of these;
-Changed most of library functions prototypes;
-Edited this README file so lines ain't more than 80 characters long;

(version 3.0.0)
------------------------
-Corrected licence in source files to GNU GPL (actual licence);
-Added SDL_Input.f() and SDL_Input.params elements to control events while in
	locking input mode (see SDL_Input_InputString documentation page for more
	infos);
-Fixed SDL_Input_InputString() bug when b_lockingInput was 0;
-Rearranged versioning order in this README file;
-Greatly improved 'unlocked input':
-b_lockingInput parameter is now replaced by a pointer to SDL_Event ( NULL =
	b_lockingInput of 1 );
-SDL_Input functions won't remove events from the queue in 'unlocked' mode
	anymore;
-Rendered SDL_Input_ComputeOffset() function public;
-Rewrote documentation according to all those changes;
-Added examples pages (polling with SDL_Input, using SDL_Input.f and
	SDL_Input.params, etc.);
-Added SDL_Input_IsHandled() function to know if a certain event is handled by
	SDL_Input;
-Added SDL_INPUT_IS_PRINT() macro to know if a certain key press unicode value
	is printable;
-Added insert key handling (toggles between insert and crush modes);
-Added SDL_INPUT_INSERT_MODE enum;
-Added SDL_Input_InsertMode() function (handles insert key);
-Reorganized code for better overall flexibility and modularity;

-Unlike previously announced, SDL_Input_InputChar() is now internal use only
	('static'). See examples documentation page for more infos.

(version 2.0.0) August 15th 2008:
--------------------------------
-Corrected bug which would shift the whole screen image.
-Translated whole code from french to english.
-Changed functions' naming rules (SDL_InputFooBar() -> SDL_Input_FooBar()).
-Optimized SDL_Input-GetCursorPos() (got rid of useless malloc() call).
-Added "offset'ed" blit (input->position.w is now used).
-Splitted functions (shortened functions which, for most, ain't more than 40
	lines long).
-Grouped most parameters into SDL_Input structure.
-Corrected: cursor could move when string is emptied (one space character
	blitted).
-Added html pages documentation.
-Added this readme file.
-Added this update log.
-Calling SDL_Input_InputString() now empties string.
-Placed the library under the GNU GPL licence (since it needs SDL_TTF, I had no
	choice anyway).

(version 1.03) June 26th 2008:
-----------------------------
-Changed file names (SDL_input -> SDL_Input) which caused users problems because
	of the case of "Input".
-Everything relative to the SDL_Input library is now named after it (SDL_Input
	prefix).
-Corrected bug about the number of bytes allocated for SDL_Input->string in
	SDL_Input_Create().
-Corrected bug which placed a '\0' at curosr index when SDLK_RETURN was hit.
-Corrected some minor warnings issued by gcc with SDL_Input version 1.02.
-Added library's version macro (SDL_INPUT_VERSION).

