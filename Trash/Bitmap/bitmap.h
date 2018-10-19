#ifndef BITMAP_H
#define BITMAP_H

typedef struct Pixel
{
    unsigned char r,g,b;
} Pixel;

typedef struct Bitmap
{
    int w, h;
    Pixel* tab;
} Bitmap;

//appel fonctions
Bitmap* NouvelleImage(int w,int h);
void SetPixel(Bitmap* img,int i,int j,Pixel p);
Pixel GetPixel(Bitmap* img,int i,int j);
Bitmap* Charger(const char* fichier);
int Sauver(Bitmap* I,const char* fichier);
#endif