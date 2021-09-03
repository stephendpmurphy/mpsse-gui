#include <gtk/gtk.h>
#include "mpsse.h"

GtkBuilder *builder;
GtkWidget *window;
GtkWidget *fixed1;
GtkWidget *btnSend;
GtkWidget *btnClear;
GtkWidget *lblStatus;
GtkWidget *scrollWindow;
GtkWidget *bxTxRx;

static int rowCounter = 0;

static void addListRow(void) {
   GtkWidget *row;
   GtkWidget *label;
   GtkWidget *hbox;
   char str[32];

   sprintf(str, "Row #%d", rowCounter++);

   hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
   label = gtk_label_new(str);
   gtk_box_pack_start(GTK_BOX(hbox), label, TRUE, TRUE, 0);

   gtk_box_pack_end(GTK_BOX(bxTxRx), hbox, TRUE, TRUE, 0);

   gtk_widget_show_all(bxTxRx);

   // // Auto scroll to the bottom
   // GtkAdjustment *adj = gtk_scrolled_window_get_vadjustment(GTK_SCROLLED_WINDOW(scrollWindow));
   // gtk_adjustment_set_value(adj,(gtk_adjustment_get_upper(adj) - gtk_adjustment_get_page_size(adj) + 50));

}

static void clearBox(void) {
   GList *children, *iter;

   children = gtk_container_get_children(GTK_CONTAINER(bxTxRx));
   for(iter = children; iter != NULL; iter = g_list_next(iter))
   gtk_widget_destroy(GTK_WIDGET(iter->data));
   g_list_free(children);
}

void on_btnSend_clicked( GtkWidget *widget, gpointer   data ) {
   addListRow();
}

void on_btnClear_clicked( GtkWidget *widget, gpointer   data ) {
   clearBox();
   rowCounter = 0;
}
int main(int argc,char *argv[]) {
   gtk_init(&argc, &argv);

   builder = gtk_builder_new_from_file("../layout.glade");

   window = GTK_WIDGET(gtk_builder_get_object(builder, "window1"));

   g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

   gtk_builder_connect_signals(builder, NULL);

   fixed1 = GTK_WIDGET(gtk_builder_get_object(builder, "fixed1"));
   btnSend = GTK_WIDGET(gtk_builder_get_object(builder, "btnSend"));
   btnClear = GTK_WIDGET(gtk_builder_get_object(builder, "btnClear"));
   lblStatus = GTK_WIDGET(gtk_builder_get_object(builder, "lblStatus"));
   bxTxRx = GTK_WIDGET(gtk_builder_get_object(builder, "bxTxRx"));
   scrollWindow = GTK_WIDGET(gtk_builder_get_object(builder, "scrollWindow"));

   gtk_widget_show(window);

   if( mpsse_init() == EXIT_FAILURE ) {
      gtk_label_set_text(GTK_LABEL(lblStatus), (const gchar*)"Failed to init MPSSE module.");
   }
   else {
      gtk_label_set_text(GTK_LABEL(lblStatus), (const gchar*)"MPSSE Init successful.");
   }

   gtk_main();

   return 0;
}

// static int counter = 0;

// void greet( GtkWidget *widget, gpointer   data )
// {
//    // printf equivalent in GTK+
//    g_print ("Hi there! Welcome to GTK\n");
//    g_print ("%s clicked %d times\n",
//       (char*)data, ++counter);

//    if( gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget)) ) {
//       g_print("Checked!");
//    }
//    else {
//       g_print("Unchecked!");
//    }
// }

// void destroy( GtkWidget *widget,gpointer   data )
// {
//    gtk_main_quit ();
// }

// int main( int   argc,char *argv[] )
// {

//    GtkWidget *window;
//    GtkWidget *button;
//    GtkWidget *button2;
//    GtkWidget *box;
//    gtk_init (&argc, &argv);

//    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);

//    g_signal_connect (window, "destroy",
//    G_CALLBACK (destroy), NULL);
//    /* Let's set the border width of the window to 20.
//     * You may play with the value and see the
//     * difference. */
//    gtk_container_set_border_width
//       (GTK_CONTAINER (window), 100);

//    button = gtk_check_button_new();
//    // button = gtk_button_new_with_label ("Click Me!");
//    button2 = gtk_button_new_with_label("Click me #2");


//    g_signal_connect (ATK_OBJECT(button),
//       "clicked",G_CALLBACK (greet),
//       "button");

//    g_signal_connect (ATK_OBJECT(button2),
//       "clicked",G_CALLBACK (greet),
//       "button #2");

//    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 3);

//    gtk_container_add (GTK_CONTAINER (box), button);
//    gtk_container_add (GTK_CONTAINER (box), button2);
//    gtk_container_add (GTK_CONTAINER (window), box);

//    gtk_widget_show_all(window);

//    gtk_main ();

//    return 0;
// }

// #include <gtk/gtk.h>

// enum
// {
//   COL_FIRST_NAME = 0,
//   COL_LAST_NAME,
//   COL_YEAR_BORN,
//   NUM_COLS
// } ;

// static GtkTreeModel *
// create_and_fill_model (void)
// {
//   GtkTreeStore  *treestore;
//   GtkTreeIter    toplevel, child;

//   treestore = gtk_tree_store_new(NUM_COLS,
//                                  G_TYPE_STRING,
//                                  G_TYPE_STRING,
//                                  G_TYPE_UINT);

//   /* Append a top level row and leave it empty */
//   gtk_tree_store_append(treestore, &toplevel, NULL);
//   gtk_tree_store_set(treestore, &toplevel,
//                      COL_FIRST_NAME, "Maria",
//                      COL_LAST_NAME, "Incognito",
//                      -1);

//   /* Append a second top level row, and fill it with some data */
//   gtk_tree_store_append(treestore, &toplevel, NULL);
//   gtk_tree_store_set(treestore, &toplevel,
//                      COL_FIRST_NAME, "Jane",
//                      COL_LAST_NAME, "Average",
//                      COL_YEAR_BORN, (guint) 1962,
//                      -1);

//   /* Append a child to the second top level row, and fill in some data */
//   gtk_tree_store_append(treestore, &child, &toplevel);
//   gtk_tree_store_set(treestore, &child,
//                      COL_FIRST_NAME, "Janinita",
//                      COL_LAST_NAME, "Average",
//                      COL_YEAR_BORN, (guint) 1985,
//                      -1);

//   return GTK_TREE_MODEL(treestore);
// }

// void
// age_cell_data_func (GtkTreeViewColumn *col,
//                     GtkCellRenderer   *renderer,
//                     GtkTreeModel      *model,
//                     GtkTreeIter       *iter,
//                     gpointer           user_data)
// {
//   guint  year_born;
//   guint  year_now = 2003; /* to save code not relevant for the example */
//   gchar  buf[64];

//   gtk_tree_model_get(model, iter, COL_YEAR_BORN, &year_born, -1);

//   if (year_born <= year_now && year_born > 0)
//   {
//     guint age = year_now - year_born;

//     g_snprintf(buf, sizeof(buf), "%u years old", age);

//     g_object_set(renderer, "foreground-set", FALSE, NULL); /* print this normal */
//   }
//   else
//   {
//     g_snprintf(buf, sizeof(buf), "age unknown");

//     /* make red */
//     g_object_set(renderer, "foreground", "Red", "foreground-set", TRUE, NULL);
//   }

//   g_object_set(renderer, "text", buf, NULL);
// }


// static GtkWidget *
// create_view_and_model (void)
// {
//   GtkTreeViewColumn   *col;
//   GtkCellRenderer     *renderer;
//   GtkWidget           *view;
//   GtkTreeModel        *model;

//   view = gtk_tree_view_new();

//   /* --- Column #1 --- */

//   col = gtk_tree_view_column_new();

//   gtk_tree_view_column_set_title(col, "First Name");

//   /* pack tree view column into tree view */
//   gtk_tree_view_append_column(GTK_TREE_VIEW(view), col);

//   renderer = gtk_cell_renderer_text_new();

//   /* pack cell renderer into tree view column */
//   gtk_tree_view_column_pack_start(col, renderer, TRUE);

//   /* connect 'text' property of the cell renderer to
//    *  model column that contains the first name */
//   gtk_tree_view_column_add_attribute(col, renderer, "text", COL_FIRST_NAME);


//   /* --- Column #2 --- */

//   col = gtk_tree_view_column_new();

//   gtk_tree_view_column_set_title(col, "Last Name");

//   /* pack tree view column into tree view */
//   gtk_tree_view_append_column(GTK_TREE_VIEW(view), col);

//   renderer = gtk_cell_renderer_text_new();

//   /* pack cell renderer into tree view column */
//   gtk_tree_view_column_pack_start(col, renderer, TRUE);

//   /* connect 'text' property of the cell renderer to
//    *  model column that contains the last name */
//   gtk_tree_view_column_add_attribute(col, renderer, "text", COL_LAST_NAME);

//   /* set 'weight' property of the cell renderer to
//    *  bold print (we want all last names in bold) */
//   g_object_set(renderer,
//                "weight", PANGO_WEIGHT_BOLD,
//                "weight-set", TRUE,
//                NULL);


//   /* --- Column #3 --- */

//   col = gtk_tree_view_column_new();

//   gtk_tree_view_column_set_title(col, "Age");

//   /* pack tree view column into tree view */
//   gtk_tree_view_append_column(GTK_TREE_VIEW(view), col);

//   renderer = gtk_cell_renderer_text_new();

//   /* pack cell renderer into tree view column */
//   gtk_tree_view_column_pack_start(col, renderer, TRUE);

//   /* connect a cell data function */
//   gtk_tree_view_column_set_cell_data_func(col, renderer, age_cell_data_func, NULL, NULL);


//   model = create_and_fill_model();

//   gtk_tree_view_set_model(GTK_TREE_VIEW(view), model);

//   g_object_unref(model); /* destroy model automatically with view */

//   gtk_tree_selection_set_mode(gtk_tree_view_get_selection(GTK_TREE_VIEW(view)),
//                               GTK_SELECTION_NONE);

//   return view;
// }


// int
// main (int argc, char **argv)
// {
//   GtkWidget *window;
//   GtkWidget *view;

//   gtk_init(&argc, &argv);

//   window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
//   g_signal_connect(window, "delete_event", gtk_main_quit, NULL); /* dirty */

//   view = create_view_and_model();

//   gtk_container_add(GTK_CONTAINER(window), view);

//   gtk_widget_show_all(window);

//   gtk_main();

//   return 0;
// }
