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

/*SDL_Input_TTF.c
 *-----------
 *Purpose: Text-blitting functions
 *Author: Olivier Diotte
 *Created on: March 17th 2008
 *Last update: Oct. 4th, 2008
 */

#include <stdio.h>
#include <stdlib.h>

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

#include "../SDL_Input.h"
#include "SDL_Input_TTF.h"

/*Checks if text can be displayed. Returns 0 if successful or -1 otherwise.*/
static int SDL_Input_TTF_CheckErrors( SDL_Input_TTF *ttf, SDL_Surface *dest ) {
	int b_error = 0;

	if( !TTF_WasInit() || !SDL_WasInit( SDL_INIT_VIDEO ) || ttf->font == NULL
			|| ttf->input->string == NULL ) {
		b_error = -1;
	}
	if( ttf->position.w > dest->w - ttf->position.x - 1 ||
			ttf->position.w < 1 ) {
		ttf->position.w = dest->w - ttf->position.x - 1;
	}

	return b_error;
}

/*Returns 0 if blit is successful or -1 otherwise*/
static int SDL_Input_TTF_BlitString( SDL_Surface *text, SDL_Surface *dest,
		SDL_Input_TTF *ttf ) {
	SDL_Rect	clip = { 0, 0, 0, 0 },
				pos = ttf->position;
	clip.w = ttf->position.w;
	clip.h = text->h;
	clip.x = ttf->offset;

	return SDL_BlitSurface( text, &clip, dest, &pos );
}

/*Returns o if successful or -1 otherwise*/
static int SDL_Input_TTF_BlitCursor( SDL_Surface *cursor, SDL_Surface *dest,
		SDL_Input_TTF *ttf ) {
	int b_error,
		cursorAbscissa;
	SDL_Rect pos = { 0, 0, 0, 0 };

	/*If no b_error is returned by SDL_Input_TTF_GetCursorPos*/
	if( !( b_error = -( 0 > ( cursorAbscissa =
			SDL_Input_TTF_GetCursorPos( ttf ))))) {
		pos.y = ttf->position.y;
		pos.x = ttf->position.x + cursorAbscissa - ttf->offset;
		b_error = SDL_BlitSurface( cursor, NULL, dest, &pos );
	}
	return b_error;
}

/*Returns SDL_Surface pointer to text surface if successful or NULL otherwise*/
static SDL_Surface *SDL_Input_TTF_RenderText( SDL_Input_TTF *ttf ) {
	SDL_Surface	*text;

	/*Renders text*/
	switch( ttf->rendering ) {
		case SDL_INPUT_TTF_BLENDED:
			text = TTF_RenderText_Blended( ttf->font,
					(char *)ttf->input->string, ttf->fg );
			break;
		case SDL_INPUT_TTF_SHADED:
			text = TTF_RenderText_Shaded( ttf->font, (char *)ttf->input->string,
					ttf->fg, ttf->bg );
			break;
		default:
			text = TTF_RenderText_Solid( ttf->font, (char *)ttf->input->string,
					ttf->fg );
			break;
	}

	return text;
}

/*Returns 0 if successful or -1 otherwise*/
static int SDL_Input_TTF_InitCursor( SDL_Surface **p_cursor, SDL_Surface *dest,
		SDL_Surface *text ) {
	int b_error = 0;

	/*Allocate cursor*/
	if ( *p_cursor == NULL ) {
		if(( *p_cursor = SDL_CreateRGBSurface( SDL_HWSURFACE, 2, text->h, 8, 0,
				0, 0, 0 )) == NULL ) {
			b_error = -1;
		} else {
			SDL_FillRect( *p_cursor, NULL, SDL_MapRGB( dest->format, 0, 0, 0 ));
		}
	}

	return b_error;
}

/*This function copies all pixels which could be erased by the user's input.
 * Thus the text may be "erased". Returns 0 if successful or -1 otherwise.*/
static int SDL_Input_TTF_InitBackSpace( SDL_Input_TTF *ttf, SDL_Surface *dest,
		SDL_Surface **backSpace ) {
	int b_error;
	int x = ttf->position.x,
		y = ttf->position.y,
		w = dest->w - x,
		h = dest->h - y;
	SDL_Rect clip = { 0, 0, 0, 0 };
	clip.x = x;
	clip.y = y;
	clip.w = w;
	clip.h = h;

	/*If allocation is successful*/
    if( !( b_error = ( NULL == ( *backSpace =
			SDL_CreateRGBSurface( SDL_HWSURFACE, w , h,
			dest->format->BitsPerPixel, 0, 0, 0, 0 ))))) {

		/*Copies dest's pixels*/
        SDL_BlitSurface( dest, &clip, *backSpace, NULL );
    }

    return -b_error;
}

SDL_Input_TTF *SDL_Input_TTF_Create( long maxChars, TTF_Font *font,
		SDL_Color fg, SDL_Color *p_bg, SDL_Rect position,
		SDL_INPUT_TTF_RENDER_TYPE rendering ) {
	SDL_Input_TTF *ttf	= NULL;

	if( NULL != ( ttf = malloc( sizeof( SDL_Input_TTF )))) {
		ttf->offset		= 0;
		ttf->rendering	= rendering;
		ttf->font		= font;
		ttf->fg			= fg;
		if( p_bg != NULL ) {
			ttf->bg			= *p_bg;
		}
		ttf->position	= position;
		if( NULL == ( ttf->input = SDL_Input_Create( maxChars ))) {
			SDL_Input_TTF_Free( ttf );
		}
	}

	return ttf;
}

void SDL_Input_TTF_Free( SDL_Input_TTF *ttf ) {
	if( ttf != NULL ) {
		SDL_Input_Free( ttf->input );
		TTF_CloseFont( ttf->font ), ttf->font = NULL;
		free( ttf ), ttf = NULL;
	}
}

/*This function computes the cursor's position and returns the actual x coord
 * of cursor relative to input->position.x or -1 in case of error
 * Please note the return value of this is not the real position of the
 * cursor ( return value - input->offset is ).*/
int SDL_Input_TTF_GetCursorPos( SDL_Input_TTF *ttf ) {
	unsigned char c;
	int width;

	c = ttf->input->string[ ttf->input->cursorIndex ];
	ttf->input->string[ ttf->input->cursorIndex ] = '\0';
	if( TTF_SizeText( ttf->font, (char *)ttf->input->string, &width, NULL )) {
		width = -1;
	}

	ttf->input->string[ ttf->input->cursorIndex ] = c;
	return width;
}

/*Returns 0 if successful or -1 otherwise*/
int SDL_Input_TTF_ComputeTextOffset( SDL_Input_TTF *ttf ) {
	int cursorAbscissa,
		textWidth,
		b_error;

	if( !( b_error = -( TTF_SizeText( ttf->font, (char *)ttf->input->string,
			&textWidth, NULL )))) {
		if( !( b_error = -( 0 > ( cursorAbscissa =
				SDL_Input_TTF_GetCursorPos( ttf ))))) {
			if( cursorAbscissa < ttf->offset ) {
				ttf->offset = cursorAbscissa;
			} else if( cursorAbscissa >= ttf->position.w + ttf->offset ||
					textWidth <= cursorAbscissa ) {
				ttf->offset = cursorAbscissa - ttf->position.w;
				if( ttf->offset < 0 ) {
					ttf->offset = 0;
				}
			}
		}
	}

	return b_error;
}

/*Computes real cursor's abscissa. Returns the latter or -1 if it fails*/
int SDL_Input_TTF_GetRealCursorPos( SDL_Input_TTF *ttf ) {
	int b_error = SDL_Input_TTF_ComputeTextOffset( ttf );

	if( !b_error ) {
		if( !( b_error = SDL_Input_TTF_GetCursorPos( ttf ))) {
			b_error -= ttf->offset;
		}
	}

	return b_error;
}

/*Returns 0 if successful or -1 otherwise.*/
int SDL_Input_TTF_Display( SDL_Input_TTF *ttf, SDL_Surface *dest,
		SDL_Surface *backSpace ) {

	SDL_Surface *text	= NULL,
				*cursor	= NULL;
	SDL_Rect	pos = { 0, 0, 0, 0 };
	int b_error = 0;

	if( !( b_error = SDL_Input_TTF_CheckErrors( ttf, dest )) &&
			(unsigned long)strlen( (char *)ttf->input->string ) > 0 ) {
		if( !( b_error = ( NULL == ( text =
				SDL_Input_TTF_RenderText( ttf ))))) {
			if( !( b_error = SDL_Input_TTF_InitCursor( &cursor, dest, text ))) {
				if( backSpace != NULL ) {
					pos = ttf->position;
					SDL_BlitSurface( backSpace, NULL, dest, &pos );
				}

				SDL_Input_TTF_ComputeTextOffset( ttf );
				SDL_Input_TTF_BlitString( text, dest, ttf );
				SDL_Input_TTF_BlitCursor( cursor, dest, ttf );
				SDL_Flip( dest );

                /*Frees memory*/
 				SDL_FreeSurface( cursor ), cursor = NULL;
			}
			SDL_FreeSurface( text ), text = NULL;
		}
	}

	return -b_error;
}

/*Returns 0 or -1 if it fails.*/
int SDL_Input_TTF_Input( SDL_Input_TTF *ttf, SDL_Surface *dest,
		SDL_Event *p_event ) {
	SDL_Surface *backSpace = NULL;
	int b_loop = ( p_event == NULL ),
		b_exit,
		b_error,
		b_destRefresh;

	if( !( b_error = SDL_Input_TTF_InitBackSpace( ttf, dest, &backSpace ))) {
		do {
			b_error = SDL_Input_InputString( ttf->input, p_event,
					&b_destRefresh, &b_exit );
			if( b_loop ) {
				SDL_Input_TTF_Display( ttf, dest, backSpace );
			}
		} while( b_loop && !b_error && !b_exit );
	}

	SDL_FreeSurface( backSpace ), backSpace = NULL;
	return b_error;
}
