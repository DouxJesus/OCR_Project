

#ifndef OCR_PROJECT_BLACKANDWHITE_H
#define OCR_PROJECT_BLACKANDWHITE_H

#include "SDL/SDL.h"
Uint32 ToBlackOrWhite(SDL_Surface* img, unsigned int i, unsigned int j, int seuil);
void to_grayscale(SDL_Surface *img);
void BlackAndWhite(SDL_Surface* img);
//void Contrast(SDL_Surface* img);
Uint32 mini, maxi;
/*void BlackAndWhiteRelative(SDL_Surface* img);
void Sharpen(SDL_Surface* img);*/
#endif //OCR_PROJECT_BLACKANDWHITE_H
