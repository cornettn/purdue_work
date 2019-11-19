#include <gtk/gtk.h>
#include <stdio.h>

int
main (int   argc,
      char *argv[])
{
  GtkBuilder *builder;
  GObject *window;
  GObject *stack;
  //  GObject *button;
  GError *error = NULL;

  gtk_init (&argc, &argv);

  /* Construct a GtkBuilder instance and load our UI description */
  builder = gtk_builder_new ();
  if (gtk_builder_add_from_file (builder, "builder.ui", &error) == 0)
    {
      printf("probelms!!!!!!!!!!!!!\n");
      g_printerr ("Error loading file: %s\n", error->message);
      g_clear_error (&error);
      return 1;
    }


  /* Connect signal handlers to the constructed widgets. */

  window = gtk_builder_get_object (builder, "window");
  g_signal_connect (window, "destroy", G_CALLBACK (gtk_main_quit), NULL);

  printf("1\n");

  stack = gtk_builder_get_object(builder, "tab1");
  printf("2\n");
  stack = gtk_builder_get_object(builder, "tab2");
  printf("3\n");
  stack = gtk_builder_get_object(builder, "tab3");
  printf("4\n");
  stack = gtk_builder_get_object(builder, "tab4");
  printf("5\n");

//  button = gtk_builder_get_object (builder, "button1");
//  g_signal_connect (button, "clicked", G_CALLBACK (print_hello), NULL);

//  button = gtk_builder_get_object (builder, "button2");
//  g_signal_connect (button, "clicked", G_CALLBACK (print_hello), NULL);

//  button = gtk_builder_get_object (builder, "quit");
//  g_signal_connect (button, "clicked", G_CALLBACK (gtk_main_quit), NULL);

  if (stack) {
    printf("stack");
  }

  gtk_main ();

  return 0;
}
