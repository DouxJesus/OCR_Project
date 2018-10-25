//
// Created by pyrd on 16/10/18.
//
#include "../Image_BMP/BMP.h"
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

typedef struct Rect_List
{
    int lenght;
    Rect* list;
} Rect_List;

typedef struct Rect_Mask_Couple
{
    SDL_Surface* mask;
    Rect_List rect_list;
} Rect_Mask_Couple;

SDL_Surface* Process(SDL_Surface* img, SDL_Surface* output, int threshold ,int horizontal);
SDL_Surface* InitSurfaceFromAnother(SDL_Surface *img, SDL_Surface *mask);
SDL_Surface* Merge(SDL_Surface *mask1, SDL_Surface *mask2, SDL_Surface *output);

Rect_List Create_Rect_List(SDL_Surface* mask);
Rect_Mask_Couple Get_Rect(SDL_Surface* mask, Rect_List list, int begin, int IsHorizontalPass, int iteration);
Rect CreateRect(int x, int y, int width, int height);
Rect_List AddElement(Rect_List list, int x, int y, int width, int height);
SDL_Surface* Draw_Rect(SDL_Surface* mask, Rect rect, Uint32 pixel_color);
#endif //OCR_RSLA_H
