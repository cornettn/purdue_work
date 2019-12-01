#include "gui.h"

#include <stdio.h>
#include <cairo.h>

#include "logger.h"
#include "sys_info.h"
#include "pid_parser.h"
#include "graph_info.h"
#include "file_system.h"
//#include "slope/slope.h"

#define UNUSED(x) (void)(x)
#define BUF_SIZE (1024)
#define PROCESS_PAGE_NUM (1)

/* Function Declarations */

int static link_all_buttons();
void static link_menu_bar_buttons();
void static link_monitor_buttons();
void static link_edit_buttons();
void static link_view_buttons();
void static link_help_buttons();
void static init_system_tab();
void static init_resource_graphs();
void static init_file_systems();
void static link_test();
void static link_tabs();

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
  link_tabs();
  link_test();
  init_system_tab();
  init_resource_graphs();
  init_file_systems();

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

/*
 * This function is used grey out the process views depending on the page
 * selected.
 */

void process_view_visibility(GtkNotebook *widget, GtkWidget *page, guint page_num,
                             gpointer data) {

  /* Grey out the process views if not on process page */

  if ( page_num == PROCESS_PAGE_NUM ) {
    gtk_widget_set_sensitive(GTK_WIDGET(my_proc), TRUE);
    gtk_widget_set_sensitive(GTK_WIDGET(all_proc), TRUE);
    gtk_widget_set_sensitive(GTK_WIDGET(active_proc), TRUE);
    return;
  }

  gtk_widget_set_sensitive(GTK_WIDGET(my_proc), FALSE);
  gtk_widget_set_sensitive(GTK_WIDGET(all_proc), FALSE);
  gtk_widget_set_sensitive(GTK_WIDGET(active_proc), FALSE);
} /* process_view_visibiliyu() */

/*
 * This function is used to link all of tabs to specific functions.
 */

void static link_tabs() {
  mylog("Linking Tabs");

  /* Initialize the views to be greyed out */

  gtk_widget_set_sensitive(GTK_WIDGET(my_proc), FALSE);
  gtk_widget_set_sensitive(GTK_WIDGET(all_proc), FALSE);
  gtk_widget_set_sensitive(GTK_WIDGET(active_proc), FALSE);

  /* Link the page switches */

  GObject *all_tabs = gtk_builder_get_object(builder, "all_tabs");
  g_signal_connect(G_OBJECT(all_tabs), "switch-page",
                   G_CALLBACK(process_view_visibility), NULL);
} /* link_tabs() */


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
 * START PROCESSES TAB FUNCTIONS
 * ##############################################
 */

/*
 * This function will display the options for processes when a user right
 * clicks a process.
 */

void show_process_options() {
  mylog("show process options");
  GObject *popup = gtk_builder_get_object(builder, "process_options");
  gtk_popover_popup(GTK_POPOVER(popup));
  gtk_widget_show_all(GTK_WIDGET(popup));
  mylog("Should be popupped");

} /* show_process_options() */

/*
 * This function is used to append a row to end of the list store.
 */

void static add_row_to_processes(GtkListStore *list_store,
                                 GtkTreeIter *iter,
                                 process_t *proc) {
  /* Enum for the different columns in the tree view */

  enum {NAME, STATUS, CPU, ID, MEM};
  gint cols[] = {NAME, STATUS, CPU, ID, MEM};

  /* Initialize GValues */

  GValue name = G_VALUE_INIT;
  GValue status = G_VALUE_INIT;
  GValue cpu = G_VALUE_INIT;
  GValue pid = G_VALUE_INIT;
  GValue mem = G_VALUE_INIT;

  /* Cast GValues to appropiate type */

  g_value_init(&name, G_TYPE_STRING);
  g_value_init(&status, G_TYPE_STRING);
  g_value_init(&cpu, G_TYPE_STRING);
  g_value_init(&pid, G_TYPE_STRING);
  g_value_init(&mem, G_TYPE_STRING);

  /* Set the values according to proc */

  g_value_set_static_string(&name, proc->proc_name);
  g_value_set_static_string(&status, proc->state);
  g_value_set_static_string(&cpu, proc->cpu_time);
  g_value_set_static_string(&pid, proc->pid);
  g_value_set_static_string(&mem, proc->memory);

  GValues vals[] = {name, status, cpu, pid, mem};

  /* Append the row with the values to the list store */

  gtk_list_store_append(list_store, iter);
  gtk_list_store_set_valuesv(list_store, iter, cols, vals, 5);

} /* add_row_to_processes() */

/*
 * This function is used to display the processes.
 */

void display_procs(process_t **procs) {
  
  /* Get references for tree view */

  GObject *tree_view = gtk_builder_get_object(builder, "processes_tree_view");
  GtkTreeModel *model = gtk_tree_view_get_model(GTK_TREE_VIEW(tree_view));

  /* Get the tree path of the first row */

  GtkTreePath *tree_path = gtk_tree_path_new_first();

  /* Create Tree iterator */

  GtkTreeIter iter = {0};
  gtk_tree_model_get_iter(model, &iter, tree_path);

  GtkListStore *list_store =
    GTK_LIST_STORE(gtk_builder_get_object(builder, "processes_list_store"));

  /* Add processes to tree view */

  int counter = 0;
  process_t *curr_proc = procs[counter];
  while (curr_proc != NULL) {
    add_row_to_processes(list_store, &iter, curr_proc);
    curr_proc = procs[++counter];
  }
} /* display_processes() */


//PLACEHOLDER
process_t **get_all_process() {
  /* This function is a placeholder */
  
  process_t proc = {0};
  proc.pid = "pid";
  proc.proc_name = "name";
  proc.sate = "state";
  proc.memory "mem";
  proc.cpu_time = "cpu_time";

  process_t procs[] = {proc};
  return &procs;
}

//temp function used for testing
void static link_test() {
  GObject *button = gtk_builder_get_object(builder, "test_button");
  g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(show_process_options), NULL);
}

/*
 * This function is used to initialize the process tab. 
 */

void init_process_view() {

  /* Initialize view to see all processes */

  gtk_check_menu_item_set_active(all_proc, TRUE);

  // TODO Get all of the processes from the pid_parser

  process_t **procs = get_all_process();
  display_procs(procs);

} /* init_process_view() */

/*
 * ##############################################
 * END PROCESSES TAB FUNCTIONS
 * ##############################################
 */

/*
 * ##############################################
 * START RESOURCE TAB FUNCTIONS
 * ##############################################
 */

/*
 * Thus function is used to draw the graphs on the resource tab.
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
} /* draw_resources() */

/*
 * This function is used to initialize the resource graphs.
 */

void static init_resource_graphs() {
  GObject *graph = gtk_builder_get_object(builder, "cpu_usage_drawing_area");
  g_signal_connect(G_OBJECT(graph), "draw", G_CALLBACK(draw_resources), NULL);
} /* init_resource_graphs() */


/*
 * ##############################################
 * END RESOURCE TAB FUNCTIONS
 * ##############################################
 */


/*
 * ##############################################
 * START FILE SYSTEMS TAB FUNCTIONS
 * ##############################################
 */

/*
 * This function is used to append a mount to the file system list store.
 */

void static add_row_to_file_system(GtkListStore *list_store,
                                   GtkTreeIter *iter,
                                   mount *curr_mount) {

  /* Create an enum for the different column numbers in the tree view */

  enum {DEVICE, DIRECTORY, TYPE, TOT, FREE, AVAIL, UNUSE};
  gint cols[] = {DEVICE, DIRECTORY, TYPE, TOT, FREE, AVAIL, UNUSE};

  /* Initialize GValues */

  GValue dev = G_VALUE_INIT;
  GValue dir = G_VALUE_INIT;
  GValue type = G_VALUE_INIT;
  GValue tot = G_VALUE_INIT;
  GValue free = G_VALUE_INIT;
  GValue avail = G_VALUE_INIT;
  GValue unuse = G_VALUE_INIT;

  /* Cast GValues to approiate type */

  g_value_init(&dev, G_TYPE_STRING);
  g_value_init(&dir, G_TYPE_STRING);
  g_value_init(&type, G_TYPE_STRING);
  g_value_init(&tot, G_TYPE_DOUBLE);
  g_value_init(&free, G_TYPE_DOUBLE);
  g_value_init(&avail, G_TYPE_DOUBLE);
  g_value_init(&unuse, G_TYPE_DOUBLE);

  /* Set the values according to the contents of curr_mount */

  g_value_set_static_string(&dev, curr_mount->dev_name);
  g_value_set_static_string(&dir, curr_mount->dev_dir);
  g_value_set_static_string(&type, curr_mount->dev_type);
  g_value_set_double(&tot, curr_mount->dev_total_space);
  g_value_set_double(&free, curr_mount->dev_free_space);
  g_value_set_double(&avail, curr_mount->dev_avail_space);
  g_value_set_double(&unuse, curr_mount->dev_used_space);

  GValue vals[] = {dev, dir, type, tot, free, avail, unuse};

  /* Append the row with the values to the list store */

  gtk_list_store_append(list_store, iter);
  gtk_list_store_set_valuesv(list_store, iter, cols, vals, 7);

} /* add_row_to_file_system() */

/*
 * This function initializes the file_systems tab to display information.
 */

void static init_file_systems() {
  mount **mounts = get_mount_list();
  if (mounts == NULL) {
    mylog("Error Getting mount list");
    return;
  }

  /* Get references for Tree View */

  GObject *file_system_tree_view =
    gtk_builder_get_object(builder, "file_system_tree");

  GtkTreeModel *model =
    gtk_tree_view_get_model(GTK_TREE_VIEW(file_system_tree_view));

  /* Get the tree path of the first row */

  GtkTreePath *tree_path = gtk_tree_path_new_first();

  /* Get the tree iterator and tree store to edit rows */

  GtkTreeIter iter = {0};
  gtk_tree_model_get_iter(model, &iter, tree_path);

  GtkListStore *list_store =
    GTK_LIST_STORE(gtk_builder_get_object(builder, "file_system_list_store"));


  /* Add all of the mounts to the list store */

  //TODO Do not limit the number of mounts to 20
  int counter = 0;
  mount *curr_mount = mounts[counter];
  while(curr_mount != NULL) {
    add_row_to_file_system(list_store, &iter, curr_mount);
    curr_mount = mounts[++counter];
    if (counter > 20) {
      break;
    }
  }
} /* init_file_systems() */

/*
 * ##############################################
 * END FILE SYSTEMS TAB FUNCTIONS
 * ##############################################
 */
