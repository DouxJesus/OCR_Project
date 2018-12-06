#include <gtk/gtk.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

#include "../ImageProcessing/BlackAndWhite.h"
#include "../Image_BMP/pixel_operations.h"
#include "../Image_BMP/BMP.h"
#include "../ImageProcessing/RLSA.h"
#include "../NeuralNetwork/NeuralCreate.h"
#include "../Restore/restore.h"
#define GTK_STOCK_MEDIA_PLAY       "gtk-media-play"



//VARIABLES GLOBALES
GtkWidget * MainWindow = NULL;
GtkWidget * display_surface = NULL;
gchar *filename = NULL;
GtkWidget *text_view = NULL;
GtkTextIter iter;
GtkTextBuffer *text_buffer = NULL;
GtkWidget *image = NULL;
GtkClipboard* clipboard = NULL;
char* str = "Hello OCR Project! :)\nPlease Select a file to start.";

static void LoadImg(GtkWidget* button, GtkWidget* buff)
{
	GtkWidget *dialog = NULL;
    dialog = gtk_file_chooser_dialog_new("Open Image", GTK_WINDOW(MainWindow), GTK_FILE_CHOOSER_ACTION_OPEN, ("_Cancel"),
                                          GTK_RESPONSE_CANCEL, ("_Open"), GTK_RESPONSE_ACCEPT,
                                          NULL);
    if (gtk_dialog_run(GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
    {
        filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));       
        GtkImage* tmpimg = gtk_image_new_from_file(filename);
        GdkPixbuf* pix_buff = gtk_image_get_pixbuf(tmpimg);
        pix_buff = gdk_pixbuf_scale_simple(pix_buff, buff->allocation.width, buff->allocation.height, GDK_INTERP_BILINEAR);
        gtk_image_set_from_pixbuf(GTK_IMAGE(image), pix_buff);
	}
	 gtk_widget_destroy (dialog);
}

static void Copy()
{
    if(!clipboard)
       clipboard = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);
    gtk_clipboard_set_text(clipboard, str, -1);
    gtk_clipboard_store(clipboard);
}


static void SaveTxt()
{
    GtkWidget *dialog;
    dialog = gtk_file_chooser_dialog_new ("Save Text",
                      NULL,
                      GTK_FILE_CHOOSER_ACTION_SAVE,
                      GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                      GTK_STOCK_SAVE, GTK_RESPONSE_ACCEPT,
                      NULL);
    if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
    {
        char *file;;
        file = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
        g_file_set_contents(file, str, -1, NULL);   
    }
    gtk_widget_destroy (dialog);
}


void runOCR()
{
	printf("%s\n", filename);
    str = filename;
	gtk_text_buffer_set_text (text_buffer, filename, -1);
    /*SDL_Surface* image_surface;
    init_sdl();
    image_surface = load_image((char*)filename);
    to_grayscale(image_surface);
    BlackAndWhite(image_surface);
    Word_List* RLSA_Output = RLSA(image_surface, 1);
    Network network = LoadNetwork();

    String* text = stringyfy(RLSA_Output);

    str = text->string;
    gtk_text_buffer_set_text(text_buffer, text->string, -1);*/

}

int main(int argc, char** argv){
    gtk_init(&argc, &argv);

    //INIT

    GtkWidget* Mainbox = NULL;
    GtkWidget *upBox = NULL;
	GtkWidget *downBox = NULL;
    GtkWidget *toolbar = NULL;

    GtkToolItem *openFile = NULL;
    GtkToolItem *run = NULL;
    GtkToolItem *save = NULL;
    GtkToolItem *copy = NULL;
    GtkToolItem *exit = NULL;

	GtkWidget *left_box = NULL;
	GtkWidget *right_box = NULL;

    //MAIN WINDOWS
    MainWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(MainWindow), "OCR");
    gtk_window_set_position(GTK_WINDOW (MainWindow), GTK_WIN_POS_CENTER);
    gtk_window_maximize(GTK_WINDOW(MainWindow));
    g_signal_connect(G_OBJECT(MainWindow), "delete-event", G_CALLBACK(gtk_main_quit), NULL);


    //BOXES
    Mainbox = gtk_vbox_new(FALSE, 5);
    gtk_container_add(GTK_CONTAINER(MainWindow), Mainbox);

    upBox = gtk_vbox_new(FALSE, 5);
    gtk_box_pack_start(GTK_BOX(Mainbox), upBox, 0, 0, 0);

    downBox = gtk_hbox_new(TRUE, 5);
    gtk_box_pack_start(GTK_BOX(Mainbox), downBox, 1, 1, 0);

    left_box = gtk_vbox_new(FALSE, 5);
    gtk_box_pack_start(GTK_BOX(downBox), left_box, 1, 1, 0);

    right_box = gtk_vbox_new(FALSE, 5);
    gtk_box_pack_start(GTK_BOX(downBox), right_box, 1, 1, 0);

    //TOOLBAR
    toolbar = gtk_toolbar_new();
    gtk_toolbar_set_style(GTK_TOOLBAR(toolbar), GTK_TOOLBAR_ICONS);

    openFile = gtk_tool_button_new_from_stock(GTK_STOCK_OPEN);
    gtk_toolbar_insert(GTK_TOOLBAR(toolbar), openFile, -1);

    run = gtk_tool_button_new_from_stock(GTK_STOCK_MEDIA_PLAY);
    gtk_toolbar_insert(GTK_TOOLBAR(toolbar), run, -1);
    g_signal_connect(G_OBJECT(run), "clicked", G_CALLBACK(runOCR), NULL);

    save = gtk_tool_button_new_from_stock(GTK_STOCK_SAVE_AS);
    gtk_toolbar_insert(GTK_TOOLBAR(toolbar), save, -1);
    g_signal_connect(G_OBJECT(save), "clicked", G_CALLBACK(SaveTxt), NULL);

    copy = gtk_tool_button_new_from_stock(GTK_STOCK_COPY);
    gtk_toolbar_insert(GTK_TOOLBAR(toolbar), copy, -1);
    g_signal_connect(G_OBJECT(copy), "clicked", G_CALLBACK(Copy), NULL);
    clipboard = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);

    exit = gtk_tool_button_new_from_stock(GTK_STOCK_QUIT);
    gtk_toolbar_insert(GTK_TOOLBAR(toolbar), exit, -1);
    g_signal_connect(G_OBJECT(exit), "clicked", G_CALLBACK(gtk_main_quit), NULL);

    gtk_container_add(GTK_CONTAINER(upBox),toolbar);

    //SURFACE AFFICHAGE IMAGE
    image = gtk_image_new_from_file(filename);
    gtk_container_add(GTK_CONTAINER(left_box), GTK_WIDGET(image));
    g_signal_connect(G_OBJECT(openFile), "clicked", G_CALLBACK(LoadImg), left_box); 


    //SURFACE DE TEXTE
  	text_view = gtk_text_view_new();
    text_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
  	gtk_box_pack_start (GTK_BOX (right_box), text_view, TRUE, TRUE, 0);
  	gtk_text_buffer_get_iter_at_line(text_buffer, &iter, 0);
  	gtk_text_buffer_insert(text_buffer, &iter, str, -1);

    gtk_widget_show_all(MainWindow);
    gtk_main();

    return 0;
}