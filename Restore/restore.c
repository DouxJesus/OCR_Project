#include <stdio.h>
#include <stdlib.h>
#include "restore.h"

void AddToString(String* str, char c){
	//printf("AddToString :  %c\n",c);
    if(str->length == 0)
    {
        str->string = malloc(sizeof(char));
        str->length = 1;
        str->string[0] = c;
    } else if (!(str->string  = realloc(str->string, (str->length + 1) * sizeof(char)))) {
        exit(-1);
    } else {
        str->length++;
        str->string[str->length-1] = c;
    }
}

String* stringyfy(Word_List* word_list){
	int len_list = word_list->length;
	String* output= (String*) malloc(sizeof(String));
	output->length = 0;
	int i = 0;
	int last_x = 0;
	int last_y = 0;
	while(i < len_list){
		int j = 0;
		int len_word = word_list->list[i]->length;
		//printf("stringify x%i y%i\n",last_x, last_y );
		while(j < len_word){
			//printf("word n°%i letter n°%i\n",i,j);
			AddToString(output, word_list->list[i]->list_of_letters[j]->letter);
			j++;
		}
		if(word_list->list[i]->x < last_x || word_list->list[i]->y > last_y){
			AddToString(output, ' ');
		} else {
			AddToString(output, '\n');
		}
		last_x = word_list->list[i]->x;
		last_y = word_list->list[i]->y;
		i++;
	}
	AddToString(output, 0);
	//printf("Stringfy : %i - %s\n",output->length, output->string);
	return output;
}