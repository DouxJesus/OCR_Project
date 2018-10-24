//
// Created by pyrd on 16/10/18.
//

#ifndef OCR_RSLA_H
#define OCR_RSLA_H
#include <stdlib.h>
#include <SDL/SDL.h>
//SDL_Surface* RSLA(SDL_Surface *img);

typedef struct Rect
{
    int x, y;
    int width, height;
} Rect;

SDL_Surface* Process(SDL_Surface* img, SDL_Surface* output, int threshold ,int horizontal);
SDL_Surface* InitSurfaceFromAnother(SDL_Surface *img, SDL_Surface *mask);
SDL_Surface* Merge(SDL_Surface *mask1, SDL_Surface *mask2, SDL_Surface *output);
Rect* Create_Rect_List(SDL_Surface* mask);
Rect* Get_Rect(SDL_Surface* mask, Rect* array, int lenght, int IsHorizontalPass);
void Draw_Rect(SDL_Surface* mask, Rect rect);
#endif //OCR_RSLA_H
