#include <gtk/gtk.h>
#include <stdio.h>
#include "notepad.h"

void save_to_file(char *filename) {
  printf(filename);
  GtkTextIter start, end;
  g_print("before buffer\n");
  GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_area));
  g_print("after buffer\n");
  FILE *fp;
  fp = fopen(filename, "w");
  if(fp==NULL) {
    g_printerr("Could not save file!\n");
    exit(1);
  }
  gtk_text_buffer_get_start_iter(buffer, &start);
  g_print("iter start\n");
  gtk_text_buffer_get_end_iter(buffer, &end);
  fprintf(fp, gtk_text_buffer_get_text(buffer, &start, &end, TRUE));
  fclose(fp);
}

static void
save (GtkDialog *dialog, int response)
{
  if(response == GTK_RESPONSE_ACCEPT) {
    GtkFileChooser *chooser = GTK_FILE_CHOOSER (dialog);

    g_autoptr(GFile) file = gtk_file_chooser_get_file (chooser);
    save_to_file((char *)g_file_get_path(file));
    g_free(file);
  }
  gtk_window_destroy(GTK_WINDOW(dialog));
}



static void 
save_dialog (GtkWidget *widget, gpointer *data) {
  GtkWidget *dialog = gtk_file_chooser_dialog_new("Save to",GTK_WINDOW(window),GTK_FILE_CHOOSER_ACTION_SAVE, "Cancel", GTK_RESPONSE_CANCEL, "Open", GTK_RESPONSE_ACCEPT, NULL);
  g_print("Another lol bites the dust\n");
  g_signal_connect(dialog, "response", G_CALLBACK(save), NULL);
  gtk_widget_show(dialog);
}

static void
activate (GtkApplication *app,
          gpointer        user_data)
{

  window = gtk_application_window_new (app);
  gtk_window_set_title (GTK_WINDOW (window), "Window");
  gtk_window_set_default_size (GTK_WINDOW (window), 200, 200);
  //gtk_window_set_titlebar(GTK_WINDOW(window), NULL);

  text_area = gtk_text_view_new();
  gtk_window_set_child (GTK_WINDOW (window), text_area);

  head_bar = gtk_header_bar_new();

  save_button = gtk_button_new_with_label("Save");
  gtk_header_bar_pack_start(GTK_HEADER_BAR(head_bar), save_button);
  g_signal_connect(save_button, "clicked", G_CALLBACK(save_dialog), NULL);
  gtk_window_set_titlebar(GTK_WINDOW(window), head_bar);

  gtk_window_present (GTK_WINDOW (window));
}

int
main (int    argc,
      char **argv)
{
  GtkApplication *app;
  int status;

  app = gtk_application_new ("org.gtk.example", G_APPLICATION_FLAGS_NONE);
  g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
  status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);

  return status;
}
