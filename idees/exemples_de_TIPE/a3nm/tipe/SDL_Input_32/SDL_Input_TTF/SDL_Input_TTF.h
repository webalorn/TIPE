/* Copyright (C) 2008 Diotte Olivier

This file is part of the SDL_Input_TTF library

SDL_Input is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public
License along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA */

/*This sub-library is a complement to my own SDL_Input library:
 * http://www.siteduzero.com/forum-83-309905-2861119-sdl_input-des-zones-de-saisie.html#r2861119 (temporary link)

 *The excellent SDL and SDL_TTF libraries are also required:
 * http://www.libsdl.org/projects/SDL_ttf
 * www.libsdl.org
 */

/*SDL_Input_TTF.h
 *------------
 *Author: Olivier Diotte
 *Created on: Sept. 4th, 2008
 *Last update: Oct. 4th, 2008
 */

#ifndef _SDL_INPUT_TTF_H_GUARD
#define _SDL_INPUT_TTF_H_GUARD

#define SDL_INPUT_TTF_MAJOR_VERSION 1
#define SDL_INPUT_TTF_MINOR_VERSION 0
#define SDL_INPUT_TTF_PATCHLEVEL	0

#define SDL_INPUT_TTF_VERSION( X )				\
{												\
	(X)->major = SDL_INPUT_TTF_MAJOR_VERSION;	\
	(X)->minor = SDL_INPUT_TTF_MINOR_VERSION;	\
	(X)->patch = SDL_INPUT_TTF_PATCHLEVEL;		\
}

typedef enum {
	SDL_INPUT_TTF_SOLID,
	SDL_INPUT_TTF_SHADED,
	SDL_INPUT_TTF_BLENDED
} SDL_INPUT_TTF_RENDER_TYPE;

typedef struct {
	int offset;		/*Read-only*/
	SDL_INPUT_TTF_RENDER_TYPE rendering;
	TTF_Font	*font;
	SDL_Color	fg,
				bg;
	SDL_Rect	position;
	SDL_Input	*input;
} SDL_Input_TTF;

SDL_Input_TTF *SDL_Input_TTF_Create( long, TTF_Font *, SDL_Color,
		SDL_Color *, SDL_Rect, SDL_INPUT_TTF_RENDER_TYPE );

void SDL_Input_TTF_Free( SDL_Input_TTF * );

int SDL_Input_TTF_GetCursorPos( SDL_Input_TTF * );

int SDL_Input_TTF_GetRealCursorPos( SDL_Input_TTF * );

int SDL_Input_TTF_Display( SDL_Input_TTF *, SDL_Surface *, SDL_Surface * );

int SDL_Input_TTF_Input( SDL_Input_TTF *, SDL_Surface *, SDL_Event * );

#endif /*Guard*/
