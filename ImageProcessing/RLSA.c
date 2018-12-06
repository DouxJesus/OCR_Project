//
// Created by pyrd on 16/10/18.
//

#include "RLSA.h"
#include "../Image_BMP/pixel_operations.h"
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

#define EXTRACTION_NUM_OF_PASSES 5
#define MATRIX_HEIGHT 32
#define MATRIX_WIDTH 32

//==================================================TOOLBOX=================================================================
SDL_Surface* InitSurfaceFromAnother(SDL_Surface *img, SDL_Surface *mask){
    if( img->flags & SDL_SRCCOLORKEY ) {
        mask = SDL_CreateRGBSurface( SDL_SWSURFACE, img->w, img->h, img->format->BitsPerPixel, img->format->Rmask, img->format->Gmask, img->format->Bmask, 0 ); }
    else { 
        mask = SDL_CreateRGBSurface( SDL_SWSURFACE, img->w, img->h, img->format->BitsPerPixel, img->format->Rmask, img->format->Gmask, img->format->Bmask, img->format->Amask );
    }
    return mask;
}

//Initialise the queue given in parameter, by enqueuing the first rectangle, which is the size of the image
int InitQueue(Queue* q, int x, int y, int w, int h){
    Node* pN = (Node*) malloc(sizeof (Node));
    pN->data = CreateRect(x, y, w, h);
    Enqueue(q, pN);
    AddMarker(q);
    return 1;
}

void AddMarker(Queue* q){
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

void ClearList(Rect_List* rect_list, int prints){
    int i;
    if(prints)
        printf("Freeing ...\n");
    for (i = 0; i < rect_list->length; ++i)
    {
        if(prints)
            printf("Free n°%i\n",i);
        free(rect_list->list[i]);
    }
    free(rect_list->list);
    free(rect_list);
    if(prints)
        printf("Free completed\n=========================================================\n");
}

Rect* CreateRect(int x, int y, int width, int height){
    Rect* rect = (Rect*) malloc(sizeof(Rect));
    rect->x = x;
    rect->y = y;
    rect->width = width;
    rect->height = height;
    return rect;
}

void Draw_Rect(SDL_Surface* mask, Rect rect, int color) {
    Uint32 pixel_color;
    if(color)
        pixel_color = SDL_MapRGB(mask->format, 229, 4, 60);
    else
        pixel_color = SDL_MapRGB(mask->format, 4, 56, 244);
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

void updateStepDebug(SDL_Surface* screen, SDL_Surface* image, char message[], int wait){
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

//========================================================== MASK PROCESS =====================================================================
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
    if( SDL_MUSTLOCK(mask1) ){
        SDL_LockSurface(mask1);
    }
    if( SDL_MUSTLOCK(mask2) ){
        SDL_LockSurface(mask2);
    }
    output = InitSurfaceFromAnother(mask1, output);
    if( SDL_MUSTLOCK(output) ){
        SDL_LockSurface(output);
    }
    for( int y = 0; y < mask1->h; y++){
        for( int x = 0; x < mask1->w; x++){
            Uint32 pixel_h = get_pixel(mask1, x, y);
            Uint8 rh, gh, bh;
            SDL_GetRGB(pixel_h, mask1->format, &rh, &gh, &bh);
            Uint32 pixel_v = get_pixel(mask2, x, y);
            Uint8 rv, gv, bv;
            SDL_GetRGB(pixel_v, mask2->format, &rv, &gv, &bv);
            if(rh < 127){ //on both mask, pixel is black
                put_pixel(output, x, y, pixel_v);
            } else {
                put_pixel(output, x, y, pixel_h);
            }
        }
    }
    if(SDL_MUSTLOCK(mask1)){
        SDL_UnlockSurface(mask1);
    }
    if(SDL_MUSTLOCK(mask2)){
        SDL_UnlockSurface(mask2);
    }
    if(SDL_MUSTLOCK(output) ){
        SDL_UnlockSurface(output);
    }
    return output;
}
//=========================================================== EXTRACTION ======================================================================
//Extraction subfunction, responsible for the processus of extraction
//Param:q - Queue used in the process
//Param:mask - SDL_Surface which is the support of the extraction process
//Param:rectangle - Rect * on which we apply the extraction process
//Param:horizontal - int that define if the process should do a horizontal pass or a vertical one, respectively 1 and -1
//Return 0 if process could not do better
//Return 1 if process was succesful
int ExtractionProcess(Queue* q, SDL_Surface *mask, Rect * rectangle, int horizontal){
    if(horizontal != -1 && horizontal !=1)                      //Test 
        exit(-1);
    int rect_x = rectangle->x, rect_y = rectangle->y;           //Coordinates of top-left corner of the rectangle given in parameters
    int relative_rect_width = rectangle->width;
    int relative_rect_height = rectangle->height;               //Relatives lengths of the rectangle
    int rect_width = relative_rect_width + rectangle->x;
    int rect_height = relative_rect_height + rectangle->y;      //Height & Width of the rectangle of the rectangle
    int first_x = 0, first_y = 0;
    int isWhiteLine = 1;
    int wasWhiteLine = 1;
    int succesful = 0;                                          //Define if the function was succesful or not
    if(horizontal >0){
        wasWhiteLine = 1;
        int j = 0;
        for(j = rect_y; j < rect_height; j++){
            isWhiteLine = 1; 
            int i = rect_x;
            while(isWhiteLine == 1 && i < rect_width){          //Check isWhiteLine
                Uint32 pixel = get_pixel(mask, i, j);
                Uint8 r, g, b;
                SDL_GetRGB(pixel, mask->format, &r, &g, &b);
                if(b < 127){                                    //Pixel is black
                    isWhiteLine = 0;                            //Line has a black line, is not a white line
                }
                i++;
            }
            if (isWhiteLine){                                   //if line IS a white line
                if (!wasWhiteLine){                             //End of bloc  --- Was = 1, Is = 0
                    Node *pN = (Node*) malloc(sizeof(Node));
                    pN->data = CreateRect(rect_x, first_y, relative_rect_width, j - first_y);
                    Enqueue(q, pN);
                    succesful=1;
                }
                wasWhiteLine = 1;
            } else {                                            //if line IS NOT a white line
                if(wasWhiteLine){                               //Beginning of bloc --- Was = 0, Is = 1
                    first_y = j;
                }
                wasWhiteLine = 0;
            }
        }
        if (!wasWhiteLine){                                     //End of last possible bloc
            Node *pN = (Node*) malloc(sizeof (Node));
            pN->data =  CreateRect(rect_x, first_y, relative_rect_width, rect_height - first_y);
            Enqueue(q, pN);
            succesful=1;
        }
        return succesful;
    } else {                                                    //Vertical pass
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
        if (!wasWhiteLine ) {                               //Previous line was not white, and it's the end of the image, we can do a bloc
            Node *pN = (Node*) malloc(sizeof (Node));
            pN->data = CreateRect(first_x, rect_y, rect_width - first_x, relative_rect_height);
            Enqueue(q, pN);
            succesful=1;
        }
        return succesful;
    }
    return succesful;
}
    
//Function to extract the rectangles from the images with the mask given
//Param:mask - SDL Surface which from we extract the rectangles
//Param:prints - bool for debug purposes
//Param:levelOfPrints - int to define which prints to do. Lvl 1 is basic prints, with important information, lvl 2 is slightly more complete, with much more data printed
Rect_List* Extraction(SDL_Surface* mask, int rectMode, Rect* rect, int prints, int levelOfPrints){
    Queue* q = ConstructQueue(-1);                                                  //Queue of rectangles to process
    Rect_List* output = malloc(sizeof(Rect_List));                                  //Array of rectangles ready
    output->list = 0;
    output->length = 0;
    if(rectMode == 0 && rect == NULL){
        InitQueue(q, 0, 0, mask->w, mask->h);
    } else {
        InitQueue(q, rect->x, rect->y, rect->width, rect->height);
        printf("Extrac RectMode : x%i y%i w%i h%i\n", rect->x, rect->y, rect->width, rect->height);
    }
    int horizontalPass = -1;                                                        //1 is horizontal, -1 is vertical -- Define if process do a horizontal pass or not
    int processCount = 0, switchCount = 0, AddToListCount = 0;
    int endCase = 0;
    while(switchCount <= EXTRACTION_NUM_OF_PASSES){
        endCase = 0;
        Node* pNode = Dequeue(q);
        if(pNode->data == NULL && !isEmpty(q)){                                     //Case of switch
            horizontalPass = -horizontalPass;                                       //Switch between Horizontal Passes & Vertical Passes
            AddMarker(q);
            switchCount++;
            if(prints && levelOfPrints >=2){                                        //For debug purposes
                if(horizontalPass > 0){
                    printf("Switch n°%i : to Horizontal\n",switchCount);
                } else {
                    printf("Switch n°%i : to Vertical\n",switchCount);
                }
            }   
        } else {     
            Rect* tmp = pNode->data;
            if(prints && levelOfPrints >=2){
                printf("Node : H%i W%i - x%iy%i - ", tmp->height, tmp->width, tmp->x, tmp->y);
                printf("Process n°%i \n", processCount);
            }
            if(prints && levelOfPrints >=2)
               printf("Still in queue : %i - ", q->size);
            endCase = ExtractionProcess(q, mask, tmp, horizontalPass);
            processCount++;
            if(switchCount == EXTRACTION_NUM_OF_PASSES){  
                AddToList(output, tmp);
                AddToListCount++;
                if(prints && levelOfPrints >=2)
                    printf(" - AddToList n°%i\n",AddToListCount);   
            }
        }
    }
    DestructQueue(q);                                                   //Extraction is finished, free queue
    // if(rectMode){
    //     free(rect);
    // }  
    if(prints && levelOfPrints >=1)                                     //For debug purposes
        printf("Extraction completed - Bloc extracted : %i - Number of switch : %i\n", AddToListCount, switchCount);
    return output;
}

//=====================================RESIZE========================================
double getByte(int value, int n){
    return (value >> (n*8) & 0xFF);
} 

double lerp(double s, double e, double t){
    return s + (e - s) * t;
}

double blerp(double c00, double c10, double c01, double c11, double tx, double ty){
    double a = lerp(c00, c10, tx);
    double b = lerp(c01, c11, tx);
    return lerp(a, b, ty);
}

SDL_Surface* resizeImage(SDL_Surface* src, SDL_Surface* output, int r_width, int r_height){    
    int srcWidth = src->w;
    int srcHeight = src->h;
    double scalex = r_width / srcWidth;
    double scaley = r_height / srcHeight;
    int newWidth = r_width;
    int newHeight= r_height;
    output = InitSurfaceFromAnother(src, output);
    int x, y;
    for(x= 0, y=0; y < newHeight; x++){
        if(x > newWidth){
            x = 0;
            y++;
        }
        double gx = x / (double)(newWidth) * (src->w-1);
        double gy = y / (double)(newHeight) * (src->h-1);
        int gxi = (int)gx;
        int gyi = (int)gy;
        Uint32 result = 0;
        Uint32 c00 = get_pixel(src, gxi, gyi);
        Uint32 c10 = get_pixel(src, gxi+1, gyi);
        Uint32 c01 = get_pixel(src, gxi, gyi+1);
        Uint32 c11 = get_pixel(src, gxi+1, gyi+1);
        Uint8  i;
        for(i = 0; i < 3; i++){
            //((uint8_t*)&result)[i] = blerp( ((uint8_t*)&c00)[i], ((uint8_t*)&c10)[i], ((uint8_t*)&c01)[i], ((uint8_t*)&c11)[i], gxi - gx, gyi - gy); // this is shady
            result |= (Uint8)blerp(getByte(c00, i), getByte(c10, i), getByte(c01, i), getByte(c11, i), gx - gxi, gy -gyi) << (8*i);
        }
        put_pixel(output,x,y, result);
    }
    return output;
}

void swap(double *a, double *b){
    double tmp = *a;
    *a = *b;
    *b = tmp;
}





//==============================================RLSA Main Function===========================================
void DisplayRLSA(Rect_List* rect_list, SDL_Surface* image, int prints){
    int i = 0;
    Rect tmp;
    printf("=====================================================\n");
    printf("DISPLAY RLSA : Length to draw - %i\n",rect_list->length );
    while(i < rect_list->length){
        //printf("PreDraw : %i\n",i );
        tmp = *rect_list->list[i];
        if(prints){
            printf("Draw_Rect n°%i : H%i W%i - x%iy%i\n",i,tmp.height, tmp.width, tmp.x, tmp.y);
        }
        if((tmp.height >= 0 && tmp.width >=0 && tmp.x >= 0 && tmp.y >= 0) && (tmp.height <= image->h && tmp.width <= image->w && tmp.x <= image->w && tmp.y <= image->h)){
            Draw_Rect(image, tmp, i % 2);
        }else {
            printf("Rect n°%i is NOT valid\n",i);
        }
        //printf("LastDraw : %i\n",i );
        i++;
    }      
}

Matrix_Rect_List* Matrixize(SDL_Surface* image, Rect_List* rect_list){
    Matrix_Rect_List* output = malloc(sizeof (Matrix_Rect_List));
    output->list_matrix = (Matrix_Rect** ) malloc(rect_list->length * sizeof (Matrix_Rect*));
    output->length = 0;
    for (int i = 0; i < rect_list->length; i++){
        Matrix_Rect* matrix_tmp = malloc(sizeof (Matrix_Rect));

        Rect* tmp = rect_list->list[i];
        int r_x = tmp->x;
        int r_y = tmp->y;
        int r_w = tmp->width;
        int r_h = tmp->height;
        free(tmp);
        matrix_tmp->matrix = calloc(((r_w * r_h)) , sizeof(double));
        matrix_tmp->height = r_h;
        matrix_tmp->width = r_w;
        matrix_tmp->x = r_x;
        matrix_tmp->y = r_y; 
        for(int x = r_x, m_x = 0; x < r_x + r_w; x++, m_x++){
            for(int y = r_y, m_y = 0; y < r_y + r_h; y++, m_y++){
                //printf("x%i y%i\n",x,y );
                double pix = 0;
                Uint32 pixel = get_pixel(image, x, y);
                Uint8 r, g, b;
                SDL_GetRGB(pixel, image->format, &r, &g, &b);
                if(b < 127){                                //pix == 1 -> pixel is black on the image
                    pix = 1;
                }
                matrix_tmp->matrix[m_x * r_h + m_y] = pix;
            }
        }
        printf("Mat n°%i : w%i h%i - x%i y%i \n",i,matrix_tmp->width, matrix_tmp->height,matrix_tmp->x, matrix_tmp->y);
        output->list_matrix[i] = matrix_tmp;
        output->length++;
    }
    printf("Matrixize - len_rect : %i - len_matrix : %i", rect_list->length, output->length);
    //free(rect_list);
    return output;
}


//Resize the given matrix at the given width and height, by applying a billineral interpolation
Matrix_Rect* resizeMatrix(Matrix_Rect* src, int r_width, int r_height){    
    int srcWidth = src->width;
    int srcHeight = src->height;
    double* matrix = src->matrix;
    //double scalex = r_width / srcWidth;
    //double scaley = r_height / srcHeight;
    int newWidth = r_width;
    int newHeight= r_height;
    int x, y;
    double* new_matrix = calloc(((newWidth * newHeight)) , sizeof(double)); 
    for(x= 0;x < newWidth; x++){
        for (y = 0; y < newHeight; y++){
            double gx = x / (double)(newWidth) * (src->width-1);
            double gy = y / (double)(newHeight) * (src->height-1);
            int gxi = (int)gx;
            int gyi = (int)gy;
            Uint32 result = 0;
            double pix00 = (matrix[gxi * srcHeight + gyi] > 0) ?1 : 0;
            double pix01 = (matrix[gxi+1 * srcHeight + gyi] > 0) ? 1 : 0;
            double pix10 = (matrix[gxi * srcHeight + gyi + 1] > 0) ? 1 : 0;
            double pix11 = (matrix[gxi+1 * srcHeight + gyi + 1] > 0) ? 1 : 0;
            Uint8  i;
            for(i = 0; i < 3; i++){
                result |= (Uint8)blerp(pix00 , pix10 , pix01 , pix11 , gx - gxi, gy -gyi) << (8*i);
            }
            new_matrix[x * newHeight + y] = result;
        }
    }
    swap(src->matrix, new_matrix);
    free(new_matrix);
    return src;
}

Matrix_Rect* Matrixize2(SDL_Surface* image, Rect* rect){
    int r_x = rect->x;
    int r_y = rect->y;
    int r_w = rect->width;
    int r_h = rect->height;
    //free(rect);
    Matrix_Rect* output = (Matrix_Rect*) malloc(sizeof (Matrix_Rect));
    output->matrix = (double*) malloc(((r_w * r_h)) * sizeof(double));
    output->height = r_h;
    output->width = r_w;
    output->x = r_x;
    output->y = r_y; 
    int len = 0;
    int i = 0;
    double pix = 0;
    int x, y;
    int m_x, m_y;
    for(y = r_y, m_y = 0; y < (r_y + r_h); y++, m_y++){
        for(x = r_x, m_x = 0; x < (r_x + r_w); x++, m_x++){

            pix = 0;
            Uint32 pixel = get_pixel(image, x, y);
            Uint8 r, g, b;
            SDL_GetRGB(pixel, image->format, &r, &g, &b);
            if(b < 127 && g < 127 && r < 127){                                //pix == 1 -> pixel is black on the image
                pix = 1;
                // printf("Hey im black x%i y%i\n",x,y );
            }
            //output->matrix[m_x * r_h + m_y] = pix;
            output->matrix[m_y * r_w + m_x] = pix;
            //printf("pix %f\n",output->matrix[m_y * r_w + m_x]);
            len++;
        }
    }    
    printf("Matrixize - len_rect : %i - len_matrix : %i\n", (rect->width)*(rect->height), len);
    return output;
}

double* resizeMatrix3(Matrix_Rect* src, int r_width, int r_height){ 
    int srcWidth = src->width;
    int srcHeight = src->height;
    double* matrix = src->matrix;
    //double scalex = r_width / srcWidth;
    //double scaley = r_height / srcHeight;
    int newWidth = r_width;
    int newHeight= r_height;
    int x, y;
    double* new_matrix = calloc(((newWidth * newHeight)) , sizeof(double));
    for(x= 0, y=0; y < newHeight; x++){
        if(x > newWidth){
            x = 0;
            y++;
        }
        double gx = x / (double)(newWidth) * (src->w-1);
        double gy = y / (double)(newHeight) * (src->h-1);
        int gxi = (int)gx;
        int gyi = (int)gy;
        Uint32 result = 0;   //white
        Uint32 c00 = (matrix[gxi * srcHeight + gyi] > 0) ? 1 : 0;
        Uint32 c10 = get_pixel(src, gxi+1, gyi);
        Uint32 c01 = get_pixel(src, gxi, gyi+1);
        Uint32 c11 = get_pixel(src, gxi+1, gyi+1);
        double pix00 = (matrix[gxi * srcHeight + gyi] > 0) ? 1 : 0;
        double pix01 = (matrix[gxi+1 * srcHeight + gyi] > 0) ? 1 : 0;
        double pix10 = (matrix[gxi * srcHeight + gyi + 1] > 0) ? 1 : 0;
        double pix11 = (matrix[gxi+1 * srcHeight + gyi + 1] > 0) ? 1 : 0;
        Uint8  i;
        for(i = 0; i < 3; i++){
            //((uint8_t*)&result)[i] = blerp( ((uint8_t*)&c00)[i], ((uint8_t*)&c10)[i], ((uint8_t*)&c01)[i], ((uint8_t*)&c11)[i], gxi - gx, gyi - gy); // this is shady
            result |= (Uint8)blerp(getByte(c00, i), getByte(c10, i), getByte(c01, i), getByte(c11, i), gx - gxi, gy -gyi) << (8*i);
        }
        new_matrix[x * newHeight + y] = result;
    }
    return output;
}


//Resize the given matrix at the given width and height, by applying a billineral interpolation
double* resizeMatrix2(Matrix_Rect* src, int r_width, int r_height){    
    int srcWidth = src->width;
    int srcHeight = src->height;
    double* matrix = src->matrix;
    //double scalex = r_width / srcWidth;
    //double scaley = r_height / srcHeight;
    int newWidth = r_width;
    int newHeight= r_height;
    int x, y;
    double* new_matrix = calloc(((newWidth * newHeight)) , sizeof(double));
    for (y = 0; y < newHeight; y++){ 
        for(x= 0;x < newWidth; x++){
            double gx = x / (double)(newWidth) * (src->width-1);
            double gy = y / (double)(newHeight) * (src->height-1);
            int gxi = (int)gx;
            int gyi = (int)gy;
            Uint32 result = 0;
            double pix00 = (matrix[gxi * srcHeight + gyi] > 0) ? 1 : 0;
            double pix01 = (matrix[gxi+1 * srcHeight + gyi] > 0) ? 1 : 0;
            double pix10 = (matrix[gxi * srcHeight + gyi + 1] > 0) ? 1 : 0;
            double pix11 = (matrix[gxi+1 * srcHeight + gyi + 1] > 0) ? 1 : 0;
            Uint8  i;
            for(i = 0; i < 3; i++){
                result |= (Uint8)blerp(pix00 , pix10 , pix01 , pix11 , gx - gxi, gy -gyi) << (8*i);
            }
            new_matrix[x * newHeight + y] = result;
        }
    }
    swap(src->matrix, new_matrix);
    //free(new_matrix);
    return src;
}

void print_matrix(char s[], double m[], size_t rows, size_t cols)
{
    printf("%s = %li X %li\n",s, rows, cols);
    for (size_t j = 0; j < cols; ++j){
        for (size_t i = 0; i < rows; ++i)
        {
            printf("%.f",m[j * rows + i] );
        }
        printf("\n");
    }
}

void AddToListWord(Word_List* word_list, Word* item){
    // printf("AddToWordList : %c\n",item->list_of_letters[1]->letter);
    if(word_list->list == 0)
    {
        word_list->list = malloc(sizeof(Word*));
        word_list->length = 1;
        word_list->list[0] = item;
    } else if (!(word_list->list = realloc(word_list->list, (word_list->length + 1) * sizeof(Word*)))) {
        exit(-1);
    } else {
        word_list->length++;
        word_list->list[word_list->length-1] = item;
    }
}

void AddToListLetters(Word* word, Letter* item){
    // printf("AddToWord : %c\n",item->letter);
    if(word->list_of_letters == 0){
        word->list_of_letters = malloc(sizeof(Letter*));
        word->length = 1;
        word->list_of_letters[0] = item;
    } else if (!(word->list_of_letters  = realloc(word->list_of_letters, (word->length + 1) * sizeof(Letter*)))) {
        exit(-1);
    } else {
        word->length++;
        word->list_of_letters[word->length-1] = item;
    }
}

//Return the Word_List, containing Word, themself containing Letters and themself the matrixes
Word_List* Wordify(SDL_Surface* image, Rect_List* rect_list, SDL_Surface * screen){
    Word_List* word_list = (Word_List *) malloc(sizeof (Word_List));
    word_list->list = 0;
    word_list->length = 0;
    int i = 0;
    while(i < rect_list->length){
        //printf("Rect n°%i \n",i);
        Rect* tmp = rect_list->list[i];
        if(tmp == NULL){
            exit(-1);
        }
        Rect_List* letters_rect = Extraction(image, 1, tmp, 1, 1);
        DisplayRLSA(letters_rect, image, 1);
        updateStepDebug(screen, image, "Wordify", 1);
        Word* word_tmp = (Word*) malloc(sizeof (Word));
        word_tmp->list_of_letters = 0;
        word_tmp->x = tmp->x;
        word_tmp->y = tmp->y;
        word_tmp->length = 0;
        //=============================
        for(int i = 0; i < letters_rect->length; i++){
            Letter* letter_tmp = (Letter*) malloc(sizeof(Letter));
            letter_tmp->matrix = 0;
            letter_tmp->letter = 0;
            Matrix_Rect* matrix_tmp = Matrixize2(image, letters_rect->list[i]);
            printf("==================Matrixize===============\n");
            print_matrix("Matrix1",matrix_tmp->matrix, letters_rect->list[i]->width, letters_rect->list[i]->height );
            letter_tmp->matrix = resizeMatrix2(matrix_tmp, MATRIX_WIDTH, MATRIX_HEIGHT);
            printf("=========================================\n");
            print_matrix("Matrix2",letter_tmp->matrix, MATRIX_WIDTH, MATRIX_HEIGHT );
            letter_tmp->letter = 'A';
            AddToListLetters(word_tmp, letter_tmp);
        }

        //=============================
        AddToListWord(word_list, word_tmp);
        i++;
    }
    return word_list;
}


//Main function of RLSA
//Param:image - SDL_Surface on which to apply the RLSA Algorithme
//Param:size - int to define the size of the output matrices
//Return a list of double
Word_List* RLSA(SDL_Surface* image, SDL_Surface* screen, int size){
    SDL_Surface* image_mask1 = NULL;                                    //Init of the necessary SDL_Surfaces
    SDL_Surface* image_mask2 = NULL;
    SDL_Surface* image_mask3 = NULL;
    SDL_Surface* image_mask4 = NULL;
    
    image_mask1 = Process(image, image_mask1, 100, 1);            //Horizontal Mask
    image_mask2 = Process(image, image_mask2, 2, 0);            //Vertical Mask
    image_mask3 = Merge(image_mask1, image_mask2, image_mask3);         //Merge of mask 1 & 2
    image_mask4 = Process(image_mask3, image_mask4, 10, 1);             //Last pass of horizontal pass
    SDL_FreeSurface(image_mask1);                                       //Free of used masks
    SDL_FreeSurface(image_mask2);
    updateStepDebug(screen, image_mask4, "RLSA -- Mask", 1);

    SDL_FreeSurface(image_mask3);
    Rect_List* word_rect_list = Extraction(image_mask4, 0, NULL, 0, 0);          //Extraction of WORDS through mask4
    //DisplayRLSA(word_rect_list, image, 1);
    updateStepDebug(screen, image, "RLSA -- Final", 1);
    //=============
    // for (int i = 0; i < word_rect_list->length; ++i)
    // {
    //     image_mask4 = resizeImage3(image, word_rect_list->list[i], image_mask4, 300, 300);
    //     updateStepDebug(screen, image_mask4, "resizedImage", 1);
    // }
    //=============
    SDL_FreeSurface(image_mask4);
    Word_List* RLSA_WORD_LIST = Wordify(image, word_rect_list, screen);
    // printf("Wordify: %i words in total\n",RLSA_WORD_LIST ->length);
    for (int i = 0; i < RLSA_WORD_LIST ->length; ++i){
        int len = RLSA_WORD_LIST->list[i]->length;
        // printf("word n°%i has %i letters\n", i, len);
    }
    return RLSA_WORD_LIST;
}
