//
// Created by pyrd on 16/10/18.
//

#include "RLSA.h"
#include "../Image_BMP/pixel_operations.h"
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"


SDL_Surface* InitSurfaceFromAnother(SDL_Surface *img, SDL_Surface *mask){
	if( img->flags & SDL_SRCCOLORKEY ) {
		mask = SDL_CreateRGBSurface( SDL_SWSURFACE, img->w, img->h, img->format->BitsPerPixel, img->format->Rmask, img->format->Gmask, img->format->Bmask, 0 ); }
	else { 
		mask = SDL_CreateRGBSurface( SDL_SWSURFACE, img->w, img->h, img->format->BitsPerPixel, img->format->Rmask, img->format->Gmask, img->format->Bmask, img->format->Amask );
	}
	return mask;
}


//Process method of RSLA
//img is a pointer to SDL_Surface to process
//threshold -> int -> the threshold value used in the RSLA algorithm 
//horizontal -> bool -> define if do horizontal pass (true) or vertical pass(false)
SDL_Surface* Process(SDL_Surface *img, SDL_Surface* output, int threshold ,int horizontal){
	output = NULL;
	output = InitSurfaceFromAnother(img, output); //Init output with img's dimension
	int width, height;
	width = img->w;
	height= img->h;
	output->h = height;
	output->w = width;
	//If the surface must be locked
    if( SDL_MUSTLOCK(img) )
    {
        //Lock the surface
        SDL_LockSurface(img);
    }
	if(horizontal){
		//DO HORIZONTAL PASS
    	//Go through rows
        for( int y = 0; y < output->h; y++)
        {
        	int adjacent_pix = 0;
        	//Go through columns
			for( int x = 0; x < output->w; x++)
   			{
        		//Get pixel
            	Uint32 pixel = get_pixel(img, x, y);
            	Uint8 r, g, b;
				SDL_GetRGB(pixel, img->format, &r, &g, &b);
            	if(r < 127 && g < 127 && b < 127){
            		//Pixel is black
            		put_pixel(output, x, y, pixel);
            		if(adjacent_pix <= threshold){
            			while(adjacent_pix > 0){
            				put_pixel(output, x - adjacent_pix, y, pixel);
            				adjacent_pix--;
            			}
            		}
            		else{
            			adjacent_pix = 0;
            		}
            	}
            	else{
					put_pixel(output, x, y, SDL_MapRGB(img->format, 255, 255, 255));
            		adjacent_pix++;
            	}
   			}	
       	}
	}
	else {
		//DO VERTICAL PASS
		for( int x = 0; x < output->w; x++ )
        {
        	int adjacent_pix = 0;
			for( int y = 0; y < output->h; y++)
   			{
            	Uint32 pixel = get_pixel(img, x, y);
            	Uint8 r, g, b;
				SDL_GetRGB(pixel, img->format, &r, &g, &b);
            	if(r < 127 && g < 127 && b < 127){
            		//Pixel is black
            		put_pixel(output, x, y, pixel);
            		if(adjacent_pix <= threshold){
            			while(adjacent_pix > 0){
            				put_pixel(output, x, y - adjacent_pix, pixel);
            				adjacent_pix--;
            			}
            		}
            		else{
            			adjacent_pix = 0;
            		}
            	}
            	else{
					put_pixel(output, x, y, SDL_MapRGB(img->format, 255, 255, 255));

            		adjacent_pix++;
            	}
   			}	
       	}

	}

	//Unlock surface
    if(SDL_MUSTLOCK(img))
    {
        SDL_UnlockSurface(img);
    }
    return output;
}

SDL_Surface* Merge(SDL_Surface *mask1, SDL_Surface *mask2, SDL_Surface *output){
	if( SDL_MUSTLOCK(mask1) )
	{
		//Lock the surface
		SDL_LockSurface(mask1);
	}
	if( SDL_MUSTLOCK(mask2) )
	{
		//Lock the surface
		SDL_LockSurface(mask2);
	}
	output = InitSurfaceFromAnother(mask1, output);
	if( SDL_MUSTLOCK(output) )
	{
		//Lock the surface
		SDL_LockSurface(output);
	}
	for( int y = 0; y < mask1->h; y++)
     {
		for( int x = 0; x < mask1->w; x++)
		{
			//Get pixel
           	Uint32 pixel_h = get_pixel(mask1, x, y);
           	Uint8 rh, gh, bh;
			SDL_GetRGB(pixel_h, mask1->format, &rh, &gh, &bh);
			Uint32 pixel_v = get_pixel(mask2, x, y);
           	Uint8 rv, gv, bv;
			SDL_GetRGB(pixel_v, mask2->format, &rv, &gv, &bv);
			if(rh < 127){
					//on both mask, pixel is black
					put_pixel(output, x, y, pixel_v);
       		}
       		else
			{
       			put_pixel(output, x, y, pixel_h);
			}
		}
	}
	if(SDL_MUSTLOCK(mask1))
	{
		SDL_UnlockSurface(mask1);
	}
	if(SDL_MUSTLOCK(mask2))
	{
		SDL_UnlockSurface(mask2);
	}
	if( SDL_MUSTLOCK(output) )
	{
		//Lock the surface
		SDL_UnlockSurface(output);
	}
	return output;
}



// 1. faire une queue et rentrer le premier rect de la taille de l'image
// 2. faire une liste de rectangle (la liste à retourner, la liste qui contiens les rectangles "finis")
// 3. faire fonction d'extraction
    //3.1 defile un element
    //3.2 apply process sur ce rect depiler
    //3.3 si reste des choses à faire, on empile 
    //3.4 sinon le rect est fini, on l'ajoute à la liste


//Initialise the queue given in parameter, by enqueuing the first rectangle, which is the size of the image
int InitQueue(Queue* q, int w, int h){
    Node* pN = (Node*) malloc(sizeof (Node));
    pN->data = CreateRect(0, 0, w, h);
    Enqueue(q, pN);
    AddMarker(q);
    return 1;
}

void AddMarker(Queue* q)
{
    Node* null = (Node*) malloc(sizeof (Node));
    null->data = NULL;
    Enqueue(q, null);
}

void AddToList(Rect_List* rect_list, Rect* item){
    if(rect_list->list == 0)
    {
        rect_list->list = malloc(sizeof(Rect*));
        rect_list->length = 1;
        rect_list->list[0] = item;
    } else if (!(rect_list->list = realloc(rect_list->list, (rect_list->length + 1) * sizeof(Rect*)))) {
        exit(-1);
    } else {
        rect_list->length++;
        rect_list->list[rect_list->length-1] = item;
    }
}

void ClearList(Rect_List* rect_list){
    int i;
    for (i = 0; i < rect_list->length; ++i)
    {
        printf("Free n°%i\n",i);
        free(rect_list->list[i]);
    }
    
    free(rect_list->list);
    free(rect_list);

}

Rect* CreateRect(int x, int y, int width, int height)
{
    Rect* rect = (Rect*) malloc(sizeof(Rect));
    rect->x = x;
    rect->y = y;
    rect->width = width;
    rect->height = height;
    return rect;
}


//Return 0 if process could not do better
//Return 1 if process was succesful
int ExtractionProcess(Queue* q, SDL_Surface *mask, Rect * rectangle, int horizontal){
    int rect_x = rectangle->x, rect_y = rectangle->y;           //Coordinates of top-left corner of the rectangle given in parameters
    int relative_rect_width = rectangle->width;
    int relative_rect_height = rectangle->height;               //Relatives lengths of the rectangle
    int rect_width = relative_rect_width + rectangle->x;
    int rect_height = relative_rect_height + rectangle->y;      //Height & Width of the rectangle of the rectangle
    int first_x = 0, first_y = 0;
    int isWhiteLine = 1;
    int wasWhiteLine = 1;

    int succesful = 0;                                          //Define if the function was succesful or not

    //printf("Process data : %i %i %i %i\n",rect_x,rect_y,  );
    if(horizontal >0){
        wasWhiteLine = 1;
        int j = 0;
        for(j = rect_y; j < rect_height; j++){
            //printf("PROCESS1: firsty : %i, j :%i \n", first_y, j);
            isWhiteLine = 1; 
            int i = rect_x;
            while(isWhiteLine == 1 && i < rect_width){      //Check isWhiteLine
                Uint32 pixel = get_pixel(mask, i, j);
                Uint8 r, g, b;
                SDL_GetRGB(pixel, mask->format, &r, &g, &b);
                if(b < 127){                                //Pixel is black
                    isWhiteLine = 0;                        //Line has a black line, is not a white line
                }
                i++;
            }
            if (isWhiteLine){                              //if line IS a white line
                if (!wasWhiteLine){                        //End of bloc  --- Was = 1, Is = 0
                    Node *pN = (Node*) malloc(sizeof(Node));
                    printf("P1 : rect_x:%i - first_y:%i - relative_rect_width:%i - j: %i - first_y:%i\nOUTPUT:%i\n",rect_x, first_y, relative_rect_width, j, first_y,j - first_y);
                    pN->data = CreateRect(rect_x, first_y, relative_rect_width, j - first_y);
                    Enqueue(q, pN);
                    succesful=1;
                }
                wasWhiteLine = 1;
            } else {                                        //if line IS NOT a white line
                if(wasWhiteLine){                           //Beginning of bloc --- Was = 0, Is = 1
                    first_y = j;
                }
                wasWhiteLine = 0;
            }
        }
        //printf("PROCESS2: firsty : %i, j :%i \n", first_y, j);
        if (!wasWhiteLine ) {             //End of last possible bloc
            Node *pN = (Node*) malloc(sizeof (Node));
            //printf("PROCESS2 %i: x%i y%i - W%i H%i\n", horizontal, rect_x, first_y, relative_rect_width, relative_rect_height - first_y);
            //printf("PROCESS2: firsty : %i, j :%i R_Rect_H : %i -- x%i y%i - W%i H%i\n", first_y, j, relative_rect_height, rect_x, first_y, relative_rect_width, j - first_y);
            printf("P2 : rect_x:%i - first_y:%i - relative_rect_width:%i - relative_rect_height: %i - first_y:%i\nOUTPUT:%i\n", rect_x, first_y, relative_rect_width, relative_rect_height, first_y,  relative_rect_height - first_y);
            pN->data =  CreateRect(rect_x, first_y, relative_rect_width, rect_height - first_y);
            Enqueue(q, pN);
            succesful=1;
        }
        return succesful;
    } else {    //Vertical pass
        wasWhiteLine = 1;
        int i = 0;
        for (i = rect_x; i < rect_width; i++){
            isWhiteLine = 1;
            int j = rect_y;
            while(isWhiteLine == 1 && j < rect_height){         //Check Whiteline
                Uint32 pixel = get_pixel(mask, i, j);
                Uint8 r, g, b;
                SDL_GetRGB(pixel, mask->format, &r, &g, &b);
                if(b < 127){                                    //Pixel is black
                    isWhiteLine = 0;                            //Line has a black line, is not a white line
                }
                j++;
            }
            if (isWhiteLine){                                   //if line IS a white line
                if (!wasWhiteLine){                             //Previous line was a BLACK line,
                    Node *pN = (Node*) malloc(sizeof (Node));
                    //printf("PROCESS3 %i: x%i y%i - W%i H%i\n", horizontal, first_x, rect_y, i - first_x, relative_rect_height);
                    printf("P3 : first_x:%i - rect_y:%i - i:%i - firstx: %i - relative_rect_height:%i\nOUTPUT:%i\n",first_x, rect_y, i,first_x, relative_rect_height,i - first_x);
                    pN->data = CreateRect(first_x, rect_y, i - first_x, relative_rect_height);
                    Enqueue(q, pN);
                    succesful=1;
                }
                wasWhiteLine = 1;
            } else {
                if(wasWhiteLine){                           //Line is not WHITE, is BLACK, previous one was -> New Rectangle
                    first_x = i;
                }
                wasWhiteLine = 0;
            }
        }
        if (!wasWhiteLine ) { //la ligne précédente n'est pas blanche, on peut faire un bloc
            Node *pN = (Node*) malloc(sizeof (Node));
            //printf("\nPROCESS4 %i: x%i y%i - W%i H%i\n", horizontal, first_x, rect_y, relative_rect_width - first_x, relative_rect_height);
            printf("P4 : first_x:%i - rect_y:%i - relative_rect_width:%i - firstx: %i - relative_rect_height:%i\nOUTPUT:%i\n",first_x, rect_y, relative_rect_width,first_x, relative_rect_height, relative_rect_width - first_x);
            pN->data = CreateRect(first_x, rect_y, rect_width - first_x, relative_rect_height);
            Enqueue(q, pN);
            succesful=1;
            
        }
        return succesful;
    }
    return succesful;
}
    
//Function to extract the rectangles from the images with the mask given
Rect_List* Extraction(SDL_Surface* mask){
    Queue* q = ConstructQueue(-1);                                                  //Queue of rectangles to process
    Rect_List* output = malloc(sizeof(Rect_List));                                  //Array of rectangles ready
    output->list = 0;
    output->length = 0;
    InitQueue(q, mask->w, mask->h);
    int horizontalPass = -1;   //1 is horizontal, -1 is vertical                                              //Define if process do a horizontal pass or not
    int processCount = 0, switchCount = 0, AddToListCount = 0;
    int endCase = 0;           //1 is sucess, 0 is not sucessful
    //while (!isEmpty(q)) {
    while(switchCount <= 10){
        Node* pNode = Dequeue(q);
        if(pNode->data == NULL && !isEmpty(q)){
            horizontalPass = -horizontalPass;                                 //Switch between Horizontal Passes & Vertical Passes
            AddMarker(q);
            switchCount++;
            if(horizontalPass > 0){
                //printf("Switch n°%i : to Horizontal\n",switchCount);
            } else {
                //printf("Switch n°%i : to Vertical\n",switchCount);
            }
            
        }
        else{
            endCase = 0;
            Rect* tmp = pNode->data;
            printf("Node : H%i W%i - x%iy%i - ", tmp->height, tmp->width, tmp->x, tmp->y);
            printf("Process n°%i \n", processCount);
            //printf(" - Still in queue : %i - ", q->size);                                                               //Rectangle is not finished, apply process another time
            endCase = ExtractionProcess(q, mask, tmp, horizontalPass);
            processCount++;
            
            //if(endCase >= 0){
            if(switchCount == 3){  
                AddToList(output, tmp);
                AddToListCount++;
                //printf(" - AddToList n°%i\n",AddToListCount);   
            }  
            else {
                printf("\n");
            }
        }
    }
    DestructQueue(q);                                                      //Extraction is finished, free queue
    printf("Extraction completed - Bloc extracted : %i - Number of switch : %i\n", AddToListCount, switchCount);
    return output;
}

void Draw_Rect(SDL_Surface* mask, Rect rect, int color) {
    Uint32 pixel_color;
    if(color)
        pixel_color = SDL_MapRGB(mask->format, 0, 0, 255);
    else
        pixel_color = SDL_MapRGB(mask->format, 0, 200, 20);


    int x = rect.x, y = rect.y;
    int width = rect.width, height = rect.height;
    if(x > mask->w || y > mask->h || width > mask->w || height > mask->h){
        printf("DrawRect :  INVALID RECT\n");
        exit(-1);
    }
    for (int i = 0; i < width && (x + i) < mask->w; ++i) {
            put_pixel(mask, x + i, y, pixel_color);
            if (height + y != 0)
                put_pixel(mask, x + i, y + height - 1, pixel_color);
        }
    for (int j = 0; j < height && (y + j) < mask->h; ++j) {
        put_pixel(mask, x, y + j, pixel_color);
        if (width + x != 0)
            put_pixel(mask, x + width - 1, y + j, pixel_color);
    }
}
