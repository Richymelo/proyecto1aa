/*
 * Generador de Cuadros Mágicos - Versión Interactiva CORREGIDA
 * Con mejores verificaciones para evitar segmentation faults
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

// Calcular sumas parciales (con verificación de widgets)
void calcular_sumas_parciales(AppData *app) {
    if (!app || !app->sums_textview) {
        printf("ERROR: sums_textview es NULL en calcular_sumas_parciales\n");
        return;
    }
    
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(app->sums_textview));
    if (!buffer) {
        printf("ERROR: No se pudo obtener el buffer del textview\n");
        return;
    }
    
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
    if (!app) {
        printf("ERROR: app es NULL en actualizar_display_cuadro\n");
        return;
    }
    
    for (int i = 0; i < app->size; i++) {
        for (int j = 0; j < app->size; j++) {
            if (app->cell_labels[i][j]) {
                if (app->matrix[i][j] == 0) {
                    gtk_label_set_text(GTK_LABEL(app->cell_labels[i][j]), "");
                } else {
                    char text[10];
                    snprintf(text, sizeof(text), "%d", app->matrix[i][j]);
                    gtk_label_set_text(GTK_LABEL(app->cell_labels[i][j]), text);
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
    if (!app || !app->size_spin) return;
    
    app->size = (int)gtk_spin_button_get_value(GTK_SPIN_BUTTON(app->size_spin));
    
    // Verificar que sea impar
    if (app->size % 2 == 0) {
        app->size++;
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(app->size_spin), app->size);
    }
    
    // Limpiar grid anterior
    if (app->magic_square_grid) {
        gtk_container_foreach(GTK_CONTAINER(app->magic_square_grid), 
                             (GtkCallback)gtk_widget_destroy, NULL);
    }
    
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
    if (app->magic_sum_label) {
        char text[100];
        snprintf(text, sizeof(text), "Suma mágica objetivo: %d", app->magic_sum);
        gtk_label_set_text(GTK_LABEL(app->magic_sum_label), text);
    }
    
    if (app->progress_label) {
        char text[100];
        snprintf(text, sizeof(text), "Progreso: 0/%d números", app->total_numbers);
        gtk_label_set_text(GTK_LABEL(app->progress_label), text);
    }
    
    // Mostrar todo
    if (app->magic_square_grid) {
        gtk_widget_show_all(app->magic_square_grid);
    }
    
    // Habilitar siguiente paso
    if (app->method_frame) {
        gtk_widget_set_sensitive(app->method_frame, TRUE);
    }
    if (app->start_filling_button) {
        gtk_widget_set_sensitive(app->start_filling_button, TRUE);
    }
    if (app->create_grid_button) {
        gtk_widget_set_sensitive(app->create_grid_button, FALSE);
    }
    
    actualizar_display_cuadro(app);
}

// Iniciar el proceso de llenado
void on_start_filling_button_clicked(GtkButton *button, AppData *app) {
    if (!app) return;
    
    // Determinar método seleccionado
    if (app->siames_radio && gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(app->siames_radio))) {
        app->selected_method = 0;
    } else if (app->louber_radio && gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(app->louber_radio))) {
        app->selected_method = 1;
    } else if (app->l_radio && gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(app->l_radio))) {
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
    if (app->position_label) {
        char text[100];
        snprintf(text, sizeof(text), "Posición inicial: (%d,%d)", 
                 app->current_row + 1, app->current_col + 1);
        gtk_label_set_text(GTK_LABEL(app->position_label), text);
    }
    
    if (app->current_number_label) {
        char text[100];
        snprintf(text, sizeof(text), "<b>Próximo número: %d</b>", app->current_number);
        gtk_label_set_markup(GTK_LABEL(app->current_number_label), text);
    }
    
    // Habilitar controles de llenado
    if (app->fill_frame) {
        gtk_widget_set_sensitive(app->fill_frame, TRUE);
    }
    if (app->next_step_button) {
        gtk_widget_set_sensitive(app->next_step_button, TRUE);
    }
    if (app->auto_complete_button) {
        gtk_widget_set_sensitive(app->auto_complete_button, TRUE);
    }
    if (app->start_filling_button) {
        gtk_widget_set_sensitive(app->start_filling_button, FALSE);
    }
    if (app->method_frame) {
        gtk_widget_set_sensitive(app->method_frame, FALSE);
    }
    
    actualizar_display_cuadro(app);
}

// Siguiente paso en el llenado
void on_next_step_button_clicked(GtkButton *button, AppData *app) {
    if (!app || !app->is_filling) return;
    
    // Colocar número actual
    app->matrix[app->current_row][app->current_col] = app->current_number;
    
    // Actualizar progreso
    if (app->progress_label) {
        char text[100];
        snprintf(text, sizeof(text), "Progreso: %d/%d números", 
                 app->current_number, app->total_numbers);
        gtk_label_set_text(GTK_LABEL(app->progress_label), text);
    }
    
    // Verificar si terminamos
    if (app->current_number >= app->total_numbers) {
        app->is_filling = FALSE;
        if (app->next_step_button) {
            gtk_widget_set_sensitive(app->next_step_button, FALSE);
        }
        if (app->auto_complete_button) {
            gtk_widget_set_sensitive(app->auto_complete_button, FALSE);
        }
        
        if (app->current_number_label) {
            gtk_label_set_markup(GTK_LABEL(app->current_number_label), 
                                "<b>¡Cuadro Mágico Completado!</b>");
        }
        
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
    if (app->current_number_label) {
        char text[100];
        snprintf(text, sizeof(text), "<b>Próximo número: %d</b>", app->current_number);
        gtk_label_set_markup(GTK_LABEL(app->current_number_label), text);
    }
    
    if (app->position_label) {
        char text[100];
        snprintf(text, sizeof(text), "Próxima posición: (%d,%d)", 
                 app->current_row + 1, app->current_col + 1);
        gtk_label_set_text(GTK_LABEL(app->position_label), text);
    }
    
    actualizar_display_cuadro(app);
}

// Completar automáticamente
void on_auto_complete_button_clicked(GtkButton *button, AppData *app) {
    if (!app) return;
    
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
    if (!app) return;
    
    // Limpiar grid
    if (app->magic_square_grid) {
        gtk_container_foreach(GTK_CONTAINER(app->magic_square_grid), 
                             (GtkCallback)gtk_widget_destroy, NULL);
    }
    
    // Resetear estado
    app->is_filling = FALSE;
    app->current_number = 1;
    
    // Resetear UI
    if (app->create_grid_button) {
        gtk_widget_set_sensitive(app->create_grid_button, TRUE);
    }
    if (app->method_frame) {
        gtk_widget_set_sensitive(app->method_frame, FALSE);
    }
    if (app->fill_frame) {
        gtk_widget_set_sensitive(app->fill_frame, FALSE);
    }
    
    if (app->magic_sum_label) {
        gtk_label_set_text(GTK_LABEL(app->magic_sum_label), "Suma mágica objetivo: -");
    }
    if (app->progress_label) {
        gtk_label_set_text(GTK_LABEL(app->progress_label), "Progreso: 0/0 números");
    }
    if (app->current_number_label) {
        gtk_label_set_markup(GTK_LABEL(app->current_number_label), "<b>Próximo número: 1</b>");
    }
    if (app->position_label) {
        gtk_label_set_text(GTK_LABEL(app->position_label), "Posición inicial: (?,?)");
    }
    
    // Limpiar texto de sumas
    if (app->sums_textview) {
        GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(app->sums_textview));
        if (buffer) {
            gtk_text_buffer_set_text(buffer, "Crea un cuadro para ver las sumas parciales...", -1);
        }
    }
}

// Cerrar ventana
void on_main_window_destroy(GtkWidget *widget, gpointer data) {
    gtk_main_quit();
}

// ============= FUNCIÓN PRINCIPAL =============

int main(int argc, char *argv[]) {
    printf("INFO: Iniciando Cuadros Mágicos Interactivos (Versión Corregida)\n");
    
    gtk_init(&argc, &argv);
    srand(time(NULL));
    
    // Crear estructura de datos
    AppData *app = g_malloc0(sizeof(AppData)); // g_malloc0 inicializa con zeros
    if (!app) {
        g_printerr("ERROR: No se pudo asignar memoria para AppData\n");
        return 1;
    }
    
    printf("INFO: Memoria asignada correctamente\n");
    
    // Crear builder
    GtkBuilder *builder = gtk_builder_new();
    GError *error = NULL;
    
    printf("INFO: Intentando cargar archivo cuadros_magicos_interactivo.glade\n");
    
    if (!gtk_builder_add_from_file(builder, "cuadros_magicos_interactivo.glade", &error)) {
        g_printerr("ERROR cargando UI: %s\n", error->message);
        g_printerr("Asegúrese de que el archivo cuadros_magicos_interactivo.glade esté en el directorio actual\n");
        g_printerr("Directorio actual: %s\n", g_get_current_dir());
        g_clear_error(&error);
        g_object_unref(builder);
        g_free(app);
        return 1;
    }
    
    printf("INFO: Archivo .glade cargado exitosamente\n");
    
    // Función auxiliar para obtener widgets con verificación
    #define GET_WIDGET(name) do { \
        app->name = GTK_WIDGET(gtk_builder_get_object(builder, #name)); \
        if (!app->name) { \
            g_printerr("WARNING: Widget '%s' no encontrado en el archivo .glade\n", #name); \
        } else { \
            printf("INFO: Widget '%s' cargado correctamente\n", #name); \
        } \
    } while(0)
    
    // Obtener widgets principales (críticos)
    GET_WIDGET(main_window);
    if (!app->main_window) {
        g_printerr("ERROR CRÍTICO: No se encontró main_window\n");
        g_object_unref(builder);
        g_free(app);
        return 1;
    }
    
    // Obtener resto de widgets (con verificaciones)
    GET_WIDGET(size_spin);
    GET_WIDGET(create_grid_button);
    GET_WIDGET(method_frame);
    GET_WIDGET(fill_frame);
    GET_WIDGET(start_filling_button);
    GET_WIDGET(next_step_button);
    GET_WIDGET(auto_complete_button);
    GET_WIDGET(reset_button);
    GET_WIDGET(magic_square_grid);
    GET_WIDGET(sums_textview);
    GET_WIDGET(current_number_label);
    GET_WIDGET(position_label);
    GET_WIDGET(magic_sum_label);
    GET_WIDGET(progress_label);
    GET_WIDGET(siames_radio);
    GET_WIDGET(louber_radio);
    GET_WIDGET(l_radio);
    GET_WIDGET(caracol_radio);
    
    printf("INFO: Todos los widgets procesados\n");
    
    // Conectar señales (solo si los widgets existen)
    g_signal_connect(app->main_window, "destroy", G_CALLBACK(on_main_window_destroy), NULL);
    
    if (app->create_grid_button) {
        g_signal_connect(app->create_grid_button, "clicked", G_CALLBACK(on_create_grid_button_clicked), app);
    }
    if (app->start_filling_button) {
        g_signal_connect(app->start_filling_button, "clicked", G_CALLBACK(on_start_filling_button_clicked), app);
    }
    if (app->next_step_button) {
        g_signal_connect(app->next_step_button, "clicked", G_CALLBACK(on_next_step_button_clicked), app);
    }
    if (app->auto_complete_button) {
        g_signal_connect(app->auto_complete_button, "clicked", G_CALLBACK(on_auto_complete_button_clicked), app);
    }
    if (app->reset_button) {
        g_signal_connect(app->reset_button, "clicked", G_CALLBACK(on_reset_button_clicked), app);
    }
    
    // Inicializar texto de sumas
    if (app->sums_textview) {
        GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(app->sums_textview));
        if (buffer) {
            gtk_text_buffer_set_text(buffer, "Crea un cuadro para ver las sumas parciales...", -1);
        }
    }
    
    // Mostrar ventana
    printf("INFO: Mostrando ventana principal\n");
    gtk_widget_show_all(app->main_window);
    
    // Iniciar loop principal
    printf("INFO: Iniciando loop principal de GTK\n");
    gtk_main();
    
    printf("INFO: Aplicación terminada correctamente\n");
    
    // Limpiar
    g_object_unref(builder);
    g_free(app);
    
    return 0;
}



