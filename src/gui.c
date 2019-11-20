#include "gui.h"

#include <stdio.h>

#include "logger.h"

GtkBuilder *builder;

int link_all_buttons();
void link_menu_bar_buttons();
void link_monitor_buttons();
void link_edit_buttons();
void link_view_buttons();
void link_help_buttons();

GtkCheckMenuItem *my_proc;
GtkCheckMenuItem *all_proc;
GtkCheckMenuItem *active_proc;


void quit_app(GtkWidget *widget, gpointer data) {
  mylog("Quitting Application");
  gtk_main_quit();
  exit(1);
}

void toggle_process_view(GtkWidget *widget, gpointer data) {
  GtkCheckMenuItem *toggle = data;

  /* Only change the other checks if toggle has been turned on */

  if (gtk_check_menu_item_get_active(toggle)) {

    /* Only turn the toggles off if they are on and are not the
     * intended toggle */

    if ((my_proc != toggle) &&
        (gtk_check_menu_item_get_active(my_proc)) ) {
      gtk_check_menu_item_set_active(my_proc, FALSE);
    }

    if ((all_proc != toggle) &&
        (gtk_check_menu_item_get_active(all_proc)) ) {
      gtk_check_menu_item_set_active(all_proc, FALSE);
    }

    if ((active_proc != toggle) &&
        (gtk_check_menu_item_get_active(active_proc)) ) {
      gtk_check_menu_item_set_active(active_proc, FALSE);
    }
  }
}

/*
 * This function is used to initialize the task manager with the
 * appropiate information.
 */

int init_task_manager(GtkBuilder *gui_builder) {
	mylog("Initializing Task Manager");
  builder = gui_builder;

  GObject *toggle = gtk_builder_get_object(builder, "view_my_processes");
  my_proc = (GtkCheckMenuItem *) toggle;

  toggle = gtk_builder_get_object(builder, "view_all_processes");
  all_proc = (GtkCheckMenuItem *) toggle;

  toggle = gtk_builder_get_object(builder, "view_active_processes");
  active_proc = (GtkCheckMenuItem *) toggle;

  link_all_buttons();
  return 1;
} /* init_task_manager() */

/*
 * This function is used to link all of the buttons in the task manager
 * together.
 */

int link_all_buttons() {
  mylog("Link All Buttons");
  mylog("Link Main Quit");
  GObject *main_window = gtk_builder_get_object(builder, "main_window");
  g_signal_connect(main_window, "destroy", G_CALLBACK(quit_app), NULL);

  link_menu_bar_buttons();

  return 1;
} /* link_all_buttons() */

/*
 * This function is used to link all the buttons of the menu bar
 * to their respective action.
 */

void link_menu_bar_buttons() {
	link_monitor_buttons();
	link_edit_buttons();
	link_view_buttons();
	link_help_buttons();
} /* link_menu_bar_buttons() */

void link_monitor_buttons() {
  mylog("Link quit button");
  GObject *quit = gtk_builder_get_object(builder, "monitor_quit_button");
	g_signal_connect(quit, "activate", G_CALLBACK(quit_app), NULL);
}

void link_edit_buttons() {
	return;
}

void link_view_buttons() {
  g_signal_connect(G_OBJECT(my_proc), "toggled", G_CALLBACK(toggle_process_view), my_proc);

  g_signal_connect(G_OBJECT(all_proc), "toggled", G_CALLBACK(toggle_process_view), all_proc);

  g_signal_connect(G_OBJECT(active_proc), "toggled", G_CALLBACK(toggle_process_view), active_proc);

  return;
}

void link_help_buttons() {
	return;
}


