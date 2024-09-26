#include <gtk/gtk.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// Global variables for result label and operation selection
GtkWidget *resultLabel, *operationCombo;
GList *entryList = NULL;  // A list to hold multiple number entries

// Function to perform the calculation
void on_calculate_button_clicked(GtkWidget *widget, gpointer data) {
    const char *operation = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(operationCombo));
    
    if (operation == NULL) {
        gtk_label_set_text(GTK_LABEL(resultLabel), "Please select an operation!");
        return;
    }

    double result = 0;
    gboolean first = TRUE;
    for (GList *iter = entryList; iter != NULL; iter = iter->next) {
        const char *num_str = gtk_entry_get_text(GTK_ENTRY(iter->data));
        double num = atof(num_str);

        if (strcmp(operation, "+") == 0) {
            result += num;
        } else if (strcmp(operation, "-") == 0) {
            if (first) result = num;
            else result -= num;
        } else if (strcmp(operation, "*") == 0) {
            if (first) result = num;
            else result *= num;
        } else if (strcmp(operation, "/") == 0) {
            if (first) result = num;
            else if (num != 0) result /= num;
            else {
                gtk_label_set_text(GTK_LABEL(resultLabel), "Error: Division by 0");
                return;
            }
        } else if (strcmp(operation, "sin(x)") == 0 && first) {
            result = sin(num);
            break;
        } else if (strcmp(operation, "cos(x)") == 0 && first) {
            result = cos(num);
            break;
        }

        first = FALSE;
    }

    char result_str[100];
    snprintf(result_str, sizeof(result_str), "Result: %.2f", result);
    gtk_label_set_text(GTK_LABEL(resultLabel), result_str);
}

// Function to add another input field
void on_add_input_button_clicked(GtkWidget *widget, gpointer data) {
    GtkWidget *entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(data), entry, 1, g_list_length(entryList) + 1, 2, 1);
    gtk_widget_show(entry);
    entryList = g_list_append(entryList, entry);  // Add entry to the list
}

// Main function to create the GTK+ GUI
int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    // Create main window
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "MATLAB Lite - Unique with Multiple Inputs");
    gtk_window_set_default_size(GTK_WINDOW(window), 500, 400);
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);

    // Create a grid layout for neat arrangement
    GtkWidget *grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    // Add the first input field
    GtkWidget *label1 = gtk_label_new("Number 1:");
    GtkWidget *entry1 = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), label1, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry1, 1, 0, 2, 1);
    entryList = g_list_append(entryList, entry1);  // Add first entry to the list

    // Create a button to add more input fields
    GtkWidget *addInputButton = gtk_button_new_with_label("Add Input");
    g_signal_connect(addInputButton, "clicked", G_CALLBACK(on_add_input_button_clicked), grid);
    gtk_grid_attach(GTK_GRID(grid), addInputButton, 0, 1, 1, 1);

    // Create a dropdown for selecting operations
    operationCombo = gtk_combo_box_text_new();
    gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(operationCombo), NULL, "+");
    gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(operationCombo), NULL, "-");
    gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(operationCombo), NULL, "*");
    gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(operationCombo), NULL, "/");
    gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(operationCombo), NULL, "sin(x)");
    gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(operationCombo), NULL, "cos(x)");
    gtk_grid_attach(GTK_GRID(grid), operationCombo, 1, 2, 2, 1);

    // Create a button to calculate
    GtkWidget *calcButton = gtk_button_new_with_label("Calculate");
    g_signal_connect(calcButton, "clicked", G_CALLBACK(on_calculate_button_clicked), NULL);
    gtk_grid_attach(GTK_GRID(grid), calcButton, 1, 3, 1, 1);

    // Label to display result
    resultLabel = gtk_label_new("Result: ");
    gtk_grid_attach(GTK_GRID(grid), resultLabel, 1, 4, 2, 1);

    // Connect the "destroy" event to quit the application
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Show all widgets
    gtk_widget_show_all(window);

    // Run the main GTK+ event loop
    gtk_main();

    return 0;
}