/*
 * Generador de Cuadros Mágicos - Versión Interactiva
 * Permite llenar paso a paso y ver las sumas parciales en tiempo real
 */

#include <gtk/gtk.h>
#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define MAX_SIZE 21  // Tamaño máximo para la matriz

// Estructura para mantener el estado del programa
typedef struct {
    // Widgets principales
    GtkWidget *main_window;
    GtkWidget *size_spin;
    GtkWidget *create_grid_button;
    GtkWidget *method_frame;
    GtkWidget *fill_frame;
    GtkWidget *start_filling_button;
    GtkWidget *next_step_button;
    GtkWidget *auto_complete_button;
    GtkWidget *reset_button;
    GtkWidget *magic_square_grid;
    GtkWidget *sums_textview;
    GtkWidget *current_number_label;
    GtkWidget *position_label;
    GtkWidget *magic_sum_label;
    GtkWidget *progress_label;
    
    // Radio buttons
    GtkWidget *siames_radio;
    GtkWidget *louber_radio;
    GtkWidget *l_radio;
    GtkWidget *caracol_radio;
    
    // Estado del cuadro mágico
    int matrix[MAX_SIZE][MAX_SIZE];
    int size;
    int current_number;
    int current_row;
    int current_col;
    int total_numbers;
    int magic_sum;
    bool is_filling;
    
    // Método seleccionado
    int selected_method; // 0=siames, 1=louber, 2=l, 3=caracol
    
    // Labels para mostrar números en el grid
    GtkWidget *cell_labels[MAX_SIZE][MAX_SIZE];
    
    // Labels para mostrar sumas
    GtkWidget *row_sum_labels[MAX_SIZE];
    GtkWidget *col_sum_labels[MAX_SIZE];
    GtkWidget *diag1_sum_label;
    GtkWidget *diag2_sum_label;
    
} AppData;

// ============= FUNCIONES DE MOVIMIENTOS =============

// Método Siamés clásico
int metodoSiames(int matriz[MAX_SIZE][MAX_SIZE], int n, int fila, int columna) {
    int nuevaFila = (fila - 1 + n) % n;
    int nuevaColumna = (columna + 1) % n;

    if (matriz[nuevaFila][nuevaColumna] != 0) { 
        nuevaFila = (fila + 1) % n;
        nuevaColumna = columna;
    }

    return nuevaFila * MAX_SIZE + nuevaColumna;
}

// Método en L
int metodoEnL(int matriz[MAX_SIZE][MAX_SIZE], int n, int fila, int columna) {
    int nuevaFila = (fila - 2 + n) % n;
    int nuevaColumna = (columna + 1) % n;
    if (matriz[nuevaFila][nuevaColumna] != 0) {
        nuevaFila = (fila + 1) % n;
        nuevaColumna = columna;
    }
    return nuevaFila * MAX_SIZE + nuevaColumna;
}

// Método De la Loubère
int metodoLouber(int matriz[MAX_SIZE][MAX_SIZE], int n, int fila, int columna) {
    int nuevaFila = (fila + 1) % n;
    int nuevaColumna = (columna - 1 + n) % n;

    if (matriz[nuevaFila][nuevaColumna] != 0) {
        nuevaFila = (fila - 1 + n) % n;
        nuevaColumna = columna;
    }

    return nuevaFila * MAX_SIZE + nuevaColumna;
}

// Método Caracol
int metodoCaracol(int matriz[MAX_SIZE][MAX_SIZE], int n, int fila, int columna) {
    int nuevaFila = (fila + 1) % n;
    int nuevaColumna = (columna + 1) % n;
    
    if (matriz[nuevaFila][nuevaColumna] != 0) {
        nuevaFila = (fila - 1 + n) % n;
        nuevaColumna = (columna - 1 + n) % n;
    }
    
    return nuevaFila * MAX_SIZE + nuevaColumna;
}

// ============= FUNCIONES AUXILIARES =============

// Inicializar la matriz
void inicializar_matriz(AppData *app) {
    for (int i = 0; i < app->size; i++) {
        for (int j = 0; j < app->size; j++) {
            app->matrix[i][j] = 0;
        }
    }
}

// Calcular suma mágica teórica
int calcular_suma_magica(int n) {
    return n * (n * n + 1) / 2;
}

// Obtener posición inicial aleatoria
void obtener_posicion_aleatoria(AppData *app) {
    app->current_row = rand() % app->size;
    app->current_col = rand() % app->size;
}

// Calcular sumas parciales
void calcular_sumas_parciales(AppData *app) {
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(app->sums_textview));
    GString *text = g_string_new("");
    
    // Sumas de filas
    g_string_append(text, "FILAS:\n");
    for (int i = 0; i < app->size; i++) {
        int sum = 0;
        for (int j = 0; j < app->size; j++) {
            sum += app->matrix[i][j];
        }
        g_string_append_printf(text, "Fila %d: %d", i + 1, sum);
        if (sum == app->magic_sum && sum > 0) {
            g_string_append(text, " ✓");
        }
        g_string_append(text, "\n");
    }
    
    // Sumas de columnas
    g_string_append(text, "\nCOLUMNAS:\n");
    for (int j = 0; j < app->size; j++) {
        int sum = 0;
        for (int i = 0; i < app->size; i++) {
            sum += app->matrix[i][j];
        }
        g_string_append_printf(text, "Columna %d: %d", j + 1, sum);
        if (sum == app->magic_sum && sum > 0) {
            g_string_append(text, " ✓");
        }
        g_string_append(text, "\n");
    }
    
    // Diagonal principal
    g_string_append(text, "\nDIAGONALES:\n");
    int diag1_sum = 0;
    for (int i = 0; i < app->size; i++) {
        diag1_sum += app->matrix[i][i];
    }
    g_string_append_printf(text, "Diagonal principal: %d", diag1_sum);
    if (diag1_sum == app->magic_sum && diag1_sum > 0) {
        g_string_append(text, " ✓");
    }
    g_string_append(text, "\n");
    
    // Diagonal secundaria
    int diag2_sum = 0;
    for (int i = 0; i < app->size; i++) {
        diag2_sum += app->matrix[i][app->size - 1 - i];
    }
    g_string_append_printf(text, "Diagonal secundaria: %d", diag2_sum);
    if (diag2_sum == app->magic_sum && diag2_sum > 0) {
        g_string_append(text, " ✓");
    }
    g_string_append(text, "\n");
    
    gtk_text_buffer_set_text(buffer, text->str, -1);
    g_string_free(text, TRUE);
}

// Actualizar el display del cuadro
void actualizar_display_cuadro(AppData *app) {
    for (int i = 0; i < app->size; i++) {
        for (int j = 0; j < app->size; j++) {
            if (app->matrix[i][j] == 0) {
                gtk_label_set_text(GTK_LABEL(app->cell_labels[i][j]), "");
                gtk_widget_set_name(app->cell_labels[i][j], "empty-cell");
            } else {
                char text[10];
                snprintf(text, sizeof(text), "%d", app->matrix[i][j]);
                gtk_label_set_text(GTK_LABEL(app->cell_labels[i][j]), text);
                
                // Resaltar la celda actual
                if (i == app->current_row && j == app->current_col) {
                    gtk_widget_set_name(app->cell_labels[i][j], "current-cell");
                } else {
                    gtk_widget_set_name(app->cell_labels[i][j], "filled-cell");
                }
            }
        }
    }
    
    // Actualizar sumas parciales
    calcular_sumas_parciales(app);
}

// ============= CALLBACKS =============

// Crear el grid del cuadro mágico
void on_create_grid_button_clicked(GtkButton *button, AppData *app) {
    app->size = (int)gtk_spin_button_get_value(GTK_SPIN_BUTTON(app->size_spin));
    
    // Verificar que sea impar
    if (app->size % 2 == 0) {
        app->size++;
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(app->size_spin), app->size);
    }
    
    // Limpiar grid anterior
    gtk_container_foreach(GTK_CONTAINER(app->magic_square_grid), 
                         (GtkCallback)gtk_widget_destroy, NULL);
    
    // Calcular la suma mágica
    app->magic_sum = calcular_suma_magica(app->size);
    app->total_numbers = app->size * app->size;
    
    // Crear nuevas celdas
    for (int i = 0; i < app->size; i++) {
        for (int j = 0; j < app->size; j++) {
            // Crear frame para la celda
            GtkWidget *frame = gtk_frame_new(NULL);
            gtk_frame_set_shadow_type(GTK_FRAME(frame), GTK_SHADOW_IN);
            
            // Crear label para el número
            app->cell_labels[i][j] = gtk_label_new("");
            gtk_widget_set_size_request(app->cell_labels[i][j], 40, 40);
            gtk_label_set_justify(GTK_LABEL(app->cell_labels[i][j]), GTK_JUSTIFY_CENTER);
            
            gtk_container_add(GTK_CONTAINER(frame), app->cell_labels[i][j]);
            gtk_grid_attach(GTK_GRID(app->magic_square_grid), frame, j, i, 1, 1);
        }
    }
    
    // Inicializar matriz
    inicializar_matriz(app);
    
    // Actualizar labels
    char text[100];
    snprintf(text, sizeof(text), "Suma mágica objetivo: %d", app->magic_sum);
    gtk_label_set_text(GTK_LABEL(app->magic_sum_label), text);
    
    snprintf(text, sizeof(text), "Progreso: 0/%d números", app->total_numbers);
    gtk_label_set_text(GTK_LABEL(app->progress_label), text);
    
    // Mostrar todo
    gtk_widget_show_all(app->magic_square_grid);
    
    // Habilitar siguiente paso
    gtk_widget_set_sensitive(app->method_frame, TRUE);
    gtk_widget_set_sensitive(app->start_filling_button, TRUE);
    gtk_widget_set_sensitive(app->create_grid_button, FALSE);
    
    actualizar_display_cuadro(app);
}

// Iniciar el proceso de llenado
void on_start_filling_button_clicked(GtkButton *button, AppData *app) {
    // Determinar método seleccionado
    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(app->siames_radio))) {
        app->selected_method = 0;
    } else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(app->louber_radio))) {
        app->selected_method = 1;
    } else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(app->l_radio))) {
        app->selected_method = 2;
    } else {
        app->selected_method = 3;
    }
    
    // Obtener posición inicial aleatoria
    obtener_posicion_aleatoria(app);
    
    // Inicializar estado
    app->current_number = 1;
    app->is_filling = TRUE;
    
    // Actualizar UI
    char text[100];
    snprintf(text, sizeof(text), "Posición inicial: (%d,%d)", 
             app->current_row + 1, app->current_col + 1);
    gtk_label_set_text(GTK_LABEL(app->position_label), text);
    
    snprintf(text, sizeof(text), "<b>Próximo número: %d</b>", app->current_number);
    gtk_label_set_markup(GTK_LABEL(app->current_number_label), text);
    
    // Habilitar controles de llenado
    gtk_widget_set_sensitive(app->fill_frame, TRUE);
    gtk_widget_set_sensitive(app->next_step_button, TRUE);
    gtk_widget_set_sensitive(app->auto_complete_button, TRUE);
    gtk_widget_set_sensitive(app->start_filling_button, FALSE);
    gtk_widget_set_sensitive(app->method_frame, FALSE);
    
    actualizar_display_cuadro(app);
}

// Siguiente paso en el llenado
void on_next_step_button_clicked(GtkButton *button, AppData *app) {
    if (!app->is_filling) return;
    
    // Colocar número actual
    app->matrix[app->current_row][app->current_col] = app->current_number;
    
    // Actualizar progreso
    char text[100];
    snprintf(text, sizeof(text), "Progreso: %d/%d números", 
             app->current_number, app->total_numbers);
    gtk_label_set_text(GTK_LABEL(app->progress_label), text);
    
    // Verificar si terminamos
    if (app->current_number >= app->total_numbers) {
        app->is_filling = FALSE;
        gtk_widget_set_sensitive(app->next_step_button, FALSE);
        gtk_widget_set_sensitive(app->auto_complete_button, FALSE);
        
        gtk_label_set_markup(GTK_LABEL(app->current_number_label), 
                            "<b>¡Cuadro Mágico Completado!</b>");
        
        actualizar_display_cuadro(app);
        return;
    }
    
    // Calcular siguiente posición
    int next_pos;
    switch (app->selected_method) {
        case 0:
            next_pos = metodoSiames(app->matrix, app->size, app->current_row, app->current_col);
            break;
        case 1:
            next_pos = metodoLouber(app->matrix, app->size, app->current_row, app->current_col);
            break;
        case 2:
            next_pos = metodoEnL(app->matrix, app->size, app->current_row, app->current_col);
            break;
        case 3:
            next_pos = metodoCaracol(app->matrix, app->size, app->current_row, app->current_col);
            break;
        default:
            next_pos = metodoSiames(app->matrix, app->size, app->current_row, app->current_col);
            break;
    }
    
    app->current_row = next_pos / MAX_SIZE;
    app->current_col = next_pos % MAX_SIZE;
    app->current_number++;
    
    // Actualizar labels
    snprintf(text, sizeof(text), "<b>Próximo número: %d</b>", app->current_number);
    gtk_label_set_markup(GTK_LABEL(app->current_number_label), text);
    
    snprintf(text, sizeof(text), "Próxima posición: (%d,%d)", 
             app->current_row + 1, app->current_col + 1);
    gtk_label_set_text(GTK_LABEL(app->position_label), text);
    
    actualizar_display_cuadro(app);
}

// Completar automáticamente
void on_auto_complete_button_clicked(GtkButton *button, AppData *app) {
    while (app->is_filling && app->current_number <= app->total_numbers) {
        on_next_step_button_clicked(NULL, app);
        
        // Pequeña pausa para ver el progreso
        while (gtk_events_pending()) {
            gtk_main_iteration();
        }
        g_usleep(100000); // 100ms
    }
}

// Reiniciar todo
void on_reset_button_clicked(GtkButton *button, AppData *app) {
    // Limpiar grid
    gtk_container_foreach(GTK_CONTAINER(app->magic_square_grid), 
                         (GtkCallback)gtk_widget_destroy, NULL);
    
    // Resetear estado
    app->is_filling = FALSE;
    app->current_number = 1;
    
    // Resetear UI
    gtk_widget_set_sensitive(app->create_grid_button, TRUE);
    gtk_widget_set_sensitive(app->method_frame, FALSE);
    gtk_widget_set_sensitive(app->fill_frame, FALSE);
    
    gtk_label_set_text(GTK_LABEL(app->magic_sum_label), "Suma mágica objetivo: -");
    gtk_label_set_text(GTK_LABEL(app->progress_label), "Progreso: 0/0 números");
    gtk_label_set_markup(GTK_LABEL(app->current_number_label), "<b>Próximo número: 1</b>");
    gtk_label_set_text(GTK_LABEL(app->position_label), "Posición inicial: (?,?)");
    
    // Limpiar texto de sumas
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(app->sums_textview));
    gtk_text_buffer_set_text(buffer, "Crea un cuadro para ver las sumas parciales...", -1);
}

// Cerrar ventana
void on_main_window_destroy(GtkWidget *widget, gpointer data) {
    gtk_main_quit();
}

// ============= FUNCIÓN PRINCIPAL =============

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);
    srand(time(NULL));
    
    // Crear estructura de datos
    AppData *app = g_malloc(sizeof(AppData));
    
    // Crear builder
    GtkBuilder *builder = gtk_builder_new();
    GError *error = NULL;
    
    if (!gtk_builder_add_from_file(builder, "cuadros_magicos_interactivo.glade", &error)) {
        g_printerr("Error cargando UI: %s\n", error->message);
        g_printerr("Asegúrese de que el archivo cuadros_magicos_interactivo.glade esté en el directorio actual\n");
        g_clear_error(&error);
        g_object_unref(builder);
        g_free(app);
        return 1;
    }
    
    // Obtener widgets
    app->main_window = GTK_WIDGET(gtk_builder_get_object(builder, "main_window"));
    app->size_spin = GTK_WIDGET(gtk_builder_get_object(builder, "size_spin"));
    app->create_grid_button = GTK_WIDGET(gtk_builder_get_object(builder, "create_grid_button"));
    app->method_frame = GTK_WIDGET(gtk_builder_get_object(builder, "method_frame"));
    app->fill_frame = GTK_WIDGET(gtk_builder_get_object(builder, "fill_frame"));
    app->start_filling_button = GTK_WIDGET(gtk_builder_get_object(builder, "start_filling_button"));
    app->next_step_button = GTK_WIDGET(gtk_builder_get_object(builder, "next_step_button"));
    app->auto_complete_button = GTK_WIDGET(gtk_builder_get_object(builder, "auto_complete_button"));
    app->reset_button = GTK_WIDGET(gtk_builder_get_object(builder, "reset_button"));
    app->magic_square_grid = GTK_WIDGET(gtk_builder_get_object(builder, "magic_square_grid"));
    app->sums_textview = GTK_WIDGET(gtk_builder_get_object(builder, "sums_textview"));
    app->current_number_label = GTK_WIDGET(gtk_builder_get_object(builder, "current_number_label"));
    app->position_label = GTK_WIDGET(gtk_builder_get_object(builder, "position_label"));
    app->magic_sum_label = GTK_WIDGET(gtk_builder_get_object(builder, "magic_sum_label"));
    app->progress_label = GTK_WIDGET(gtk_builder_get_object(builder, "progress_label"));
    
    app->siames_radio = GTK_WIDGET(gtk_builder_get_object(builder, "siames_radio"));
    app->louber_radio = GTK_WIDGET(gtk_builder_get_object(builder, "louber_radio"));
    app->l_radio = GTK_WIDGET(gtk_builder_get_object(builder, "l_radio"));
    app->caracol_radio = GTK_WIDGET(gtk_builder_get_object(builder, "caracol_radio"));
    
    // Conectar señales
    g_signal_connect(app->main_window, "destroy", G_CALLBACK(on_main_window_destroy), NULL);
    g_signal_connect(app->create_grid_button, "clicked", G_CALLBACK(on_create_grid_button_clicked), app);
    g_signal_connect(app->start_filling_button, "clicked", G_CALLBACK(on_start_filling_button_clicked), app);
    g_signal_connect(app->next_step_button, "clicked", G_CALLBACK(on_next_step_button_clicked), app);
    g_signal_connect(app->auto_complete_button, "clicked", G_CALLBACK(on_auto_complete_button_clicked), app);
    g_signal_connect(app->reset_button, "clicked", G_CALLBACK(on_reset_button_clicked), app);
    
    // Agregar CSS para estilos (compatible con Linux)
    GtkCssProvider *css_provider = gtk_css_provider_new();
    const char *css_data = 
        "label#empty-cell { background-color: #f0f0f0; border: 1px solid #ccc; }"
        "label#filled-cell { background-color: #e8f4fd; border: 1px solid #0078d4; }"
        "label#current-cell { background-color: #ffeb3b; border: 2px solid #ff9800; font-weight: bold; }";
    
    GError *css_error = NULL;
    if (!gtk_css_provider_load_from_data(css_provider, css_data, -1, &css_error)) {
        if (css_error) {
            g_warning("Error cargando CSS: %s", css_error->message);
            g_clear_error(&css_error);
        }
    } else {
        gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
                                                 GTK_STYLE_PROVIDER(css_provider),
                                                 GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    }
    g_object_unref(css_provider);
    
    // Inicializar texto de sumas
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(app->sums_textview));
    gtk_text_buffer_set_text(buffer, "Crea un cuadro para ver las sumas parciales...", -1);
    
    // Mostrar ventana
    gtk_widget_show_all(app->main_window);
    
    // Iniciar loop principal
    gtk_main();
    
    // Limpiar
    g_object_unref(builder);
    g_free(app);
    
    return 0;
}

