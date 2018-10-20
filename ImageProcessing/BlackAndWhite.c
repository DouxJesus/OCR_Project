//
// Created by pyrd on 13/10/18.
//

#include "BlackAndWhite.h"
#include "../Image_BMP/pixel_operations.h"
#include "../Image_BMP/BMP.h"

Uint32 ToBlackOrWhite(SDL_Surface* img, unsigned int i, unsigned int j, int seuil)
{
    Uint32 pixel = get_pixel(img, i, j);
    Uint8 r, g, b;
    SDL_GetRGB(pixel, img->format, &r, &g, &b);
    Uint8 average = (r + g + b) / (Uint8) 3;

    if (average > seuil)
        pixel = SDL_MapRGB(img->format, 255, 255, 255);
    else
        pixel = SDL_MapRGB(img->format, 0, 0, 0);

    return pixel;

}

void to_grayscale(SDL_Surface *img)
{
    unsigned int width = (unsigned int) img->w;
    unsigned int height = (unsigned int) img->h;
    for (int unsigned i = 0; i < width; i++) {
        for (unsigned int j = 0; j < height; j++) {
            Uint32 pixel = get_pixel(img, i, j);
            Uint8 r, g, b;
            SDL_GetRGB(pixel, img->format, &r, &g, &b);
            Uint8 average = (Uint8) (0.3*r + 0.59*g + 0.11*b);
            pixel = SDL_MapRGB(img->format, average, average, average);
            put_pixel(img, i, j, pixel);
        }
    }
}

void gammaCorrection(SDL_Surface *img)
{
    //the gamma
    SDL_MapRGB(img->format, 4,4,4);
}


void BlackAndWhite(SDL_Surface* img)
{
    unsigned int height = img->h;
    unsigned int weight = img->w;
    for (unsigned int i = 0; i < weight; ++i) {
        for (unsigned int j = 0; j < height; ++j) {

            put_pixel(img, i, j, ToBlackOrWhite(img, i, j, 255/2));
        }
    }
}

void SauvolaBinarization(SDL_Surface* img)
{

}
/*

void BlackAndWhiteRelative(SDL_Surface* img)
{
//seuil relatif à l'image
}


void Sharpen(SDL_Surface* img)
{
//netteté
}*/