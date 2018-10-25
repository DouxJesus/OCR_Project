//
// Created by pyrd on 16/10/18.
//

#include "RSLA.h"
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


Rect_List Create_Rect_List(SDL_Surface* mask)
{
    int lenght = 1;
    Rect* array;
    if(!(array = malloc(sizeof(Rect))))
    {
        exit(-1);
    }
    Rect first_mask;
    first_mask.x = 0;
    first_mask.y = 0;
    first_mask.width = mask->w;
    first_mask.height = mask->h;
    array[0]= first_mask;

    Rect_List res;
    res.lenght = lenght;
    res.list = array;

    return res;
}


//Get_Rect
//took a mask, a list of rectangles, the point from when we begin the loop and a bool : is it an horizontal pass or not
//return the list of blocs from the mask + mask
Rect_Mask_Couple Get_Rect(SDL_Surface* mask, Rect_List list, int begin, int IsHorizontalPass, int iteration) {

    if (iteration == 0)
    {
        Rect_Mask_Couple res;
        res.mask = mask;
        res.rect_list = list;
        return res;
    }
    int do_it_again = 0; //false

    Rect* array = list.list;

    int len = list.lenght;
    int init_lenght = len;
    //int width = mask->w;
    //int height = mask->h;

    for (int k = begin; k < init_lenght; ++k) { //go through the list of rect

        Rect rec_mask = array[k];

        int rect_x = rec_mask.x, rect_y = rec_mask.y;
        int relativ_rect_width = rec_mask.width, relativ_rect_height = rec_mask.height;
        int rect_width = relativ_rect_width + rec_mask.x, rect_height = relativ_rect_height + rec_mask.y;

        int WasWhiteLine = 0; //false

        int frst_x = rect_x, frst_y = rect_y;

        /*///////////////////////////////////////////////////////////////////////////////////////////////*/
        if (!IsHorizontalPass) { //Vetical Pass
            for (int i = rect_x; i < rect_width; i++) {

                int IsWhiteLine = 1; //true
                int j = rect_y;
                while (IsWhiteLine == 1 && j < rect_height) {

                    Uint32 pixel = get_pixel(mask, i, j);
                    Uint8 r, g, b;
                    SDL_GetRGB(pixel, mask->format, &r, &g, &b);

                    if (b < 127) { //le pixel est noir
                        IsWhiteLine = 0; //false
                    }
                    j++;
                }

                if (IsWhiteLine) {

                    if (!WasWhiteLine) { //la ligne précédente n'est pas blanche, on peut faire un bloc
                        printf("Vertical : len : %i\n", len);
                        len++;
                        if (NULL == (array = realloc(array, (len) * sizeof(Rect)))) {
                            printf("Realloc == NULL \n");
                            exit(-1);
                        }

                        Rect new_Rect;
                        new_Rect.x = frst_x;
                        new_Rect.y = rect_y;
                        new_Rect.width = i - frst_x;
                        new_Rect.height = rect_height;
                        array[len-1] = new_Rect;

                        //Draw_Rect(mask,new_Rect);

                        do_it_again++; //on a une ligne blanche donc on devra refaire forcément un tour sur tt les
                    }
                    else
                    {
                        Rect blueline;
                        blueline.height = relativ_rect_height;
                        blueline.width = 0;
                        blueline.x = i;
                        blueline.y = frst_y;
                        Draw_Rect(mask, blueline, 42);
                    }
                    WasWhiteLine = 1; //true
                } else
                {
                    if(WasWhiteLine){//la ligne n'est pas blanche et celle d'avant l'était donc c'est un nouveau rect
                        frst_x = i;
                    }
                    WasWhiteLine = 0; //false
                }
            }

            if (!WasWhiteLine  && rect_width> 0) { //la ligne précédente n'est pas blanche, on peut faire un bloc
                len++;
                if (!(array = realloc(array, (len) * sizeof(Rect)))) {
                    exit(-1);
                }
                printf("Horizontal : len : %i\n", len);
                Rect new_Rect;
                new_Rect.x = frst_x;
                new_Rect.y = frst_y;
                new_Rect.width = rect_width - frst_x - 1;
                new_Rect.height = rect_height;

                array[len-1] = new_Rect;
                //Draw_Rect(mask,new_Rect);
            }


        }

        /*///////////////////////////////////////////////////////////////////////////////////////////////*/
        else { //Horizontal Pass
            for (int i = rect_y; i < rect_height; i++) {

                int IsWhiteLine = 1; //true
                int j = rect_x;
                while (IsWhiteLine == 1 && j < rect_width) {

                    Uint32 pixel = get_pixel(mask, j, i);
                    Uint8 r, g, b;
                    SDL_GetRGB(pixel, mask->format, &r, &g, &b);

                    if (b < 127) { //le pixel est noir
                        IsWhiteLine = 0; //false
                    }
                    j++;
                }

                if (IsWhiteLine) {

                    if (!WasWhiteLine) { //la ligne précédente n'est pas blanche, on peut faire un bloc
                        printf("Horizontal : len : %i\n", len);
                        len++;
                        if (NULL == (array = realloc(array, (len) * sizeof(Rect)))) {
                            printf("Realloc == NULL \n");
                            exit(-1);
                        }

                        Rect new_Rect;
                        new_Rect.x = rect_x;
                        new_Rect.y = frst_y;
                        new_Rect.width = rect_width;
                        new_Rect.height = i - frst_y;
                        array[len-1] = new_Rect;

                        //Draw_Rect(mask,new_Rect);

                        do_it_again++; //on a une ligne blanche donc on devra refaire forcément un tour sur tt les
                    }
                    else
                    {
                        Rect blueline;
                        blueline.height = 0;
                        blueline.width = relativ_rect_width;
                        blueline.x = frst_x;
                        blueline.y = i;
                        Draw_Rect(mask, blueline, 42);
                    }
                    WasWhiteLine = 1; //true
                } else
                {
                    if(WasWhiteLine){//la ligne n'est pas blanche et celle d'avant l'était donc c'est un nouveau rect
                        frst_y = i;
                    }
                    WasWhiteLine = 0; //false
                }
            }

            if (!WasWhiteLine && rect_height > 0) { //la ligne précédente n'est pas blanche, on peut faire un bloc
                printf("Horizontal : len : %i\n", len);
                len++;
                if (!(array = realloc(array, (len) * sizeof(Rect)))) {
                    exit(-1);
                }
                Rect new_Rect;
                new_Rect.x = rect_x;
                new_Rect.y = frst_y;
                new_Rect.width = rect_width;
                new_Rect.height = rect_height - frst_y - 1;

                array[len-1] = new_Rect;

                //Draw_Rect(mask,new_Rect, 42);
            }
        }

    }

    printf("Final : len : %i\n", len);
    Rect_Mask_Couple res;

    //FreeFirst
    list.lenght = len;
    list.list = array;

    res.mask = mask;
    res.rect_list = list;

    //if(do_it_again)
    //{
        //wait_for_keypressed();
        if (IsHorizontalPass)
            res = Get_Rect(mask, list, init_lenght, 0, iteration -1);
        else
            res = Get_Rect(mask, list, init_lenght, 1, iteration -1);
   // }

    return res;
}

SDL_Surface* Draw_Rect(SDL_Surface* mask, Rect rect, Uint32 pixel_color) {
    printf("Draw  \n");
    if (pixel_color == 42)
        pixel_color = SDL_MapRGB(mask->format, 0, 0, 255);

    int x = rect.x, y = rect.y;
    int width = rect.width, height = rect.height;
    int w = mask->w, h = mask->h;
        printf("%i - %i - %i - %i - %i - %i \n", x, y, width, height, w, h);

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

    return mask;
}


Rect_List FreeBeginningOfRectList(Rect_List list,Rect* rectangles, int init_len, int new_len)
{
    //TODO
    list.list = rectangles;
    list.lenght = new_len - init_len;
    return list;
}
	//SDL_Rect** Rectangles;
/*SDL_Surface Extract(SDL_Surface *img, SDL_Surface *mask3){
	for( int y = 0; y < output->h; y++)
    {
		for( int x = 0; x < output->w; x++)
		{
			//Get pixel
           	Uint32 pixel_h = get_pixel(mask3, x, y);
           	Uint8 r, g, b;
			SDL_GetRGB(pixel, mask3->format, &r, &g, &b);
			if(r < 127 && g < 127 && b < 127){
				//Pixel is black
				//Check for rectangle
				SDL_Rect rect = CheckRectangle(img, x, y);
			}
		}
	}
	return img;

}

SDL_Rect CheckRectangle(SDL_Surface *img, x, y){
	SDL_Rect output;
	output->x = x;
	output->y = y;
	int j = y;
	int goOn = 1;
	while(goOn == 1 && j < img->h){
		//Get pixel
        Uint32 pixel = get_pixel(img, x, j);
	   	Uint8 r, g, b;
		SDL_GetRGB(pixel, img->format, &r, &g, &b);
		if(r > 127 && g > 127 && b > 127){
			//pixel is white
			goOn = 0
		}
		j++;
	}
	output->h = j
	int i = x;
	goOn = 1;
	while(goOn == 1 && i < img->w){
		//Get pixel
        Uint32 pixel = get_pixel(img, i, y);
	   	Uint8 r, g, b;
		SDL_GetRGB(pixel, img->format, &r, &g, &b);
		if(r > 127 && g > 127 && b > 127){
			//pixel is white
			goOn = 0;
		}
		i++;
	}
	output->w = i;

	return output;

}

SDL_Surface* RSLA(SDL_Surface *img){
	SDL_Surface* mask1, mask2, mask3;
	SDL_Surface *output;
	mask1 = Process(img, mask1, 6, 1);
	mask2 = Process(img, mask2, 3, 0);
	mask3 = Merge(mask1, mask2, mask3);
	//DO EXTRACT 					->*output = Extract(*img, *mask3 );
	SDL_FreeSurface(mask1);
	SDL_FreeSurface(mask2);
	SDL_FreeSurface(mask3);
	return output;
}

*/


 
