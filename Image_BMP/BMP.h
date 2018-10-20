//
// Created by marine on 18/10/18.
//

#ifndef OCR_BMP_H
#define OCR_BMP_H
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "pixel_operations.h"


void init_sdl();
SDL_Surface* load_image(char *path);
SDL_Surface* display_image(SDL_Surface *img);
void wait_for_keypressed();

#endif //OCR_BMP_H
