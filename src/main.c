#include <stdlib.h>
#include <gtk/gtk.h>
#include <stdio.h>
#include <mntent.h>

#include "gui.h"
#include "sys_info.h"
#include "graph_info.h"
#include "file_system.h"
#include "logger.h"
#include "pid_parser.h"


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
/*
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


  net_hist *temp = get_net();

  printf("%lf - Recieving\n%lf - Total Recieved\n", temp->recieving, temp->total_recieved);
  printf("%lf - Sending\n%lf - Total Sent\n", temp->sending, temp->total_sent);

  ms_hist *test = get_memswap();
  printf("\n\n%lf Mem Use\n%lf - Mem Total\n", test->mem_use, test->mem_total);
  printf("%lf Swap Use\n%lf - Swap Total\n", test->swap_use, test->swap_total);
*/


  process_t **test = create_pid_list();
  int count = 0;

  while (test[count] != NULL) {
    printf("Name: %s, PID: %d\n", test[count]->proc_name, test[count]->pid);
    count++;
  }

  return 0;
}





