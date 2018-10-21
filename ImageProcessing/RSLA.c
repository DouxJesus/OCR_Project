//
// Created by pyrd on 16/10/18.
//

#include "RSLA.h"
#include "../Image_BMP/pixel_operations.h"
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

SDL_Surface* RSLA(SDL_Surface *img){
	SDL_Surface* mask1, mask2, mask3;
	SDL_Surface *output;
	//CALL PROCESS ON horizontal    ->Mask1
	//CALL PROCESS ON VERTICAL 		->Mask2
	//DO MERGE OF MASKS				->*Mask3 = *Mask1 && *Mask2
	//DO EXTRACT 					->*output = Extract(*img, *mask3 );
	//FREE MASKS
	return output;
}
//Process method of RSLA
//img is a pointer to SDL_Surface to process
//threshold -> int -> the threshold value used in the RSLA algorithm 
//horizontal -> bool -> define if do horizontal pass (true) or vertical pass(false)
SDL_Surface* Process(SDL_Surface *img, int threshold ,int horizontal){
	SDL_Surface *output = NULL;
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
    if( SDL_MUSTLOCK(img) )
    {
        SDL_UnlockSurface(img);
    }

	return output;

}

SDL_Surface* InitSurfaceFromAnother(SDL_Surface *img, SDL_Surface *mask){
	if( img->flags & SDL_SRCCOLORKEY ) { 
		mask = SDL_CreateRGBSurface( SDL_SWSURFACE, img->w, img->h, img->format->BitsPerPixel, img->format->Rmask, img->format->Gmask, img->format->Bmask, 0 ); }
	else { 
		mask = SDL_CreateRGBSurface( SDL_SWSURFACE, img->w, img->h, img->format->BitsPerPixel, img->format->Rmask, img->format->Gmask, img->format->Bmask, img->format->Amask ); 
	}
	return mask;
}

 