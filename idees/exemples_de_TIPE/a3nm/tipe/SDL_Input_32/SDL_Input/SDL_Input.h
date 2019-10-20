/* Copyright (C) 2008 Diotte Olivier

This file is part of the SDL_Input library

    SDL_Input is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    SDL_Input is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with SDL_Input.  If not, see <http://www.gnu.org/licenses/>.
*/

/*This library uses the excellent SDL library:
 * www.libsdl.org
 *
 * Nota: Shall there be any error, bug, mistake or other form of inconsistency,
 * 	please email me at vhann3000 at gmail dot com
 */	

/*SDL_Input.h
 *------------
 *Author: Olivier Diotte
 *Created on: March 18th, 2008
 *Last update: Oct. 4th, 2008
 */

#ifndef _SDL_INPUT_H_GUARD
#define _SDL_INPUT_H_GUARD

/*version-related macros (thanks to SDL_ttf.h for guidelines)*/
#define SDL_INPUT_MAJOR_VERSION 4
#define SDL_INPUT_MINOR_VERSION	0
#define SDL_INPUT_PATCHLEVEL	1

#define SDL_INPUT_VERSION( X )				\
{											\
	(X)->major = SDL_INPUT_MAJOR_VERSION;	\
	(X)->minor = SDL_INPUT_MINOR_VERSION;	\
	(X)->patch = SDL_INPUT_PATCHLEVEL;		\
}

#define SDL_INPUT_CAPS ( SDL_GetModState() & ( KMOD_SHIFT | KMOD_CAPS ))
#define SDL_INPUT_IS_PRINT( x ) \
		( !iscntrl( x.key.keysym.unicode ))

typedef enum {
	SDL_INPUT_INS_MOD_QUERY		= 0,
	SDL_INPUT_INS_MOD_INSERT	= 1,
	SDL_INPUT_INS_MOD_CRUSH		= 2,
	SDL_INPUT_INS_MOD_TOGGLE	= 4
} SDL_INPUT_INSERT_MODE;

typedef struct {
    unsigned char	*string;		/*Read-only*/
	long			cursorIndex,	/*Read-only*/
					maxChars;		/*Read-only*/
	SDL_Event		lastEvent;
} SDL_Input;

SDL_Input *SDL_Input_Create( long );

void SDL_Input_Free( SDL_Input * );

int SDL_Input_InputString( SDL_Input *, SDL_Event *, int *, int * );

int SDL_Input_IsHandled( SDL_Event );

void SDL_Input_EmptyString( SDL_Input * );

SDL_INPUT_INSERT_MODE SDL_Input_InsertMode( SDL_INPUT_INSERT_MODE );

void SDL_Input_SetText( SDL_Input *, const char * );

int SDL_Input_SetCursorIndex( SDL_Input *, long );

#endif /*Guard*/
