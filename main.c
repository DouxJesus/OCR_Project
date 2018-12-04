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

void DisplayRLSA(Rect_List* rect_list, SDL_Surface* image){
    int i = 0;
    while(i < rect_list->length){
        Draw_Rect(image, *rect_list->list[i], i % 2);
        i++;
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
        updateStep(screen_surface, image_surface, "Grayscaling", 0);
        //printf("%i %i \n", maxi, mini);

    // ============ Black & White ===========================
        BlackAndWhite(image_surface);
        updateStep(screen_surface, image_surface, "Black and White", 0);


    // ================= RSLA =======================
    // ==============================================
        SDL_Surface* image_mask1 = NULL;
        SDL_Surface* image_mask2 = NULL;
        SDL_Surface* image_mask3 = NULL;
        SDL_Surface* image_mask4 = NULL;

        image_mask1 = Process(image_surface, image_mask1, 200, 1);
        image_mask2 = Process(image_surface, image_mask2, 200, 0);
        image_mask3 = Merge(image_mask1, image_mask2, image_mask3);
        image_mask4 = Process(image_mask3, image_mask4, 10, 1);
        SDL_FreeSurface(image_mask1);
        SDL_FreeSurface(image_mask2);
        SDL_FreeSurface(image_mask3);

        updateStep(screen_surface, image_mask4, "RLSA -- Final Mask", 1);

        Rect_List* RLSA_Output = Extraction(image_mask4);
        DisplayRLSA(RLSA_Output, image_surface);
        ClearList(RLSA_Output);
        updateStep(screen_surface, image_surface, "RLSA -- Display", 1);
        printf("Quitting ...\n");
        SDL_FreeSurface(image_mask4);
        SDL_FreeSurface(image_surface);
        SDL_FreeSurface(screen_surface);
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
