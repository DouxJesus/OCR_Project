
//==============================MARINE'S STUFF=============================================================================

// //Init_Rect_List
// //initialisation of the List
// //an containing as a fisrt item the whole mask
// Rect_List Init_Rect_List(SDL_Surface* mask)
// {
//     int lenght = 1;
//     Rect* array;
//     if(!(array = malloc(sizeof(Rect))))
//     {
//         exit(-1);
//     }
//     Rect first_mask;
//     first_mask.x = 0;
//     first_mask.y = 0;
//     first_mask.width = mask->w;
//     first_mask.height = mask->h;
//     array[0]= first_mask;

//     Rect_List res;
//     res.lenght = lenght;
//     res.list = array;

//     return res;
// }

//Get_Rect
//took a mask, a list of rectangles, the point from when we begin the loop and a bool : is it an horizontal pass or not
//return the list of blocs of text from the mask
Rect_Mask_Couple Get_Rect(SDL_Surface* mask, Rect_List list, int begin,
        int IsHorizontalPass, int iteration) {

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

    for (int k = begin; k < init_lenght; ++k) { //go through the list of rect

        Rect rec_mask = array[k];

        int rect_x = rec_mask.x, rect_y = rec_mask.y; //coordinate of point up-left of the rectangle
        int relativ_rect_width = rec_mask.width;
        int relativ_rect_height = rec_mask.height; // relative lenght of the rectangle
        int rect_width = relativ_rect_width + rec_mask.x;
        int rect_height = relativ_rect_height + rec_mask.y; //where the rectangle actually ends

        int WasWhiteLine = 0; //false

        int frst_x = rect_x, frst_y = rect_y;

        if (!IsHorizontalPass) { //Vertical Pass
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
                        list = AddElement(list,frst_x,rect_y,i - frst_x, rect_height);
                        //Draw_Rect(mask,new_Rect);
                        do_it_again++; //on a une ligne blanche donc on devra refaire forcément un tour sur tt les
                    }
                    else
                    {
                        Rect blueline = CreateRect(i, frst_y, 0, relativ_rect_height);
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
                list = AddElement(list, frst_x, frst_y, rect_width - frst_x - 1, rect_height);
                //Draw_Rect(mask,new_Rect);
            }
        }else { //Horizontal Pass
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
                        list = AddElement(list, rect_x, frst_y, rect_width, i - frst_y);
                        //Draw_Rect(mask,new_Rect);
                        do_it_again++; //on a une ligne blanche donc on devra refaire forcément un tour sur tt les
                    }
                    else
                    {
                        Rect blueline = CreateRect(frst_x, i, relativ_rect_width, 0);
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
                list = AddElement(list, rect_x, frst_y, rect_width, rect_height - frst_y - 1);
            }
        }

    }

    printf("Final : len : %i\n", len);
    Rect_Mask_Couple res;
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

//CreateRect
//from the needed data, create a Rect (rectangle)
Rect CreateRect(int x, int y, int width, int height)
{

    Rect new_Rect;
    new_Rect.x = x;
    new_Rect.y = y;
    new_Rect.width = width;
    new_Rect.height = height;

    return new_Rect;
}


//AddElement
//Create and add a Rectangle (from the data) in the list of the Rect_List and edit its lenght
Rect_List AddElement(Rect_List list, int x, int y, int width, int height)
{
    list.lenght = list.lenght + 1;
    if (!(list.list = realloc(list.list, (list.lenght) * sizeof(Rect)))) {
        exit(-1);
    }
    list.list[list.lenght-1] = CreateRect(x, y, width, height);
    return list;
}

//DrawRect
//Draw on the mask, a rectangle of size rect of colol pixel_color
//If cpixel_color=42, the default color is blue
SDL_Surface* Draw_Rect(SDL_Surface* mask, Rect rect, Uint32 pixel_color) {
    printf("Draw  \n");
    if (pixel_color == 42)
        pixel_color = SDL_MapRGB(mask->format, 0, 0, 255);
    int x = rect.x, y = rect.y;
    int width = rect.width, height = rect.height;
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

//FreeBeginningOfRectList
//Should Free init_len Rect in list of Rect_List and edit lenght of the Rect_List
Rect_List FreeBeginningOfRectList(Rect_List list,Rect* rectangles, int init_len, int new_len)
{
    //TODO
    list.list = rectangles;
    list.lenght = new_len - init_len;
    return list;
}
