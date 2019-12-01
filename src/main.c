#include <stdlib.h>
#include <gtk/gtk.h>
#include <stdio.h>
#include <mntent.h>

#include "gui.h"
#include "sys_info.h"
#include "graph_info.h"
#include "file_system.h"
#include "logger.h"



/* Debug function for the file_system */

void print_mounts(mount **list) {
  if (list == NULL) {
    return;
  }

  for (int x = 0; x < get_mount_num(); x++) {
    printf("Dev Name: %s\nDev Dir: %s\nDev Type: %s\nDev Total Space: %f\nDev Free Space: %f\nDev Avail Space: %f\nDev Used Space: %f\n",
            list[x]->dev_name, list[x]->dev_type, list[x]->dev_type,
            list[x]->dev_total_space, list[x]->dev_free_space,
            list[x]->dev_avail_space, list[x]->dev_used_space);
  }
}



int main(int argc, char **argv) {

  GtkBuilder *builder;
  GtkWidget *window;


  init_log();
  mylog("Logging Capabilites setup");


  gtk_init(&argc, &argv);


  builder = gtk_builder_new();
  gtk_builder_add_from_file(builder, "builder.ui", NULL);


  if (init_task_manager(builder) < 0) {
  	mylog("Error: Could not intialize the GUI");
  	fprintf(stderr, "Error: Initialization of GUI\n");
  }

  // init_sys_info();

  window = GTK_WIDGET(gtk_builder_get_object(builder, "main_window"));
  gtk_builder_connect_signals(builder, NULL);

  gtk_widget_show(window);
  gtk_main();

  return 0;
}





