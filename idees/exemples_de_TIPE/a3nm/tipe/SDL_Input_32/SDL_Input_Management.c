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

/*SDL_Input_Management.c
 *------------
 *Author: Olivier Diotte
 *Created on: Sept. 3rd, 2008
 *Last update: Oct. 4th, 2008
 */

#include <stdio.h>
#include <stdlib.h>

#include <SDL/SDL.h>

#include "SDL_Input.h"

/*This function allocates a SDL_Input struct and returns its address
 * if successful or NULL otherwise*/
SDL_Input* SDL_Input_Create( long maxChars ) {

    SDL_Input *input = NULL;
    if( NULL != ( input = malloc( sizeof( SDL_Input )))) {
        if( NULL != ( input->string = malloc( maxChars *
				sizeof( *input->string ) + 1 ))) {
            input->cursorIndex	= 0;
			input->maxChars		= maxChars;
			SDL_Input_EmptyString( input );
        } else {
            SDL_Input_Free( input ), input = NULL;
        }
    }

    return input;
}

/*This function frees memory used by the SDL_Input pointer.*/
void SDL_Input_Free( SDL_Input *input ) {
	if( input != NULL ) {
    	free( input->string ), input->string = NULL;
    	free( input ), input = NULL;
	}
}

/*Returns -1 if unicode wasn't enabled or 1 or 0 depending on if key is handled
 * otherwise*/
int SDL_Input_IsHandled( SDL_Event event ) {
	int b_handled = ( SDL_EnableUNICODE( -1 ) ? 0 : -1 );

	if( b_handled != -1 ) {
		switch( event.type ) {
		case SDL_KEYDOWN:
			/* *MUST* use !iscntrl() rather than isprint() */
			if( !( b_handled = !iscntrl( event.key.keysym.unicode ))) {
				switch( event.key.keysym.sym ) {
				case SDLK_BACKSPACE:
				case SDLK_DELETE:
				case SDLK_RETURN:
				case SDLK_ESCAPE:
				case SDLK_LEFT:
				case SDLK_RIGHT:
				case SDLK_HOME:
				case SDLK_END:
				case SDLK_INSERT:
					b_handled = 1;
					break;
				default:
					;
				}
			}
		}
	}

	return b_handled;
}

void SDL_Input_EmptyString( SDL_Input *input ) {
	memset( input->string, '\0', input->maxChars );
    input->cursorIndex	= 0;
}

SDL_INPUT_INSERT_MODE SDL_Input_InsertMode( SDL_INPUT_INSERT_MODE insMode ) {
	static SDL_INPUT_INSERT_MODE insertMode = SDL_INPUT_INS_MOD_INSERT;

	switch( insMode ) {
	case SDL_INPUT_INS_MOD_INSERT:
	case SDL_INPUT_INS_MOD_CRUSH:
		insertMode = insMode;
		break;
	case SDL_INPUT_INS_MOD_TOGGLE:
		insertMode ^= 1;
	default:
		;
	}

	return insertMode;
}

void SDL_Input_SetText( SDL_Input *input, const char* str ) {
	long length;

	strncpy( (char*)input->string, str, input->maxChars );
	input->string[ input->maxChars + 1 ] = '\0';

	if( input->cursorIndex > ( length = (long)strlen( (char*)input->string ))) {
		input->cursorIndex = length;
	}
}

/*Returns 0 if succesful or -1 if it fails.*/
int SDL_Input_SetCursorIndex( SDL_Input *input, long cursorIndex ) {
	int b_error = -1;

	if( cursorIndex > -1 && cursorIndex <= input->maxChars ) {
		input->cursorIndex = cursorIndex;
		b_error = 0;
	}

	return b_error;
}
