//
// Created by pyrd on 16/10/18.
//

#include "RSLA.h"
#include "../Image_BMP/pixel_operations.h"
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

SDL_Surface* RSLA(SDL_Surface *img){
	SDL_Surface mask1, mask2, mask3, output;
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
	output.h = height;
	output.w = width;


	//If the surface must be locked
    if( SDL_MUSTLOCK( surface ) )
    {
        //Lock the surface
        SDL_LockSurface( surface );
    }


	if(horizontal){
		//DO HORIZONTAL PASS
    	//Go through rows
        for( int y = 0; y < output->h; y++)
        {
        	//Go through columns
			for( int x = 0; x < output->w; x++ )
   			{
        		//Get pixel
            	Uint32 pixel = get_pixel(img, x, y);
            	Uint8 r, g, b;
				SDL_GetRGB(pixel, img->format, &r, &g, &b);
            	if(r < 127 && g < 127 && b < 127){
            		//Pixel is black
            		put_pixel(output, x, y, pixel)
            	}
            	else{
            		//Pixel is White
            		//Check if there is less or equal than threshold adjacent white pixels
            		int left_adjacent_pix = 0;
            		int index = x - 1;
            		int goOn = 1;
            		//Check on left side adjacent pix
            		while(index >= 0 && goOn == 1){
            			if(left_adjacent_pix > threshold){
            				//TOOMUCH -> stop
            				left_adjacent_pix = -1;
            				goOn = 0;
            			}
            			Uint32 c_pixel = get_pixel(img, index, y);
            			Uint8 cr,cg, cb;
						SDL_GetRGB(c_pixel, img->format, &cr, &cg, &cb);
            			if(cr > 127 && cg > 127 && cb > 127){
            				//Pixel is white
            				left_adjacent_pix++;
            			}
            			else
            			{
            				//Pixel is black -> stop
            				goOn = 0;
            			}
            			index--;
            		}

            		index = x + 1;
            		goOn = 1;
            		int right_adjacent_pix = 0;
            		while(index < width - 1 && goOn == 1){
            			if(right_adjacent_pix > threshold){
            				//TOOMUCH -> stop
            				left_adjacent_pix = -1;
            				goOn = 0;
            			}
            			Uint32 c_pixel = get_pixel(img, index, y);
            			Uint8 cr,cg, cb;
						SDL_GetRGB(c_pixel, img->format, &cr, &cg, &cb);
            			if(cr > 127 && cg > 127 && cb > 127){
            				//Pixel is white
            				right_adjacent_pix++;
            			}
            			else
            			{
            				//Pixel is black -> stop
            				goOn = 0;
            			}
            			index--;
            		}

            	}
   			}	
       	}
	}
	else {
		//NOT FINISHED
		//DO VERTICAL PASS

	}

	//Unlock surface
    if( SDL_MUSTLOCK( surface ) )
    {
        SDL_UnlockSurface( surface );
    }

	return output;

}

SDL_Surface* InitSurfaceFromAnother(SDL_Surface *img, SDL_Surface *mask){
	if( img->flags & SDL_SRCCOLORKEY ) { 
		mask = SDL_CreateRGBSurface( SDL_SWSURFACE, img->w, img->h, img->format->BitsPerPixel, img->format->Rmask, surfaceimg>format->Gmask, img->format->Bmask, 0 ); } 
	else { 
		mask = SDL_CreateRGBSurface( SDL_SWSURFACE, img->w, img->h, img->format->BitsPerPixel, img->format->Rmask, img->format->Gmask, img->format->Bmask, img->format->Amask ); 
	}
}

 