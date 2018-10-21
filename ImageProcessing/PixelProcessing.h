//
// Created by pyrd on 16/10/18.
//

#ifndef OCR_PIXELPROCESSING_H
#define OCR_PIXELPROCESSING_H
#include <stdlib.h>
#include <SDL.h>


Uint32 get_pixel(SDL_Surface *surface, unsigned x, unsigned y);
void put_pixel(SDL_Surface *surface, unsigned x, unsigned y, Uint32 pixel);
void update_surface(SDL_Surface* screen, SDL_Surface* image);

#endif