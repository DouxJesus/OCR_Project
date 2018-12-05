#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "ImageProcessing/BlackAndWhite.h"
#include "Image_BMP/pixel_operations.h"
#include "Image_BMP/BMP.h"
#include "ImageProcessing/RLSA.h"
#include "NeuralNetwork/NeuralCreate.h"
#include "Restore/restore.h"
#include "Interface/interface.c"

void updateStep(SDL_Surface* screen, SDL_Surface* image, char message[], int wait){
    update_surface(screen, image);
    printf(" ==== %s ====\n",message);
    printf("Press a key to continue ...\n");
    printf(" ====");
    size_t len = 28;
    size_t len2 = strlen(message);
    if(len2 > len){
        len = len2; 
    }
    for(size_t i = 0; i < len; ++i){
        printf("=");
    }
    printf("====\n");
    if(wait == 1){
        wait_for_keypressed();
    }
}

int main(int argc, char** argv) {
    printf("Hello, OCR Project!\n");
	if (argc > 2){
		printf("you put a lot of parameters !");
    }
    char* param = argv[1];

    printf("param = %s \n", argv[1]);
    if (param){    
        SDL_Surface* screen_surface;
        SDL_Surface* image_surface;

        init_sdl();
        image_surface = load_image(param);
        screen_surface = display_image(image_surface);
        printf("SDL Initiated \n");
        wait_for_keypressed();
    // ============== Grayscale ===========================
        to_grayscale(image_surface);
        updateStep(screen_surface, image_surface, "Grayscaling", 1);
        //printf("%i %i \n", maxi, mini);

    // ============ Black & White ===========================
        BlackAndWhite(image_surface);
        updateStep(screen_surface, image_surface, "Black and White", 1);


    // ================= RSLA =======================
    // ==============================================

        Word_List* RLSA_Output = RLSA(image_surface, screen_surface, 1);
        String* text = stringyfy(RLSA_Output);
        printf("==============================================================\n");
        printf("==============================================================\n");
        printf("==============================================================\n");
        printf("%s\n",text->string );
        SDL_FreeSurface(image_surface);
        SDL_FreeSurface(screen_surface);
        printf("Quitting ...\n");
    } else {
       // printf("You need to call ./main with an argument \n");
        //exit(1);

    /* int* layers;
        int laylen = 10;
        if(!(layers = malloc(laylen * sizeof(int))))
         {
             exit(-1);
        }
     layers[0] = 4;
     layers[1] = 5;
     layers[2] = 3;
     layers[3] = 5;
     layers[4] = 2;
     layers[5] = 2;
     layers[6] = 2;
     layers[7] = 2;
     layers[8] = 2;
     layers[9] = 2;

     //Network net = CreateNetwork(layers, laylen);
     //SaveNetwork(net);
     //FreeNetwork(net);
    Network net = LoadNetwork();
    SaveNetwork(net);*/
    gtk_init(&argc, &argv);
    GtkWidget * MainWindow = NULL;
    MainWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_widget_show(MainWindow);

}
    return 0;
}
