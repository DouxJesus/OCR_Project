//
// Created by pyrd on 16/10/18.
//

#ifndef OCR_RSLA_H
#define OCR_RSLA_H
#include <stdlib.h>
#include <SDL.h>
SDL_Surface* RSLA(SDL_Surface *img);
SDL_Surface* Process(SDL_Surface *img, int threshold ,int horizontal);
SDL_Surface* InitSurfaceFromAnother(SDL_Surface *img, SDL_Surface *mask);

#endif //OCR_RSLA_H
