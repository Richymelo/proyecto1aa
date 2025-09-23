/*
 * Cuadros Mágicos sin archivo .glade - interfaz creada programáticamente
 */

#include <gtk/gtk.h>
#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define MAX_SIZE 21

// Estructura simplificada
typedef struct {
    GtkWidget *main_window;
    GtkWidget *size_spin;
    GtkWidget *create_button;
    GtkWidget *grid;
    GtkWidget *info_label;
    GtkWidget *cell_labels[MAX_SIZE][MAX_SIZE];
    int matrix[MAX_SIZE][MAX_SIZE];
    int size;
} AppData;

// Función simple para crear cuadros mágicos
void crear_cuadro_magico(AppData *app) {
    app->size = (int)gtk_spin_button_get_value(GTK_SPIN_BUTTON(app->size_spin));
    
    // Asegurar número impar
    if (app->size % 2 == 0) app->size++;
    
    // Limpiar grid anterior
    gtk_container_foreach(GTK_CONTAINER(app->grid), (GtkCallback)gtk_widget_destroy, NULL);
    
    // Inicializar matriz
    for (int i = 0; i < app->size; i++) {
        for (int j = 0; j < app->size; j++) {
            app->matrix[i][j] = 0;
        }
    }
    
    // Algoritmo Siamés básico
    int row = 0;
    int col = app->size / 2;
    
    for (int num = 1; num <= app->size * app->size; num++) {
        app->matrix[row][col] = num;
        
        int next_row = (row - 1 + app->size) % app->size;
        int next_col = (col + 1) % app->size;
        
        if (app->matrix[next_row][next_col] != 0) {
            row = (row + 1) % app->size;
        } else {
            row = next_row;
            col = next_col;
        }
    }
    
    // Crear interfaz visual
    for (int i = 0; i < app->size; i++) {
        for (int j = 0; j < app->size; j++) {
            GtkWidget *frame = gtk_frame_new(NULL);
            app->cell_labels[i][j] = gtk_label_new("");
            
            char text[10];
            snprintf(text, sizeof(text), "%d", app->matrix[i][j]);
            gtk_label_set_text(GTK_LABEL(app->cell_labels[i][j]), text);
            
            gtk_widget_set_size_request(app->cell_labels[i][j], 50, 50);
            gtk_container_add(GTK_CONTAINER(frame), app->cell_labels[i][j]);
            gtk_grid_attach(GTK_GRID(app->grid), frame, j, i, 1, 1);
        }
    }
    
    gtk_widget_show_all(app->grid);
    
    // Actualizar información
    int suma_magica = app->size * (app->size * app->size + 1) / 2;
    char info[100];
    snprintf(info, sizeof(info), "Cuadro Mágico %dx%d - Suma Mágica: %d", 
             app->size, app->size, suma_magica);
    gtk_label_set_text(GTK_LABEL(app->info_label), info);
}

void on_create_clicked(GtkButton *button, gpointer user_data) {
    AppData *app = (AppData*)user_data;
    crear_cuadro_magico(app);
}

void on_window_destroy(GtkWidget *widget, gpointer data) {
    gtk_main_quit();
}

int main(int argc, char *argv[]) {
    printf("INFO: Iniciando Cuadros Mágicos (Sin archivo .glade)\n");
    
    gtk_init(&argc, &argv);
    srand(time(NULL));
    
    AppData *app = g_malloc0(sizeof(AppData));
    
    // Crear ventana principal
    app->main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(app->main_window), "Cuadros Mágicos");
    gtk_window_set_default_size(GTK_WINDOW(app->main_window), 800, 600);
    gtk_container_set_border_width(GTK_CONTAINER(app->main_window), 10);
    
    // Crear layout principal
    GtkWidget *main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(app->main_window), main_box);
    
    // Crear controles
    GtkWidget *controls_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_pack_start(GTK_BOX(main_box), controls_box, FALSE, FALSE, 0);
    
    // Label y SpinButton para tamaño
    GtkWidget *size_label = gtk_label_new("Tamaño del cuadro:");
    gtk_box_pack_start(GTK_BOX(controls_box), size_label, FALSE, FALSE, 0);
    
    app->size_spin = gtk_spin_button_new_with_range(3, 21, 2);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(app->size_spin), 3);
    gtk_box_pack_start(GTK_BOX(controls_box), app->size_spin, FALSE, FALSE, 0);
    
    // Botón crear
    app->create_button = gtk_button_new_with_label("Crear Cuadro Mágico");
    gtk_box_pack_start(GTK_BOX(controls_box), app->create_button, FALSE, FALSE, 0);
    
    // Label de información
    app->info_label = gtk_label_new("Selecciona un tamaño y haz clic en 'Crear Cuadro Mágico'");
    gtk_box_pack_start(GTK_BOX(main_box), app->info_label, FALSE, FALSE, 0);
    
    // Área scrollable para el grid
    GtkWidget *scrolled = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled), 
                                   GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_box_pack_start(GTK_BOX(main_box), scrolled, TRUE, TRUE, 0);
    
    // Grid para el cuadro mágico
    app->grid = gtk_grid_new();
    gtk_grid_set_row_homogeneous(GTK_GRID(app->grid), TRUE);
    gtk_grid_set_column_homogeneous(GTK_GRID(app->grid), TRUE);
    gtk_grid_set_row_spacing(GTK_GRID(app->grid), 2);
    gtk_grid_set_column_spacing(GTK_GRID(app->grid), 2);
    gtk_widget_set_halign(app->grid, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(app->grid, GTK_ALIGN_CENTER);
    gtk_container_add(GTK_CONTAINER(scrolled), app->grid);
    
    // Conectar señales
    g_signal_connect(app->main_window, "destroy", G_CALLBACK(on_window_destroy), NULL);
    g_signal_connect(app->create_button, "clicked", G_CALLBACK(on_create_clicked), app);
    
    // Mostrar todo
    gtk_widget_show_all(app->main_window);
    
    printf("INFO: Interfaz creada exitosamente\n");
    
    // Crear cuadro inicial
    crear_cuadro_magico(app);
    
    // Iniciar loop principal
    gtk_main();
    
    g_free(app);
    return 0;
}


