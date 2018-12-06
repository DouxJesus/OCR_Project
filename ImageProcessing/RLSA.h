//
// Created by pyrd on 16/10/18.
//
#include "../Image_BMP/BMP.h"
#ifndef OCR_RLSA_H
#define OCR_RLSA_H
#include <stdlib.h>
#include <SDL/SDL.h>
#include "Queue.h"
#include "../Restore/restore.h"

typedef struct Rect
{
    int x, y;
    int width, height;
} Rect;

typedef struct Rect_List
{
    int length;
    Rect** list;
} Rect_List;

typedef struct Matrix_Rect
{
    int x, y;
    int width, height;
    double* matrix;
} Matrix_Rect;

typedef struct Matrix_Rect_List
{
    int length;
    Matrix_Rect** list_matrix;
} Matrix_Rect_List;



Word_List* RLSA(SDL_Surface* image, SDL_Surface* screen, int size);
SDL_Surface* Process(SDL_Surface* img, SDL_Surface* output, int threshold ,int horizontal);
SDL_Surface* InitSurfaceFromAnother(SDL_Surface *img, SDL_Surface *mask);
SDL_Surface* Merge(SDL_Surface *mask1, SDL_Surface *mask2, SDL_Surface *output);
SDL_Surface* resizeImage(SDL_Surface* src, SDL_Surface* output, int r_width, int r_height);
Rect* CreateRect(int x, int y, int width, int height);
int InitQueue(Queue* q, int x, int y, int w, int h);
Rect_List* Extraction(SDL_Surface* mask, int rectMode, Rect* rect, int prints, int levelOfPrints);
int ExtractionProcess(Queue* q, SDL_Surface *mask, Rect * rectangle, int horizontal);
void AddToList(Rect_List* list, Rect* item);
void Draw_Rect(SDL_Surface* mask, Rect rect, int color);
void ClearList(Rect_List* rect_list, int prints);
void AddMarker(Queue* q);
void DisplayRLSA(Rect_List* rect_list, SDL_Surface* image, int prints);
Word_List* Wordify(SDL_Surface* image, Rect_List* rect_list);
Matrix_Rect_List* Matrixize(SDL_Surface* image, Rect_List* rect_list);
#endif //OCR_RLSA_H
