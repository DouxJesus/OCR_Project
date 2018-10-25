#include <stdio.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "ImageProcessing/BlackAndWhite.h"
#include "Image_BMP/pixel_operations.h"
#include "Image_BMP/BMP.h"
#include "ImageProcessing/RSLA.h"

int main(int argc, char** argv) {
    printf("Hello, OCR Project!\n");
	if (argc > 2)
	printf("you put a lot of parameters !");
    char* param = argv[1];

    printf("param = %s \n", argv[1]);
    SDL_Surface* image_surface;
    SDL_Surface* screen_surface;

    init_sdl();

    image_surface = load_image(param);
    screen_surface = display_image(image_surface);

    wait_for_keypressed();
    /////////////////////////////////////////////////
    printf("to_grayscale\n");
    to_grayscale(image_surface);

    update_surface(screen_surface, image_surface);
    printf("%i %i \n", maxi, mini);
    wait_for_keypressed();


    /////////////////////////////////////////////////

   // printf("Contrast\n");
    //Contrast(image_surface);

   // update_surface(screen_surface, image_surface);

    //wait_for_keypressed();
    /////////////////////////////////////////////////

    printf("B&W\n");
    BlackAndWhite(image_surface);

    update_surface(screen_surface, image_surface);

    wait_for_keypressed();


    /////////////////////////////////////////////////
    printf("RSLA : horizontal\n");
    SDL_Surface* image_mask1 = NULL;
    image_mask1 = Process(image_surface, image_mask1, 200, 1);
    update_surface(screen_surface, image_mask1);
    wait_for_keypressed();


    /////////////////////////////////////////////////
    printf("RSLA : vertical\n");
    SDL_Surface* image_mask2 = NULL;
    image_mask2 = Process(image_surface, image_mask2, 200, 0);
    update_surface(screen_surface, image_mask2);
    wait_for_keypressed();


    /////////////////////////////////////////////////
    SDL_Surface* image_mask3 = NULL;
    image_mask3 = Merge(image_mask1, image_mask2, image_mask3);
    update_surface(screen_surface, image_mask3);
    printf("RSLA : merge\n");
    wait_for_keypressed();


 /////////////////////////////////////////////////
    printf("RSLA : horizontal 2\n");
    SDL_Surface* image_mask31 = NULL;
    image_mask31 = Process(image_mask3, image_mask31, 10, 1);
    update_surface(screen_surface, image_mask31);
    wait_for_keypressed();

    /////////////////////////////////////////////////
    printf("RSLA :  CreateListRect \n");
    Rect_List list_rect = Create_Rect_List(image_mask31);
    printf("RSLA :  GetRect \n");

    Rect_Mask_Couple result = Get_Rect(image_mask31, list_rect, 0, 0, 4);
    update_surface(screen_surface, result.mask);
    wait_for_keypressed();
    update_surface(screen_surface, image_surface);
    wait_for_keypressed();
    Uint32 pixel_rouge = SDL_MapRGB(image_surface->format, 255, 0, 0);
    for (int i = 0; i < 100; ++i) {
        image_surface = Draw_Rect(image_surface, result.rect_list.list[i], pixel_rouge);
    }
    update_surface(screen_surface, image_surface);
    wait_for_keypressed();
    /*       /////////////////////////////////////////////////
     printf("RSLA : vertical 2\n");
     SDL_Surface* image_mask32 = NULL;
     image_mask32 = Process(image_mask3, image_mask32, 5, 0);
     update_surface(screen_surface, image_mask32);
     wait_for_keypressed();

  /////////////////////////////////////////////////
     SDL_Surface* image_mask33 = NULL;
     image_mask33 = Merge(image_mask31, image_mask32, image_mask33);
     update_surface(screen_surface, image_mask33);
     printf("RSLA : merge\n");
     wait_for_keypressed();*/

    /////////////////////////////////////////////////
    printf("End\n");
    SDL_FreeSurface(image_mask1);
    SDL_FreeSurface(image_mask2);
    SDL_FreeSurface(image_mask3);

    SDL_FreeSurface(image_mask31);
    SDL_FreeSurface(image_surface);
    SDL_FreeSurface(screen_surface);
    return 0;
}
