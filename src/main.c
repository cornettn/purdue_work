#include <stdlib.h>
#include <gtk/gtk.h>
#include <stdio.h>

#include "gui.h"
#include "sys_info.h"
#include "logger.h"

int main(int argc, char **argv) {
  GtkBuilder *builder;
  GtkWidget *window;

  /* Initialize logging capabilities */

  init_log();
  mylog("Logging Capabilites setup");

  /* Initialize GTK */

  gtk_init(&argc, &argv);

  /* Build the UI from the file made with Glade */

  builder = gtk_builder_new();
  gtk_builder_add_from_file(builder, "builder.ui", NULL);

  /* Initialize the GUI with all the information that should
   * already there i.e System Information, etc */

  if (init_task_manager(builder) < 0) {
  	mylog("Error: Could not intialize the GUI");
  	fprintf(stderr, "Error: Initialization of GUI\n");
  }

  window = GTK_WIDGET(gtk_builder_get_object(builder, "main_window"));
  gtk_builder_connect_signals(builder, NULL);

  g_object_unref(builder);

  gtk_widget_show(window);
  gtk_main();

  return 0;
}
