//
// Created by pyrd on 4/12/18.
//
#ifndef RESTORE
#define RESTORE
typedef struct Letter
{
    double* matrix;
    char letter;
} Letter;

typedef struct Word
{
    Letter** list_of_letters;
    int x;
    int y;
    int length;
} Word;

typedef struct Word_List
{
    int length;
    Word** list;
} Word_List;

typedef struct String
{
    int length;
    char* string;
} String;

String* stringyfy(Word_List* word_list);


#endif