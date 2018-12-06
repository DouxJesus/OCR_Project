
// #include <gtk/gtk.h>
// #include <stdlib.h>
// #define GTK_STOCK_MEDIA_PLAY       "gtk-media-play"


// //VARIABLES GLOBALES
// GtkWidget * display_surface = NULL;
// const gchar *filename = NULL;
// GtkWidget *text_view = NULL;
// GtkTextIter iter;
// GtkTextBuffer *text_buffer = NULL;
// GtkImage *image = NULL;

// static void LoadImg(GtkWidget* button, GtkWidget* frame)
// {

// 	GtkWidget *dialog = NULL;
//     gint res;

//     dialog = gtk_file_chooser_dialog_new ("Open Image", GTK_WINDOW(frame), GTK_FILE_CHOOSER_ACTION_OPEN, ("_Cancel"),
//                                           GTK_RESPONSE_CANCEL, ("_Open"), GTK_RESPONSE_ACCEPT,
//                                           NULL);
//     res = gtk_dialog_run(GTK_DIALOG (dialog));

//     if (res == GTK_RESPONSE_ACCEPT)
//     {
//     	GtkFileChooser *choosefile = GTK_FILE_CHOOSER(dialog);
//         filename = gtk_file_chooser_get_filename(choosefile);
//         printf("%s\n", filename);
        
//         GtkImage* tmpimg = gtk_image_new_from_file(filename);
//         GdkPixbuf* pix_buff = gtk_image_get_pixbuf(tmpimg);
//         gtk_image_set_from_pixbuf(image, pix_buff);
// 		//gtk_image_set_from_image(image, tmpimg, NULL); //gtk_image_new_from_file(filename);
// 		//GdkPixbuf* pix_buff =  gtk_image_get_pixbuf(image);
// 		//gtk_container_add(GTK_CONTAINER(frame), image);
//     	//gtk_frame_set_label(GTK_FRAME(frame), "Run the OCR !");
// 	}
// 	 gtk_widget_destroy (dialog);
// }

// static void runOCR()
// {
// 	printf("%s\n", filename);
// 	gtk_text_buffer_set_text (text_buffer, filename, -1);
// 	//gtk_text_buffer_insert(text_buffer, &iter, filename, -1);

// }

// /*gint progress (gpointer data)
//  {
//     gfloat pvalue; 
//     pvalue = GTK_PROGRESS_BAR (data)->percentage;
     
//     if ((pvalue >= 1.0) || (pstat == FALSE)) {
//         pvalue = 0.0;
//         pstat = TRUE;
//     }
//     pvalue += 0.01;
    
//     gtk_progress_bar_update (GTK_PROGRESS_BAR (data), pvalue);
    
//     return TRUE;
//  }*/

// int main(int argc, char** argv){
//     gtk_init(&argc, &argv);

//     //INIT
//     GtkWidget * MainWindow = NULL;

//     GtkWidget* Mainbox = NULL;
//     GtkWidget *upBox = NULL;
// 	GtkWidget *downBox = NULL;
//     GtkWidget *toolbar = NULL;
//     GtkToolItem *openFile = NULL;
//     GtkToolItem *run = NULL;
//     GtkToolItem *exit = NULL;
//     GtkToolItem *separator = NULL;

// 	GtkWidget *left_box = NULL;
// 	GtkWidget *right_box = NULL;

//     GtkWidget * display_surface = NULL;

//     GtkImage* image_surface = NULL;

//     GtkWidget* label = NULL;
//     GtkWidget *ProgressionBar = NULL;
//     GtkWidget *runBtn = NULL;

//     //MAIN WINDOWS
//     MainWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
//     gtk_window_set_title(GTK_WINDOW(MainWindow), "OCR");
//     gtk_window_set_position(GTK_WINDOW (MainWindow), GTK_WIN_POS_CENTER);
//     gtk_window_maximize(GTK_WINDOW(MainWindow));
//     g_signal_connect(G_OBJECT(MainWindow), "delete-event", G_CALLBACK(gtk_main_quit), NULL);


//     //BOXES
//     Mainbox = gtk_vbox_new(FALSE, 5);
//     gtk_container_add(GTK_CONTAINER(MainWindow), Mainbox);

//     upBox = gtk_vbox_new(FALSE, 5);
//     gtk_box_pack_start(GTK_BOX(Mainbox), upBox, 0, 0, 0);

//     downBox = gtk_hbox_new(TRUE, 5);
//     gtk_box_pack_start(GTK_BOX(Mainbox), downBox, 1, 1, 0);

//     left_box = gtk_vbox_new(FALSE, 5);
//     gtk_box_pack_start(GTK_BOX(downBox), left_box, 1, 1, 0);

//     right_box = gtk_vbox_new(FALSE, 5);
//     gtk_box_pack_start(GTK_BOX(downBox), right_box, 1, 1, 0);

//     //TOOLBAR
//     toolbar = gtk_toolbar_new();
//     gtk_toolbar_set_style(GTK_TOOLBAR(toolbar), GTK_TOOLBAR_ICONS);

//     openFile = gtk_tool_button_new_from_stock(GTK_STOCK_OPEN);
//     gtk_toolbar_insert(GTK_TOOLBAR(toolbar), openFile, -1);

//     run = gtk_tool_button_new_from_stock(GTK_STOCK_MEDIA_PLAY);
//     gtk_toolbar_insert(GTK_TOOLBAR(toolbar), run, -1);
//     g_signal_connect(G_OBJECT(run), "clicked", G_CALLBACK(runOCR), NULL);

//     exit = gtk_tool_button_new_from_stock(GTK_STOCK_QUIT);
//     gtk_toolbar_insert(GTK_TOOLBAR(toolbar), exit, -1);
//     g_signal_connect(G_OBJECT(exit), "clicked", G_CALLBACK(gtk_main_quit), NULL);
//     /*separator = gtk_separator_tool_item_new();
//     gtk_toolbar_insert(GTK_TOOLBAR(toolbar), separator, -1);*/

//     gtk_container_add(GTK_CONTAINER(upBox),toolbar);

//  	//LABELS
//     label = gtk_label_new ("<big><big>Select a image to analyse</big></big>");
//     gtk_container_add (GTK_CONTAINER(display_surface), label);
//     gtk_label_set_use_markup(GTK_LABEL(label), 1);

//     //SURFACE AFFICHAGE IMAGE
//     image = gtk_image_new_from_file(filename);
//     //GdkPixbuf* pix_buff =  gtk_image_get_pixbuf(image);
//     gtk_container_add(GTK_CONTAINER(left_box), image);
//     g_signal_connect(G_OBJECT(openFile), "clicked", G_CALLBACK(LoadImg), left_box); 


//     //SURFACE DE TEXTE
//   	text_view = gtk_text_view_new();
//     text_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
//   	gtk_box_pack_start (GTK_BOX (right_box), text_view, TRUE, TRUE, 0);
//   	gtk_text_buffer_get_iter_at_line(text_buffer, &iter, 0);
//   	gtk_text_buffer_insert(text_buffer, &iter, "Hello OCR Project! :)\nPlease Select a file to start.", -1);

//     ProgressionBar = gtk_progress_bar_new ();
//     //gtk_table_attach_defaults(GTK_TABLE(table), ProgressionBar, 0,2,1,2);
//     //gtk_widget_show (ProgressionBar); 
//     //ptimer = gtk_timeout_add (100, progress, ProgressionBar);

//     gtk_widget_show_all(MainWindow);
//     gtk_main();

//     return 0;
// }