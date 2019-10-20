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

/*SDL_Input_EventsHandling.c
 *------------
 *Purpose: SDL text writing functions
 *Author: Olivier Diotte
 *Created on: March 18th, 2008
 *Last update: Oct. 4th, 2008
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <limits.h>

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

#include "SDL_Input.h"

/*This function makes sure cursorPos never is out of bounds*/
static void SDL_Input_ControlString( SDL_Input *input ) {

	/*Can't point to '\0'*/
    if ( input->cursorIndex > input->maxChars ) {
        input->cursorIndex = input->maxChars;
    }
    if ( input->cursorIndex < 0 ) {
        input->cursorIndex = 0;
    }
}

/*This function initialize *input*/
static void SDL_Input_InitString( SDL_Input *input, int *p_b_emptyString ) {

	/*Avoids NULL pointer errors when blitting*/
    if( input->string[ 0 ] == '\0' ) {
		*p_b_emptyString = 1;
    }

	/*Makes very sure blit won't display an infinite string*/
	input->string[ input->maxChars ] = '\0';

	SDL_Input_ControlString( input );
}

/*Returns 0 if successful or -1 otherwise*/
static int SDL_Input_Init( SDL_Input *input, int *p_b_emptyString,
		int b_lockingInput ) {
	int b_error = 0;

	/*UNICODE *MUST* be enabled if input isn't locking*/
	if( !( b_error = !SDL_EnableUNICODE( -1 ) && !b_lockingInput )) {
		SDL_EnableUNICODE( 1 );
		SDL_Input_InitString( input, p_b_emptyString );
	}

	return -b_error;
}

/*This function handles characters deleting (del. and backSpace keys)*/
static void SDL_Input_DeleteChar( SDL_Input *input, int deleteKey,
		int *p_b_emptyString ) {

	/*If not out of bounds*/
    if(( input->cursorIndex > 0 && deleteKey == SDLK_BACKSPACE ) ||
            ( input->cursorIndex < input->maxChars &&
            deleteKey == SDLK_DELETE )) {

        if( deleteKey == SDLK_BACKSPACE ) {
            input->cursorIndex--;
        }
        input->string[ input->cursorIndex ] = '\0';
		strcat( (char *)input->string, (char *)( input->string +
				input->cursorIndex + 1 ));
    }

	/*Avoids text has zero width error when rendering (TTF_Render*)*/
    if ( 0 == (long)strlen( (char *)input->string )) {
        input->string[ 0 ] = ' ';
		*p_b_emptyString = 1;
    }
}

static int SDL_Input_HandlePrintableEvents( SDL_Input *input,
		int *p_b_emptyString ) {
	int b_printable = SDL_INPUT_IS_PRINT( input->lastEvent ),
		b_change = 1,
		unicode = input->lastEvent.key.keysym.unicode;
	long	length = (long)strlen( (char *)input->string ),
			curIndex = input->cursorIndex;

	/*Handles printable characters (UNICODE value)*/
	if( b_printable ) {
		if( SDL_INPUT_INS_MOD_INSERT ==
				SDL_Input_InsertMode( SDL_INPUT_INS_MOD_QUERY )) {
			if( length < input->maxChars - 1 ) {
				if( !*p_b_emptyString ) {
					memmove( &input->string[ curIndex + 1 ],
							&input->string[ curIndex ], length - curIndex );
				}
			} else {
				b_change = 0;
			}
		}
		
		if( b_change ) {
			/*Stores UNICODE key value in string and iterates index*/
			input->string[ input->cursorIndex++ ] = unicode;
			*p_b_emptyString = 0;
		}
	}

	return b_printable;
}

/*Returns 1 if SDL_Input has to quit or 0 otherwise*/
static int SDL_Input_HandleEvents( SDL_Input *input, int *p_b_destRefresh,
		int *p_b_emptyString ) {
	int b_exit = 0,
		key = input->lastEvent.key.keysym.sym;
	
	if( SDL_Input_IsHandled( input->lastEvent )) {
		if( !SDL_Input_HandlePrintableEvents( input, p_b_emptyString )) {
			switch( key ) {
			case SDLK_BACKSPACE: case SDLK_DELETE: /*BackSpace and Delete keys*/
				SDL_Input_DeleteChar( input, key, p_b_emptyString );
				break;
			case SDLK_LEFT: /*Left key*/
				if( input->cursorIndex > 0 ) {
					input->cursorIndex--;
				}
				break;
			case SDLK_RIGHT: /*Right key*/
				if( (long)strlen( (char *)input->string ) > input->cursorIndex &&
						!*p_b_emptyString ) {
					input->cursorIndex++;
				}
				break;
			case SDLK_HOME: /*Home key*/
				input->cursorIndex = 0;
				break;
			case SDLK_END: /*End key*/
				if( !*p_b_emptyString ) {
					input->cursorIndex = strlen( (char *)input->string );
				}
				break;
			case SDLK_INSERT: /*Insert key*/
				SDL_Input_InsertMode( SDL_INPUT_INS_MOD_TOGGLE );
				break;
			default:
				/*Exit if ENTER key is pressed*/
				b_exit = ( key == SDLK_RETURN );

				/*Screen won't be refreshed*/
				*p_b_destRefresh = 0;
			}
		}
		SDL_Input_ControlString( input );
	}

	return b_exit;
}

/*This function handles the first event in the queue.
 * Returns -1 if unicode wasn't enabled or 1 or 0 depending on if event is a
 * printable character or not.*/
static int SDL_Input_InputChar( SDL_Input *input, const SDL_Event *p_event,
		int *p_b_exit, int *p_b_destRefresh, int *p_b_emptyString ) {
	int b_error = ( SDL_EnableUNICODE( -1 ) ? 0 : -1 ),
		b_loop	= 1,
		b_lockingInput	= ( p_event == NULL ),
		b_exit;
	SDL_Event event;

	while( !b_error && b_loop ) {
		if( p_event == NULL ) {
			SDL_WaitEvent( &event );
		} else {
			event = *p_event;
		}
		input->lastEvent = event;
		b_loop = !SDL_Input_IsHandled( event );

		if( !b_lockingInput ) {
			b_loop = 0;
		}
	}
	
	b_exit = SDL_Input_HandleEvents( input, p_b_destRefresh, p_b_emptyString );
	if( p_b_exit != NULL ) {
		*p_b_exit = b_exit;
	}

	if( !b_error ) {
		b_error = SDL_INPUT_IS_PRINT( event );
	}
	return b_error;
}

/*Handles input. Generally speaking, it should be the only or so function
 * called by the user. Return 0 on success or -1 if it fails.*/
int SDL_Input_InputString( SDL_Input *input, SDL_Event *p_event,
		int *p_b_destRefresh, int *p_b_exit ) {
    int	b_emptyString		= !(long)strlen( (char *)input->string ),
		b_error				= 0,
        b_exit				= 0,
		b_destRefresh		= 1;

	if( !( b_error = SDL_Input_Init( input, &b_emptyString,
			( p_event == NULL )))) {

		if( !b_error ) {
			b_error = ( 0 > ( SDL_Input_InputChar( input, p_event, &b_exit,
					&b_destRefresh, &b_emptyString )));
			if( p_b_exit != NULL ) {
				*p_b_exit = b_exit;
			}
		}
	}

	if( b_emptyString ) {
		SDL_Input_EmptyString( input );
	}

	if( p_b_destRefresh != NULL ) {
		*p_b_destRefresh = b_destRefresh;
	}

	return b_error;
}
