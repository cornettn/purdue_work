#include "gui.h"

#include <stdio.h>
#include <cairo.h>
#include <slope.h>

#include "logger.h"
#include "sys_info.h"

#define BUF_SIZE (1024)

/* Function Declarations */

int static link_all_buttons();
void static link_menu_bar_buttons();
void static link_monitor_buttons();
void static link_edit_buttons();
void static link_view_buttons();
void static link_help_buttons();
void static init_system_tab();
void static init_resource_graphs();

/* Global Variables */

GtkBuilder *builder;
GtkCheckMenuItem *my_proc;
GtkCheckMenuItem *all_proc;
GtkCheckMenuItem *active_proc;

/*
 * ##############################################
 * START MENU BAR FUNCTIONS
 * ##############################################
 */

/*
 * This function is used to quit to application.
 */

void quit_app(GtkWidget *widget, gpointer data) {
  mylog("Quitting Application");
  stop_logging();
  g_object_unref(builder);
  gtk_main_quit();
  exit(1);
} /* quit_app() */

/*
 * This function is called whenever the user is adjusting what type of
 * view they want. i.e. All Processes, My Processes, Active Processes.
 * This function is reposnsible for changing the gui accordingly.
 */

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
} /* toggle_process_view() */

/*
 * This function is used to initialize the task manager with the
 * appropiate information.
 * NOTE: This function links all of the buttons together.
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
 * together as well as link the buttons to their appropiate function.
 */

int static link_all_buttons() {
  mylog("Link All Buttons");
  mylog("Link Main Quit");
  GObject *main_window = gtk_builder_get_object(builder, "main_window");
  g_signal_connect(main_window, "destroy", G_CALLBACK(quit_app), NULL);

  link_menu_bar_buttons();
  init_system_tab();
  init_resource_graphs();

  return 1;
} /* link_all_buttons() */

/*
 * This function is used to link all the buttons of the menu bar
 * to their respective action.
 */

void static link_menu_bar_buttons() {
  link_monitor_buttons();
	link_edit_buttons();
	link_view_buttons();
	link_help_buttons();
} /* link_menu_bar_buttons() */

/*
 * This function is used to link all of the 'Monitor' menu bar buttons.
 */

void static link_monitor_buttons() {
  GObject *quit = gtk_builder_get_object(builder, "monitor_quit_button");
	g_signal_connect(quit, "activate", G_CALLBACK(quit_app), NULL);
} /* link_monitor_buttons() */

/*
 * This function is used to link all of the 'Edit' menu bar buttons.
 */

void static link_edit_buttons() {
	return;
} /* link_edit_buttons() */

/*
 * This function is used to link of the 'View' menu bar buttons.
 */

void static link_view_buttons() {
  g_signal_connect(G_OBJECT(my_proc), "toggled", G_CALLBACK(toggle_process_view), my_proc);
  g_signal_connect(G_OBJECT(all_proc), "toggled", G_CALLBACK(toggle_process_view), all_proc);
  g_signal_connect(G_OBJECT(active_proc), "toggled", G_CALLBACK(toggle_process_view), active_proc);
} /* link_view_buttons() */

/*
 * This function is used to link the 'Help' menu bar buttons.
 */

void static link_help_buttons() {
	return;
} /* link_help_buttons() */


/*
 * ##############################################
 * END MENU BAR FUNCTIONS
 * ##############################################
 */

/*
 * ##############################################
 * START ALL TAB FUNCTIONS
 * ##############################################
 */

void process_view_visibility(GtkWidget *widget, GdkEvent *event,
                             gpointer data) {
  mylog("Tab Switch");
  int view = (int) data;

  if (view) {
    gtk_widget_set_sensitive(my_proc, TRUE);
    gtk_widget_set_sensitive(all_proc, TRUE);
    gtk_widget_set_sensitive(active_proc, TRUE);
    return;
  }

  gtk_widget_set_sensitive(my_proc, FALSE);
  gtk_widget_set_sensitive(all_proc, FALSE);
  gtk_widget_set_sensitive(active_proc, FALSE);
}

void link_tabs() {
  GObject *sys_tab = gtk_builder_get_object(builder, "system_tab");
  GObject *proc_tab = gtk_builder_get_object(builder, "process_tab");
  GObject *resource_tab = gtk_builder_get_object(builder, "process_tab");
  GObject *file_tab = gtk_builder_get_object(builder, "process_tab");

  g_signal_connect(G_OBJECT(sys_tab), "button-press-event", G_CALLBACK(process_view_visibility), 0);
  g_signal_connect(G_OBJECT(proc_tab), "button-press-event", G_CALLBACK(process_view_visibility), 1);
  g_signal_connect(G_OBJECT(resource_tab), "button-press-event", G_CALLBACK(process_view_visibility), 0);
  g_signal_connect(G_OBJECT(file_tab), "button-press-event", G_CALLBACK(process_view_visibility), 0);
}

/*
 * ##############################################
 * END ALL TAB FUNCTIONS
 * ##############################################
 */

/*
 * ##############################################
 * START SYSTEM TAB FUNCTIONS
 * ##############################################
 */

void static init_system_tab() {

  /* Initialize sys_info functions */

  if (init_sys_info() < 0) {
    mylog("Error initializing sys_info");
  }

  /* Get system information using sys_info.c */

  sys_info info = {0};
  fill_info(&info);

  /* Retrieve labels from builder */

  GObject *os_type = gtk_builder_get_object(builder, "os_type_label");
  GObject *os_verison = gtk_builder_get_object(builder, "os_release_version_label");
  GObject *kernel_version = gtk_builder_get_object(builder, "kernel_version_label");
  GObject *mem_amount = gtk_builder_get_object(builder, "memory_label");
  GObject *proc_type = gtk_builder_get_object(builder, "processor_type_label");
  GObject *disk_space = gtk_builder_get_object(builder, "disk_space_label");

  /* Set labels to correct text */

  gtk_label_set_text(GTK_LABEL(os_type), info.OS_name);
  gtk_label_set_text(GTK_LABEL(os_verison), info.OS_version);
  gtk_label_set_text(GTK_LABEL(kernel_version), info.kernel_version);
  gtk_label_set_text(GTK_LABEL(mem_amount), info.mem_total);
  gtk_label_set_text(GTK_LABEL(proc_type), info.CPU_info);

  char disk_storage[1024] = {0};
  sprintf(disk_storage, "%.2f", info.disk_storage);
  gtk_label_set_text(GTK_LABEL(disk_space), disk_storage);

}

/*
 * ##############################################
 * END SYSTEM TAB FUNCTIONS
 * ##############################################
 */

/*
 * ##############################################
 * START RESOURCE TAB FUNCTIONS
 * ##############################################
 */

gboolean draw_resources (GtkWidget *widget, cairo_t *cr, gpointer data) {
  mylog("Drawing Resource Graph");

  //guint width = 0;
  //guint height = 0;
  //GtkStyleContext *context;
  //GdkRGBA color;

  //context = gtk_widget_get_style_context(widget);
  //width = gtk_widget_get_allocated_width(widget);
  //height = gtk_widget_get_allocated_height(widget);

  //gtk_render_background(context, cr, 0, 0, width, height);

  cairo_set_source_rgb(cr, 0.6, 0.6, 0.6);
  cairo_set_line_width(cr, 1);
  cairo_rectangle(cr, 180, 20, 80, 80);
  cairo_stroke_preserve(cr);
  cairo_fill(cr);

  //gtk_style_context_get_color(context,
  //                            gtk_style_context_get_state (context),
  //                            &color);

//  gdk_cairo_set_source_rgba(cr, &color);

  cairo_fill(cr);

  return FALSE;
}

void static init_resource_graphs() {
  GObject *graph = gtk_builder_get_object(builder, "cpu_usage_drawing_area");
  g_signal_connect(G_OBJECT(graph), "draw", G_CALLBACK(draw_resources), NULL);
}


/*
 * ##############################################
 * END RESOURCE TAB FUNCTIONS
 * ##############################################
 */


