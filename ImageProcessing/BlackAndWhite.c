//
// Created by pyrd on 13/10/18.
//

#include "BlackAndWhite.h"

Pixel ToBlackOrWhite(Bitmap* img, int i, int j)
{
    Pixel p = GetPixel(img, i, j);
    int g = (p.b + p.g + p.r) / 3;
    if (g > 255 / 2)
    {
        p.b = 255;
        p.r = 255;
        p.g = 255;
    }
    else {
        p.b = 0;
        p.r = 0;
        p.g = 0;
    }

    return p;

}


void BlackAndWhite(Bitmap* img)
{
    int height = img->h;
    int weight = img->w;
    for (int i = 0; i < weight; ++i) {
        for (int j = 0; j < height; ++j) {
            SetPixel(img, i, j, ToBlackOrWhite(img, i, j));
        }
    }
}