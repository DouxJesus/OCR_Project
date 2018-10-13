#include <stdio.h>
#include "Bitmap/bitmap.h"

int main() {
    printf("Hello, OCR Project!\n");

    int i,j;
    Bitmap* I = NouvelleImage(256,256);
    for(i=0;i<256;i++)
    {
        for(j=0;j<256;j++)
        {
            Pixel p;
            p.r = i;
            p.g = j;
            p.b = 0;
            SetPixel(I,i,j,p);
        }
    }
    //Bitmap* I = Charger("test.bmp");
    Sauver(I,"test.bmp");
    return 0;
}