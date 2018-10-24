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

void BlackAndWhite(SDL_Surface* img)
{
    unsigned int height = img->h;
    unsigned int weight = img->w;
    for (unsigned int i = 0; i < weight; ++i) {
        for (unsigned int j = 0; j < height; ++j) {

            put_pixel(img, i, j, ToBlackOrWhite(img, i, j, 150));
        }
    }
}

int abs(int a)
{
    if (a < 0)
        return -a;
    return a;
}

void Contrast(SDL_Surface* img)
{
    unsigned int width = (unsigned int) img->w;
    unsigned int height = (unsigned int) img->h;
    for (int unsigned i = 0; i < width; i++) {
        for (unsigned int j = 0; j < height; j++) {

            Uint32 pixel = get_pixel(img, i, j);
            Uint8 r, g, b;
            SDL_GetRGB(pixel, img->format, &r, &g, &b);

            unsigned int k = i - 2, l = j - 2;
            int average_around = 0, nb = 1;
            Uint32 r_a = 0, g_a = 0, b_a = 0;
            while (k < width && k <= i + 2)
            {
                if (k > 0) {
                    unsigned int tmp = l;
                    while (l < height && l <= j + 2) {
                        if(l > 0) {
                            Uint32 p = get_pixel(img, k, l);
                            Uint8 r1, g1, b1;
                            SDL_GetRGB(p, img->format, &r1, &g1, &b1);
                           r_a += r1;
                           b_a += b1;
                           g_a += g1;
                            nb++;
                        }
                        l++;
                    }
                    l = tmp;
                }
                k++;
            }
            if (nb != 0) {
                int moy = SDL_MapRGB(img->format, r_a / nb, g_a / nb, b_a / nb);
                int new_pix = abs(pixel - moy) / abs(pixel + moy);
                put_pixel(img, i, j, new_pix);
            }
        }
    }
}

/*void SauvolaBinarization(SDL_Surface* img)
{

}*/
/*

void BlackAndWhiteRelative(SDL_Surface* img)
{
//seuil relatif à l'image
}


void Sharpen(SDL_Surface* img)
{
//netteté
}*/