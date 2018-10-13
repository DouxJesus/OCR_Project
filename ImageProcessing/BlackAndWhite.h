//
// Created by pyrd on 13/10/18.
//
#include "../Bitmap/bitmap.h"

#ifndef OCR_PROJECT_BLACKANDWHITE_H
#define OCR_PROJECT_BLACKANDWHITE_H
Pixel ToBlackOrWhite(Bitmap* img, int i, int j, int seuil);
void BlackAndWhite(Bitmap* img);
void BlackAndWhiteRelative(Bitmap* img);
void Sharpen(Bitmap* img);
#endif //OCR_PROJECT_BLACKANDWHITE_H
