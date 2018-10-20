#include <stdio.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "ImageProcessing/BlackAndWhite.h"
#include "Image_BMP/pixel_operations.h"
#include "Image_BMP/BMP.h"

int main() {
    printf("Hello, OCR Project!\n");

    SDL_Surface* image_surface;
    SDL_Surface* screen_surface;

    init_sdl();

    image_surface = load_image("Images_test/RTEmagicC_texte-ponctuation_02.bmp");
    screen_surface = display_image(image_surface);

    wait_for_keypressed();
    /////////////////////////////////////////////////
    to_grayscale(image_surface);

    update_surface(screen_surface, image_surface);

    wait_for_keypressed();
    /////////////////////////////////////////////////
    gammaCorrection(image_surface);

    update_surface(screen_surface, image_surface);

    wait_for_keypressed();
    /////////////////////////////////////////////////
    BlackAndWhite(image_surface);

    update_surface(screen_surface, image_surface);

    wait_for_keypressed();

    /////////////////////////////////////////////////
   image_surface = RSLA(image_surface);

    update_surface(screen_surface, image_surface);

    wait_for_keypressed();

    /////////////////////////////////////////////////
    SDL_FreeSurface(image_surface);
    SDL_FreeSurface(screen_surface);
    return 0;
}