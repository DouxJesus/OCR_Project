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
reSDL_Surface Process(SDL_Surface *img, SDL_Surface output, int threshold ,int horizontal){
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
					put_pixel(output, x, y, 255);
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
					put_pixel(output, x, y, 255);

            		adjacent_pix++;
            	}
   			}	
       	}

	}

	//Unlock surface
    if( SDL_MUSTLOCK(img) )
    {
        SDL_UnlockSurface(img);
    }
    return output;
}

SDL_Surface Merge(SDL_Surface *mask1, SDL_Surface *mask2, SDL_Surface *output){
	output = InitSurfaceFromAnother(mask1, output);
	for( int y = 0; y < output->h; y++)
     {
		for( int x = 0; x < output->w; x++)
		{
			//Get pixel
           	Uint32 pixel_h = get_pixel(mask1, x, y);
           	Uint8 rh, gh, bh;
			SDL_GetRGB(pixel, mask1->format, &rh, &gh, &bh);
			Uint32 pixel_v = get_pixel(mask2, x, y);
           	Uint8 rv, gv, bv;
			SDL_GetRGB(pixel, mask1->format, &rv, &gv, &bv);
			if(rh < 127 && gh < 127 && bh < 127 && rv < 127 && gv < 127 && bv < 127){
				//on both mask, pixel is black
				put_pixel(output, x, y, pixel_v)
       		}
		}
	}
	return output;
}

SDL_Surface* RSLA(SDL_Surface *img){
	SDL_Surface* mask1, mask2, mask3;
	SDL_Surface *output;
	mask1 = Process(img, mask1, 6, 1);
	mask2 = Process(img, mask2, 3, 0);
	mask3 = Merge(mask1, mask2, mask3);
	//DO EXTRACT 					->*output = Extract(*img, *mask3 );
	//FREE MASKS
	SDL_FreeSurface(mask1);
	SDL_FreeSurface(mask2);
	SDL_FreeSurface(mask3);
	return output;
}




 