#include "gui.h"

#include "logger.h"

GtkBuilder *builder;

int link_all_buttons();
void link_menu_bar_buttons();
void link_monitor_buttons();
void link_edit_buttons();
void link_view_buttons();
void link_help_buttons();



void quit_app(GtkWidget *widget, gpointer data) {
  mylog("Quitting Application");
  gtk_main_quit();
  exit(1);
}

/*
 * This function is used to initialize the task manager with the
 * appropiate information.
 */

int init_task_manager(GtkBuilder *gui_builder) {
	mylog("Initializing Task Manager");
  builder = gui_builder;
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
  GObject *quit = gtk_builder_get_object(builder, "monitor-quit-button");
	g_signal_connect(quit, "activate", G_CALLBACK(quit_app), NULL);
}

void link_edit_buttons() {
	return;
}

void link_view_buttons() {
	return;
}

void link_help_buttons() {
	return;
}


