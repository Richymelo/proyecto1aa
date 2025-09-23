/*
 * Versión Debug de Cuadros Mágicos Interactivos
 * Con verificaciones de widgets para detectar errores de segmentación
 */

#include <gtk/gtk.h>
#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define MAX_SIZE 21

typedef struct {
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
    GtkWidget *siames_radio;
    GtkWidget *louber_radio;
    GtkWidget *l_radio;
    GtkWidget *caracol_radio;
    
    int matrix[MAX_SIZE][MAX_SIZE];
    int size;
    int current_number;
    int current_row;
    int current_col;
    int total_numbers;
    int magic_sum;
    bool is_filling;
    int selected_method;
    GtkWidget *cell_labels[MAX_SIZE][MAX_SIZE];
} AppData;

// ============= FUNCIONES DE MOVIMIENTOS =============

int metodoSiames(int matriz[MAX_SIZE][MAX_SIZE], int n, int fila, int columna) {
    int nuevaFila = (fila - 1 + n) % n;
    int nuevaColumna = (columna + 1) % n;
    if (matriz[nuevaFila][nuevaColumna] != 0) { 
        nuevaFila = (fila + 1) % n;
        nuevaColumna = columna;
    }
    return nuevaFila * MAX_SIZE + nuevaColumna;
}

int metodoEnL(int matriz[MAX_SIZE][MAX_SIZE], int n, int fila, int columna) {
    int nuevaFila = (fila - 2 + n) % n;
    int nuevaColumna = (columna + 1) % n;
    if (matriz[nuevaFila][nuevaColumna] != 0) {
        nuevaFila = (fila + 1) % n;
        nuevaColumna = columna;
    }
    return nuevaFila * MAX_SIZE + nuevaColumna;
}

int metodoLouber(int matriz[MAX_SIZE][MAX_SIZE], int n, int fila, int columna) {
    int nuevaFila = (fila + 1) % n;
    int nuevaColumna = (columna - 1 + n) % n;
    if (matriz[nuevaFila][nuevaColumna] != 0) {
        nuevaFila = (fila - 1 + n) % n;
        nuevaColumna = columna;
    }
    return nuevaFila * MAX_SIZE + nuevaColumna;
}

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

void inicializar_matriz(AppData *app) {
    for (int i = 0; i < app->size; i++) {
        for (int j = 0; j < app->size; j++) {
            app->matrix[i][j] = 0;
        }
    }
}

int calcular_suma_magica(int n) {
    return n * (n * n + 1) / 2;
}

void obtener_posicion_aleatoria(AppData *app) {
    app->current_row = rand() % app->size;
    app->current_col = rand() % app->size;
}

void calcular_sumas_parciales(AppData *app) {
    if (!app->sums_textview) {
        printf("ERROR: sums_textview es NULL\n");
        return;
    }
    
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(app->sums_textview));
    if (!buffer) {
        printf("ERROR: No se pudo obtener buffer del textview\n");
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
    
    // Diagonales
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

void actualizar_display_cuadro(AppData *app) {
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
    calcular_sumas_parciales(app);
}

// ============= CALLBACKS =============

void on_create_grid_button_clicked(GtkButton *button, AppData *app) {
    printf("DEBUG: on_create_grid_button_clicked iniciado\n");
    
    app->size = (int)gtk_spin_button_get_value(GTK_SPIN_BUTTON(app->size_spin));
    if (app->size % 2 == 0) {
        app->size++;
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(app->size_spin), app->size);
    }
    
    printf("DEBUG: Tamaño seleccionado: %d\n", app->size);
    
    // Limpiar grid anterior
    gtk_container_foreach(GTK_CONTAINER(app->magic_square_grid), 
                         (GtkCallback)gtk_widget_destroy, NULL);
    
    app->magic_sum = calcular_suma_magica(app->size);
    app->total_numbers = app->size * app->size;
    
    printf("DEBUG: Suma mágica: %d, Total números: %d\n", app->magic_sum, app->total_numbers);
    
    // Crear nuevas celdas
    for (int i = 0; i < app->size; i++) {
        for (int j = 0; j < app->size; j++) {
            GtkWidget *frame = gtk_frame_new(NULL);
            gtk_frame_set_shadow_type(GTK_FRAME(frame), GTK_SHADOW_IN);
            
            app->cell_labels[i][j] = gtk_label_new("");
            gtk_widget_set_size_request(app->cell_labels[i][j], 40, 40);
            gtk_label_set_justify(GTK_LABEL(app->cell_labels[i][j]), GTK_JUSTIFY_CENTER);
            
            gtk_container_add(GTK_CONTAINER(frame), app->cell_labels[i][j]);
            gtk_grid_attach(GTK_GRID(app->magic_square_grid), frame, j, i, 1, 1);
        }
    }
    
    inicializar_matriz(app);
    
    // Actualizar labels
    char text[100];
    snprintf(text, sizeof(text), "Suma mágica objetivo: %d", app->magic_sum);
    gtk_label_set_text(GTK_LABEL(app->magic_sum_label), text);
    
    snprintf(text, sizeof(text), "Progreso: 0/%d números", app->total_numbers);
    gtk_label_set_text(GTK_LABEL(app->progress_label), text);
    
    gtk_widget_show_all(app->magic_square_grid);
    
    gtk_widget_set_sensitive(app->method_frame, TRUE);
    gtk_widget_set_sensitive(app->start_filling_button, TRUE);
    gtk_widget_set_sensitive(app->create_grid_button, FALSE);
    
    actualizar_display_cuadro(app);
    
    printf("DEBUG: on_create_grid_button_clicked completado\n");
}

void on_start_filling_button_clicked(GtkButton *button, AppData *app) {
    printf("DEBUG: on_start_filling_button_clicked iniciado\n");
    
    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(app->siames_radio))) {
        app->selected_method = 0;
    } else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(app->louber_radio))) {
        app->selected_method = 1;
    } else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(app->l_radio))) {
        app->selected_method = 2;
    } else {
        app->selected_method = 3;
    }
    
    printf("DEBUG: Método seleccionado: %d\n", app->selected_method);
    
    obtener_posicion_aleatoria(app);
    printf("DEBUG: Posición inicial: (%d,%d)\n", app->current_row, app->current_col);
    
    app->current_number = 1;
    app->is_filling = TRUE;
    
    char text[100];
    snprintf(text, sizeof(text), "Posición inicial: (%d,%d)", 
             app->current_row + 1, app->current_col + 1);
    gtk_label_set_text(GTK_LABEL(app->position_label), text);
    
    snprintf(text, sizeof(text), "<b>Próximo número: %d</b>", app->current_number);
    gtk_label_set_markup(GTK_LABEL(app->current_number_label), text);
    
    gtk_widget_set_sensitive(app->fill_frame, TRUE);
    gtk_widget_set_sensitive(app->next_step_button, TRUE);
    gtk_widget_set_sensitive(app->auto_complete_button, TRUE);
    gtk_widget_set_sensitive(app->start_filling_button, FALSE);
    gtk_widget_set_sensitive(app->method_frame, FALSE);
    
    actualizar_display_cuadro(app);
    
    printf("DEBUG: on_start_filling_button_clicked completado\n");
}

void on_next_step_button_clicked(GtkButton *button, AppData *app) {
    printf("DEBUG: on_next_step_button_clicked - número %d en (%d,%d)\n", 
           app->current_number, app->current_row, app->current_col);
    
    if (!app->is_filling) return;
    
    app->matrix[app->current_row][app->current_col] = app->current_number;
    
    char text[100];
    snprintf(text, sizeof(text), "Progreso: %d/%d números", 
             app->current_number, app->total_numbers);
    gtk_label_set_text(GTK_LABEL(app->progress_label), text);
    
    if (app->current_number >= app->total_numbers) {
        app->is_filling = FALSE;
        gtk_widget_set_sensitive(app->next_step_button, FALSE);
        gtk_widget_set_sensitive(app->auto_complete_button, FALSE);
        gtk_label_set_markup(GTK_LABEL(app->current_number_label), 
                            "<b>¡Cuadro Mágico Completado!</b>");
        actualizar_display_cuadro(app);
        printf("DEBUG: Cuadro completado\n");
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
    
    snprintf(text, sizeof(text), "<b>Próximo número: %d</b>", app->current_number);
    gtk_label_set_markup(GTK_LABEL(app->current_number_label), text);
    
    snprintf(text, sizeof(text), "Próxima posición: (%d,%d)", 
             app->current_row + 1, app->current_col + 1);
    gtk_label_set_text(GTK_LABEL(app->position_label), text);
    
    actualizar_display_cuadro(app);
}

void on_auto_complete_button_clicked(GtkButton *button, AppData *app) {
    printf("DEBUG: Auto-completar iniciado\n");
    while (app->is_filling && app->current_number <= app->total_numbers) {
        on_next_step_button_clicked(NULL, app);
        while (gtk_events_pending()) {
            gtk_main_iteration();
        }
        g_usleep(50000); // 50ms
    }
    printf("DEBUG: Auto-completar terminado\n");
}

void on_reset_button_clicked(GtkButton *button, AppData *app) {
    printf("DEBUG: Reinicio iniciado\n");
    
    gtk_container_foreach(GTK_CONTAINER(app->magic_square_grid), 
                         (GtkCallback)gtk_widget_destroy, NULL);
    
    app->is_filling = FALSE;
    app->current_number = 1;
    
    gtk_widget_set_sensitive(app->create_grid_button, TRUE);
    gtk_widget_set_sensitive(app->method_frame, FALSE);
    gtk_widget_set_sensitive(app->fill_frame, FALSE);
    
    gtk_label_set_text(GTK_LABEL(app->magic_sum_label), "Suma mágica objetivo: -");
    gtk_label_set_text(GTK_LABEL(app->progress_label), "Progreso: 0/0 números");
    gtk_label_set_markup(GTK_LABEL(app->current_number_label), "<b>Próximo número: 1</b>");
    gtk_label_set_text(GTK_LABEL(app->position_label), "Posición inicial: (?,?)");
    
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(app->sums_textview));
    gtk_text_buffer_set_text(buffer, "Crea un cuadro para ver las sumas parciales...", -1);
    
    printf("DEBUG: Reinicio completado\n");
}

void on_main_window_destroy(GtkWidget *widget, gpointer data) {
    printf("DEBUG: Cerrando aplicación\n");
    gtk_main_quit();
}

// ============= FUNCIÓN PRINCIPAL =============

int main(int argc, char *argv[]) {
    printf("DEBUG: Iniciando aplicación\n");
    
    gtk_init(&argc, &argv);
    srand(time(NULL));
    
    AppData *app = g_malloc(sizeof(AppData));
    if (!app) {
        printf("ERROR: No se pudo asignar memoria para AppData\n");
        return 1;
    }
    
    printf("DEBUG: Memoria asignada correctamente\n");
    
    GtkBuilder *builder = gtk_builder_new();
    GError *error = NULL;
    
    printf("DEBUG: Intentando cargar archivo .glade\n");
    
    if (!gtk_builder_add_from_file(builder, "cuadros_magicos_interactivo.glade", &error)) {
        g_printerr("ERROR cargando UI: %s\n", error->message);
        g_printerr("Asegúrese de que el archivo cuadros_magicos_interactivo.glade esté en el directorio actual\n");
        g_clear_error(&error);
        g_object_unref(builder);
        g_free(app);
        return 1;
    }
    
    printf("DEBUG: Archivo .glade cargado exitosamente\n");
    
    // Obtener widgets con verificación
    printf("DEBUG: Obteniendo widgets...\n");
    
    app->main_window = GTK_WIDGET(gtk_builder_get_object(builder, "main_window"));
    if (!app->main_window) { printf("ERROR: main_window es NULL\n"); return 1; }
    
    app->size_spin = GTK_WIDGET(gtk_builder_get_object(builder, "size_spin"));
    if (!app->size_spin) { printf("ERROR: size_spin es NULL\n"); return 1; }
    
    app->create_grid_button = GTK_WIDGET(gtk_builder_get_object(builder, "create_grid_button"));
    if (!app->create_grid_button) { printf("ERROR: create_grid_button es NULL\n"); return 1; }
    
    app->method_frame = GTK_WIDGET(gtk_builder_get_object(builder, "method_frame"));
    if (!app->method_frame) { printf("ERROR: method_frame es NULL\n"); return 1; }
    
    app->fill_frame = GTK_WIDGET(gtk_builder_get_object(builder, "fill_frame"));
    if (!app->fill_frame) { printf("ERROR: fill_frame es NULL\n"); return 1; }
    
    app->start_filling_button = GTK_WIDGET(gtk_builder_get_object(builder, "start_filling_button"));
    if (!app->start_filling_button) { printf("ERROR: start_filling_button es NULL\n"); return 1; }
    
    app->next_step_button = GTK_WIDGET(gtk_builder_get_object(builder, "next_step_button"));
    if (!app->next_step_button) { printf("ERROR: next_step_button es NULL\n"); return 1; }
    
    app->auto_complete_button = GTK_WIDGET(gtk_builder_get_object(builder, "auto_complete_button"));
    if (!app->auto_complete_button) { printf("ERROR: auto_complete_button es NULL\n"); return 1; }
    
    app->reset_button = GTK_WIDGET(gtk_builder_get_object(builder, "reset_button"));
    if (!app->reset_button) { printf("ERROR: reset_button es NULL\n"); return 1; }
    
    app->magic_square_grid = GTK_WIDGET(gtk_builder_get_object(builder, "magic_square_grid"));
    if (!app->magic_square_grid) { printf("ERROR: magic_square_grid es NULL\n"); return 1; }
    
    app->sums_textview = GTK_WIDGET(gtk_builder_get_object(builder, "sums_textview"));
    if (!app->sums_textview) { printf("ERROR: sums_textview es NULL\n"); return 1; }
    
    app->current_number_label = GTK_WIDGET(gtk_builder_get_object(builder, "current_number_label"));
    if (!app->current_number_label) { printf("ERROR: current_number_label es NULL\n"); return 1; }
    
    app->position_label = GTK_WIDGET(gtk_builder_get_object(builder, "position_label"));
    if (!app->position_label) { printf("ERROR: position_label es NULL\n"); return 1; }
    
    app->magic_sum_label = GTK_WIDGET(gtk_builder_get_object(builder, "magic_sum_label"));
    if (!app->magic_sum_label) { printf("ERROR: magic_sum_label es NULL\n"); return 1; }
    
    app->progress_label = GTK_WIDGET(gtk_builder_get_object(builder, "progress_label"));
    if (!app->progress_label) { printf("ERROR: progress_label es NULL\n"); return 1; }
    
    app->siames_radio = GTK_WIDGET(gtk_builder_get_object(builder, "siames_radio"));
    if (!app->siames_radio) { printf("ERROR: siames_radio es NULL\n"); return 1; }
    
    app->louber_radio = GTK_WIDGET(gtk_builder_get_object(builder, "louber_radio"));
    if (!app->louber_radio) { printf("ERROR: louber_radio es NULL\n"); return 1; }
    
    app->l_radio = GTK_WIDGET(gtk_builder_get_object(builder, "l_radio"));
    if (!app->l_radio) { printf("ERROR: l_radio es NULL\n"); return 1; }
    
    app->caracol_radio = GTK_WIDGET(gtk_builder_get_object(builder, "caracol_radio"));
    if (!app->caracol_radio) { printf("ERROR: caracol_radio es NULL\n"); return 1; }
    
    printf("DEBUG: Todos los widgets obtenidos correctamente\n");
    
    // Conectar señales
    printf("DEBUG: Conectando señales...\n");
    
    g_signal_connect(app->main_window, "destroy", G_CALLBACK(on_main_window_destroy), NULL);
    g_signal_connect(app->create_grid_button, "clicked", G_CALLBACK(on_create_grid_button_clicked), app);
    g_signal_connect(app->start_filling_button, "clicked", G_CALLBACK(on_start_filling_button_clicked), app);
    g_signal_connect(app->next_step_button, "clicked", G_CALLBACK(on_next_step_button_clicked), app);
    g_signal_connect(app->auto_complete_button, "clicked", G_CALLBACK(on_auto_complete_button_clicked), app);
    g_signal_connect(app->reset_button, "clicked", G_CALLBACK(on_reset_button_clicked), app);
    
    printf("DEBUG: Señales conectadas correctamente\n");
    
    // Inicializar texto de sumas
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(app->sums_textview));
    gtk_text_buffer_set_text(buffer, "Crea un cuadro para ver las sumas parciales...", -1);
    
    printf("DEBUG: Mostrando ventana principal\n");
    gtk_widget_show_all(app->main_window);
    
    printf("DEBUG: Iniciando loop principal de GTK\n");
    gtk_main();
    
    printf("DEBUG: Limpiando recursos\n");
    g_object_unref(builder);
    g_free(app);
    
    return 0;
}

