#include <stdio.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "ImageProcessing/BlackAndWhite.h"
#include "Image_BMP/pixel_operations.h"
#include "Image_BMP/BMP.h"
#include "RSLA.h"

int main() {
    printf("Hello, OCR Project!\n");

    SDL_Surface* image_surface;
    SDL_Surface* screen_surface;

    init_sdl();

    image_surface = load_image("Images_test/rafale.bmp");
    screen_surface = display_image(image_surface);

    wait_for_keypressed();
    /////////////////////////////////////////////////
    printf("to_grayscale\n");
    to_grayscale(image_surface);

    update_surface(screen_surface, image_surface);

    wait_for_keypressed();
    /////////////////////////////////////////////////
    printf("B&W\n");
    BlackAndWhite(image_surface);

    update_surface(screen_surface, image_surface);

    wait_for_keypressed();


    /////////////////////////////////////////////////
    printf("RSLA : horizontal\n");
    SDL_Surface* image_mask1;
    image_mask1 = Process(image_surface, 10, 1);
    update_surface(screen_surface, image_mask1);
    wait_for_keypressed();

    /////////////////////////////////////////////////
    printf("RSLA : vertical\n");
    SDL_Surface* image_mask2;
    image_mask2 = Process(image_surface, 15, 0);
    update_surface(screen_surface, image_mask2);
    wait_for_keypressed();


    /////////////////////////////////////////////////
    printf("RSLA : merge\n");
    SDL_Surface* image_mask3;
    image_mask3 = NULL;
    image_mask3 = Merge(image_mask1, image_mask2, image_mask3);
    update_surface(screen_surface, image_mask3);
    wait_for_keypressed();

    /////////////////////////////////////////////////
    printf("End\n");
    SDL_FreeSurface(image_surface);
    SDL_FreeSurface(screen_surface);
    return 0;
}