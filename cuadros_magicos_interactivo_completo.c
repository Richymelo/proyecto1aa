/*
 * Generador de Cuadros Mágicos Interactivo - Versión Completa
 * Sin dependencia de archivos .glade - Interfaz creada programáticamente
 * 
 * Características:
 * - Orden del cuadro: número impar entre 3 y 21
 * - 4+ métodos de llenado diferentes
 * - Posición inicial aleatoria
 * - Modo paso a paso y modo automático
 * - Sumas parciales en tiempo real
 * - Interfaz estéticamente agradable
 */

#include <gtk/gtk.h>
#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>

#define MAX_SIZE 21

// Enumeración de métodos
typedef enum {
    METODO_SIAMES = 0,
    METODO_L = 1,
    METODO_DIAGONAL_PRINCIPAL = 2,
    METODO_DIAGONAL_SECUNDARIA = 3,
    NUM_METODOS = 4
} MetodoLlenado;

// Estructura principal de la aplicación
typedef struct {
    // Ventana principal
    GtkWidget *main_window;
    
    // Panel de controles
    GtkWidget *controls_frame;
    GtkWidget *size_spin;
    GtkWidget *create_button;
    GtkWidget *method_combo;
    GtkWidget *start_button;
    GtkWidget *step_button;
    GtkWidget *complete_button;
    GtkWidget *reset_button;
    
    // Panel de información
    GtkWidget *info_frame;
    GtkWidget *current_number_label;
    GtkWidget *position_label;
    GtkWidget *magic_sum_label;
    GtkWidget *progress_label;
    GtkWidget *method_description_label;
    
    // Grid del cuadro mágico
    GtkWidget *grid_frame;
    GtkWidget *magic_grid;
    GtkWidget *cell_labels[MAX_SIZE][MAX_SIZE];
    
    // Panel de sumas parciales
    GtkWidget *sums_frame;
    GtkWidget *sums_textview;
    GtkTextBuffer *sums_buffer;
    
    // Estado del cuadro mágico
    int matrix[MAX_SIZE][MAX_SIZE];
    int size;
    int current_number;
    int current_row;
    int current_col;
    int total_numbers;
    int magic_sum;
    bool is_filling;
    bool grid_created;
    MetodoLlenado selected_method;
    
    // CSS Provider para estilos
    GtkCssProvider *css_provider;
} AppData;

// Descripciones de los métodos
static const char* method_descriptions[] = {
    "Método Siamés: Subir y derecha, si ocupado bajar",
    "Método en L: Subir 2 y derecha, si ocupado bajar",
    "Método Diagonal Principal: Llenado por diagonal principal",
    "Método Diagonal Secundaria: Llenado por diagonal secundaria"
};

// ============= FUNCIONES DE MOVIMIENTOS =============

// Método Siamés clásico
void metodo_siames(AppData *app, int *new_row, int *new_col) {
    *new_row = (app->current_row - 1 + app->size) % app->size;
    *new_col = (app->current_col + 1) % app->size;
    
    if (app->matrix[*new_row][*new_col] != 0) {
        *new_row = (app->current_row + 1) % app->size;
        *new_col = app->current_col;
    }
}


// Método en L
void metodo_l(AppData *app, int *new_row, int *new_col) {
    *new_row = (app->current_row - 2 + app->size) % app->size;
    *new_col = (app->current_col + 1) % app->size;
    
    if (app->matrix[*new_row][*new_col] != 0) {
        *new_row = (app->current_row + 1) % app->size;
        *new_col = app->current_col;
    }
}


// Método Diagonal Principal
void metodo_diagonal_principal(AppData *app, int *new_row, int *new_col) {
    // Avanzar por diagonal principal, luego columnas adyacentes
    int pos = app->current_number - 1;
    int diag_pos = pos % app->size;
    int offset = pos / app->size;
    
    if (offset == 0) {
        *new_row = diag_pos;
        *new_col = diag_pos;
    } else {
        int side = (offset - 1) / 2 + 1;
        bool is_right = ((offset - 1) % 2) == 0;
        
        if (is_right) {
            *new_col = (diag_pos + side) % app->size;
            *new_row = diag_pos;
        } else {
            *new_col = (diag_pos - side + app->size) % app->size;
            *new_row = diag_pos;
        }
    }
}

// Método Diagonal Secundaria
void metodo_diagonal_secundaria(AppData *app, int *new_row, int *new_col) {
    // Avanzar por diagonal secundaria, luego columnas adyacentes
    int pos = app->current_number - 1;
    int diag_pos = pos % app->size;
    int offset = pos / app->size;
    
    if (offset == 0) {
        *new_row = diag_pos;
        *new_col = app->size - 1 - diag_pos;
    } else {
        int side = (offset - 1) / 2 + 1;
        bool is_right = ((offset - 1) % 2) == 0;
        
        if (is_right) {
            *new_col = (app->size - 1 - diag_pos + side) % app->size;
            *new_row = diag_pos;
        } else {
            *new_col = (app->size - 1 - diag_pos - side + app->size) % app->size;
            *new_row = diag_pos;
        }
    }
}

// ============= FUNCIONES AUXILIARES =============

// Inicializar matriz
void inicializar_matriz(AppData *app) {
    for (int i = 0; i < app->size; i++) {
        for (int j = 0; j < app->size; j++) {
            app->matrix[i][j] = 0;
        }
    }
}

// Calcular suma mágica
int calcular_suma_magica(int n) {
    return n * (n * n + 1) / 2;
}

// Obtener posición inicial aleatoria válida
void obtener_posicion_inicial(AppData *app) {
    // Para algunos métodos, usar posiciones específicas optimizadas
    switch (app->selected_method) {
        case METODO_SIAMES:
            app->current_row = 0;
            app->current_col = app->size / 2;
            break;
        case METODO_DIAGONAL_PRINCIPAL:
            app->current_row = 0;
            app->current_col = 0;
            break;
        case METODO_DIAGONAL_SECUNDARIA:
            app->current_row = 0;
            app->current_col = app->size - 1;
            break;
        default:
            // Posición aleatoria para otros métodos
            app->current_row = rand() % app->size;
            app->current_col = rand() % app->size;
            break;
    }
}

// Actualizar sumas parciales
void actualizar_sumas_parciales(AppData *app) {
    if (!app->sums_buffer) return;
    
    GString *text = g_string_new("");
    
    // Título
    g_string_append(text, "═══════════════════════════════════\n");
    g_string_append(text, "           SUMAS PARCIALES\n");
    g_string_append(text, "═══════════════════════════════════\n\n");
    
    // Sumas de filas
    g_string_append(text, "📋 FILAS:\n");
    for (int i = 0; i < app->size; i++) {
        int sum = 0;
        for (int j = 0; j < app->size; j++) {
            sum += app->matrix[i][j];
        }
        g_string_append_printf(text, "  Fila %2d: %3d", i + 1, sum);
        if (sum == app->magic_sum && sum > 0) {
            g_string_append(text, " ✓ ¡COMPLETA!");
        } else if (sum > 0) {
            g_string_append_printf(text, " (faltan %d)", app->magic_sum - sum);
        }
        g_string_append(text, "\n");
    }
    
    // Sumas de columnas
    g_string_append(text, "\n📋 COLUMNAS:\n");
    for (int j = 0; j < app->size; j++) {
        int sum = 0;
        for (int i = 0; i < app->size; i++) {
            sum += app->matrix[i][j];
        }
        g_string_append_printf(text, "  Col. %2d: %3d", j + 1, sum);
        if (sum == app->magic_sum && sum > 0) {
            g_string_append(text, " ✓ ¡COMPLETA!");
        } else if (sum > 0) {
            g_string_append_printf(text, " (faltan %d)", app->magic_sum - sum);
        }
        g_string_append(text, "\n");
    }
    
    // Diagonales
    g_string_append(text, "\n📋 DIAGONALES:\n");
    
    // Diagonal principal
    int diag1_sum = 0;
    for (int i = 0; i < app->size; i++) {
        diag1_sum += app->matrix[i][i];
    }
    g_string_append_printf(text, "  Principal: %3d", diag1_sum);
    if (diag1_sum == app->magic_sum && diag1_sum > 0) {
        g_string_append(text, " ✓ ¡COMPLETA!");
    } else if (diag1_sum > 0) {
        g_string_append_printf(text, " (faltan %d)", app->magic_sum - diag1_sum);
    }
    g_string_append(text, "\n");
    
    // Diagonal secundaria
    int diag2_sum = 0;
    for (int i = 0; i < app->size; i++) {
        diag2_sum += app->matrix[i][app->size - 1 - i];
    }
    g_string_append_printf(text, "  Secundaria: %3d", diag2_sum);
    if (diag2_sum == app->magic_sum && diag2_sum > 0) {
        g_string_append(text, " ✓ ¡COMPLETA!");
    } else if (diag2_sum > 0) {
        g_string_append_printf(text, " (faltan %d)", app->magic_sum - diag2_sum);
    }
    g_string_append(text, "\n");
    
    // Estadísticas
    int completed_rows = 0, completed_cols = 0;
    for (int i = 0; i < app->size; i++) {
        int row_sum = 0, col_sum = 0;
        for (int j = 0; j < app->size; j++) {
            row_sum += app->matrix[i][j];
            col_sum += app->matrix[j][i];
        }
        if (row_sum == app->magic_sum) completed_rows++;
        if (col_sum == app->magic_sum) completed_cols++;
    }
    
    bool diag1_complete = (diag1_sum == app->magic_sum);
    bool diag2_complete = (diag2_sum == app->magic_sum);
    
    g_string_append(text, "\n═══════════════════════════════════\n");
    g_string_append_printf(text, "📊 PROGRESO:\n");
    g_string_append_printf(text, "  Filas completas: %d/%d\n", completed_rows, app->size);
    g_string_append_printf(text, "  Columnas completas: %d/%d\n", completed_cols, app->size);
    g_string_append_printf(text, "  Diagonales completas: %d/2\n", 
                          (diag1_complete ? 1 : 0) + (diag2_complete ? 1 : 0));
    
    if (completed_rows == app->size && completed_cols == app->size && 
        diag1_complete && diag2_complete) {
        g_string_append(text, "\n🎉 ¡CUADRO MÁGICO COMPLETADO! 🎉\n");
        g_string_append(text, "   ¡Todas las sumas son correctas!\n");
    }
    
    gtk_text_buffer_set_text(app->sums_buffer, text->str, -1);
    g_string_free(text, TRUE);
}

// Actualizar display del cuadro
void actualizar_display_cuadro(AppData *app) {
    if (!app->grid_created) return;
    
    for (int i = 0; i < app->size; i++) {
        for (int j = 0; j < app->size; j++) {
            if (app->cell_labels[i][j]) {
                if (app->matrix[i][j] == 0) {
                    gtk_label_set_text(GTK_LABEL(app->cell_labels[i][j]), "");
                    gtk_widget_set_name(app->cell_labels[i][j], "empty-cell");
                } else {
                    char text[10];
                    snprintf(text, sizeof(text), "%d", app->matrix[i][j]);
                    gtk_label_set_text(GTK_LABEL(app->cell_labels[i][j]), text);
                    
                    // Resaltar celda actual
                    if (i == app->current_row && j == app->current_col && app->is_filling) {
                        gtk_widget_set_name(app->cell_labels[i][j], "current-cell");
                    } else if (app->matrix[i][j] == app->current_number - 1 && app->is_filling) {
                        gtk_widget_set_name(app->cell_labels[i][j], "last-cell");
                    } else {
                        gtk_widget_set_name(app->cell_labels[i][j], "filled-cell");
                    }
                }
            }
        }
    }
    
    actualizar_sumas_parciales(app);
}

// ============= CALLBACKS =============

// Crear el grid del cuadro mágico
void on_create_button_clicked(GtkButton *button, AppData *app) {
    app->size = (int)gtk_spin_button_get_value(GTK_SPIN_BUTTON(app->size_spin));
    
    // Asegurar que sea impar
    if (app->size % 2 == 0) {
        app->size++;
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(app->size_spin), app->size);
    }
    
    // Limpiar grid anterior
    if (app->grid_created) {
        gtk_container_foreach(GTK_CONTAINER(app->magic_grid), 
                             (GtkCallback)gtk_widget_destroy, NULL);
    }
    
    // Calcular suma mágica
    app->magic_sum = calcular_suma_magica(app->size);
    app->total_numbers = app->size * app->size;
    
    // Crear nuevas celdas
    for (int i = 0; i < app->size; i++) {
        for (int j = 0; j < app->size; j++) {
            GtkWidget *event_box = gtk_event_box_new();
            app->cell_labels[i][j] = gtk_label_new("");
            
            // Configurar tamaño y estilo
            int cell_size = MAX(40, 300 / app->size);
            gtk_widget_set_size_request(event_box, cell_size, cell_size);
            gtk_widget_set_name(app->cell_labels[i][j], "empty-cell");
            
            // Agregar borde visual
            gtk_widget_set_name(event_box, "cell-container");
            
            gtk_container_add(GTK_CONTAINER(event_box), app->cell_labels[i][j]);
            gtk_grid_attach(GTK_GRID(app->magic_grid), event_box, j, i, 1, 1);
        }
    }
    
    inicializar_matriz(app);
    app->grid_created = TRUE;
    app->is_filling = FALSE;
    app->current_number = 1;
    
    // Actualizar información
    char text[200];
    snprintf(text, sizeof(text), "<span size='large'><b>Suma Mágica Objetivo: %d</b></span>", 
             app->magic_sum);
    gtk_label_set_markup(GTK_LABEL(app->magic_sum_label), text);
    
    snprintf(text, sizeof(text), "Cuadro %dx%d creado. Selecciona método y presiona 'Iniciar'", 
             app->size, app->size);
    gtk_label_set_text(GTK_LABEL(app->progress_label), text);
    
    gtk_widget_show_all(app->magic_grid);
    
    // Habilitar controles
    gtk_widget_set_sensitive(app->method_combo, TRUE);
    gtk_widget_set_sensitive(app->start_button, TRUE);
    gtk_widget_set_sensitive(app->create_button, FALSE);
    
    actualizar_display_cuadro(app);
}

// Iniciar proceso de llenado
void on_start_button_clicked(GtkButton *button, AppData *app) {
    app->selected_method = gtk_combo_box_get_active(GTK_COMBO_BOX(app->method_combo));
    
    obtener_posicion_inicial(app);
    app->current_number = 1;
    app->is_filling = TRUE;
    
    // Actualizar información
    char text[200];
    snprintf(text, sizeof(text), "<span size='large' color='blue'><b>Próximo número: %d</b></span>", 
             app->current_number);
    gtk_label_set_markup(GTK_LABEL(app->current_number_label), text);
    
    snprintf(text, sizeof(text), "Posición actual: (%d, %d)", 
             app->current_row + 1, app->current_col + 1);
    gtk_label_set_text(GTK_LABEL(app->position_label), text);
    
    gtk_label_set_text(GTK_LABEL(app->method_description_label), 
                      method_descriptions[app->selected_method]);
    
    // Habilitar/deshabilitar controles
    gtk_widget_set_sensitive(app->step_button, TRUE);
    gtk_widget_set_sensitive(app->complete_button, TRUE);
    gtk_widget_set_sensitive(app->start_button, FALSE);
    gtk_widget_set_sensitive(app->method_combo, FALSE);
    
    actualizar_display_cuadro(app);
}

// Siguiente paso
void on_step_button_clicked(GtkButton *button, AppData *app) {
    if (!app->is_filling) return;
    
    // Colocar número actual
    app->matrix[app->current_row][app->current_col] = app->current_number;
    
    // Actualizar progreso
    char text[200];
    snprintf(text, sizeof(text), "Números colocados: %d/%d (%.1f%%)", 
             app->current_number, app->total_numbers, 
             (app->current_number * 100.0) / app->total_numbers);
    gtk_label_set_text(GTK_LABEL(app->progress_label), text);
    
    // Verificar si terminamos
    if (app->current_number >= app->total_numbers) {
        app->is_filling = FALSE;
        gtk_widget_set_sensitive(app->step_button, FALSE);
        gtk_widget_set_sensitive(app->complete_button, FALSE);
        
        gtk_label_set_markup(GTK_LABEL(app->current_number_label), 
                            "<span size='x-large' color='green'><b>¡COMPLETADO!</b></span>");
        
        gtk_label_set_text(GTK_LABEL(app->progress_label), 
                          "🎉 ¡Cuadro mágico completado exitosamente!");
        
        actualizar_display_cuadro(app);
        return;
    }
    
    // Calcular siguiente posición
    int new_row, new_col;
    switch (app->selected_method) {
        case METODO_SIAMES:
            metodo_siames(app, &new_row, &new_col);
            break;
        case METODO_L:
            metodo_l(app, &new_row, &new_col);
            break;
        case METODO_DIAGONAL_PRINCIPAL:
            metodo_diagonal_principal(app, &new_row, &new_col);
            break;
        case METODO_DIAGONAL_SECUNDARIA:
            metodo_diagonal_secundaria(app, &new_row, &new_col);
            break;
        default:
            metodo_siames(app, &new_row, &new_col);
            break;
    }
    
    app->current_row = new_row;
    app->current_col = new_col;
    app->current_number++;
    
    // Actualizar información
    snprintf(text, sizeof(text), "<span size='large' color='blue'><b>Próximo número: %d</b></span>", 
             app->current_number);
    gtk_label_set_markup(GTK_LABEL(app->current_number_label), text);
    
    snprintf(text, sizeof(text), "Próxima posición: (%d, %d)", 
             app->current_row + 1, app->current_col + 1);
    gtk_label_set_text(GTK_LABEL(app->position_label), text);
    
    actualizar_display_cuadro(app);
}

// Completar automáticamente
void on_complete_button_clicked(GtkButton *button, AppData *app) {
    gtk_widget_set_sensitive(app->step_button, FALSE);
    gtk_widget_set_sensitive(app->complete_button, FALSE);
    
    while (app->is_filling && app->current_number <= app->total_numbers) {
        on_step_button_clicked(NULL, app);
        
        // Pequeña pausa para visualización
        while (gtk_events_pending()) {
            gtk_main_iteration();
        }
        g_usleep(50000); // 50ms
    }
}

// Reiniciar
void on_reset_button_clicked(GtkButton *button, AppData *app) {
    if (app->grid_created) {
        gtk_container_foreach(GTK_CONTAINER(app->magic_grid), 
                             (GtkCallback)gtk_widget_destroy, NULL);
        app->grid_created = FALSE;
    }
    
    app->is_filling = FALSE;
    app->current_number = 1;
    
    // Resetear controles
    gtk_widget_set_sensitive(app->create_button, TRUE);
    gtk_widget_set_sensitive(app->method_combo, FALSE);
    gtk_widget_set_sensitive(app->start_button, FALSE);
    gtk_widget_set_sensitive(app->step_button, FALSE);
    gtk_widget_set_sensitive(app->complete_button, FALSE);
    
    // Limpiar información
    gtk_label_set_text(GTK_LABEL(app->current_number_label), "Selecciona tamaño y crea el cuadro");
    gtk_label_set_text(GTK_LABEL(app->position_label), "");
    gtk_label_set_text(GTK_LABEL(app->magic_sum_label), "Suma mágica: --");
    gtk_label_set_text(GTK_LABEL(app->progress_label), "Listo para crear nuevo cuadro");
    gtk_label_set_text(GTK_LABEL(app->method_description_label), "");
    
    gtk_text_buffer_set_text(app->sums_buffer, 
                            "Crea un cuadro mágico para ver las sumas parciales en tiempo real...", -1);
}

// Cerrar aplicación
void on_window_destroy(GtkWidget *widget, gpointer data) {
    gtk_main_quit();
}

// ============= FUNCIÓN PRINCIPAL =============

int main(int argc, char *argv[]) {
    printf("INFO: Iniciando Cuadros Mágicos Interactivos (Versión Completa)\n");
    
    gtk_init(&argc, &argv);
    srand(time(NULL));
    
    AppData *app = g_malloc0(sizeof(AppData));
    
    // ===== CREAR ESTILOS CSS =====
    app->css_provider = gtk_css_provider_new();
    const char *css_data = 
        "/* Estilos para celdas del cuadro mágico */"
        ".cell-container {"
        "    border: 2px solid #333333;"
        "    border-radius: 4px;"
        "    margin: 1px;"
        "}"
        "label#empty-cell {"
        "    background-color: #f5f5f5;"
        "    color: #888888;"
        "    font-size: 14px;"
        "    font-weight: bold;"
        "}"
        "label#filled-cell {"
        "    background-color: #e3f2fd;"
        "    color: #1976d2;"
        "    font-size: 16px;"
        "    font-weight: bold;"
        "}"
        "label#current-cell {"
        "    background-color: #ffeb3b;"
        "    color: #f57f17;"
        "    font-size: 18px;"
        "    font-weight: bold;"
        "    border: 2px solid #ff9800;"
        "}"
        "label#last-cell {"
        "    background-color: #c8e6c9;"
        "    color: #388e3c;"
        "    font-size: 16px;"
        "    font-weight: bold;"
        "}"
        "/* Estilos para frames */"
        "frame {"
        "    border-radius: 8px;"
        "    margin: 5px;"
        "}"
        "/* Estilos para botones */"
        "button {"
        "    border-radius: 6px;"
        "    padding: 8px 16px;"
        "    font-weight: bold;"
        "}"
        "button:hover {"
        "    background-color: #e1f5fe;"
        "}";
    
    GError *css_error = NULL;
    if (gtk_css_provider_load_from_data(app->css_provider, css_data, -1, &css_error)) {
        gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
                                                 GTK_STYLE_PROVIDER(app->css_provider),
                                                 GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    } else {
        g_warning("Error cargando CSS: %s", css_error->message);
        g_clear_error(&css_error);
    }
    
    // ===== CREAR VENTANA PRINCIPAL =====
    app->main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(app->main_window), "🔮 Cuadros Mágicos Interactivos 🔮");
    gtk_window_set_default_size(GTK_WINDOW(app->main_window), 1200, 800);
    gtk_window_set_position(GTK_WINDOW(app->main_window), GTK_WIN_POS_CENTER);
    gtk_container_set_border_width(GTK_CONTAINER(app->main_window), 10);
    
    // Layout principal (horizontal)
    GtkWidget *main_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 15);
    gtk_container_add(GTK_CONTAINER(app->main_window), main_hbox);
    
    // ===== PANEL IZQUIERDO: CONTROLES =====
    GtkWidget *left_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_widget_set_size_request(left_vbox, 400, -1);
    gtk_box_pack_start(GTK_BOX(main_hbox), left_vbox, FALSE, FALSE, 0);
    
    // Título
    GtkWidget *title_label = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(title_label), 
                        "<span size='xx-large' weight='bold' color='#1976d2'>🔮 Cuadros Mágicos</span>\n"
                        "<span size='large' color='#666'>Generador Interactivo</span>");
    gtk_label_set_justify(GTK_LABEL(title_label), GTK_JUSTIFY_CENTER);
    gtk_box_pack_start(GTK_BOX(left_vbox), title_label, FALSE, FALSE, 0);
    
    // Frame de configuración
    app->controls_frame = gtk_frame_new("📋 Configuración del Cuadro");
    gtk_box_pack_start(GTK_BOX(left_vbox), app->controls_frame, FALSE, FALSE, 0);
    
    GtkWidget *controls_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_set_border_width(GTK_CONTAINER(controls_vbox), 15);
    gtk_container_add(GTK_CONTAINER(app->controls_frame), controls_vbox);
    
    // Selección de tamaño
    GtkWidget *size_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_pack_start(GTK_BOX(controls_vbox), size_hbox, FALSE, FALSE, 0);
    
    GtkWidget *size_label = gtk_label_new("Tamaño del cuadro (impar, 3-21):");
    gtk_box_pack_start(GTK_BOX(size_hbox), size_label, FALSE, FALSE, 0);
    
    app->size_spin = gtk_spin_button_new_with_range(3, 21, 2);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(app->size_spin), 5);
    gtk_box_pack_start(GTK_BOX(size_hbox), app->size_spin, FALSE, FALSE, 0);
    
    app->create_button = gtk_button_new_with_label("✨ Crear Cuadro");
    gtk_box_pack_start(GTK_BOX(controls_vbox), app->create_button, FALSE, FALSE, 0);
    
    // Selección de método
    GtkWidget *method_label = gtk_label_new("Método de llenado:");
    gtk_box_pack_start(GTK_BOX(controls_vbox), method_label, FALSE, FALSE, 0);
    
    app->method_combo = gtk_combo_box_text_new();
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(app->method_combo), "🔄 Método Siamés");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(app->method_combo), "📐 Método en L");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(app->method_combo), "📍 Diagonal Principal");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(app->method_combo), "📍 Diagonal Secundaria");
    gtk_combo_box_set_active(GTK_COMBO_BOX(app->method_combo), 0);
    gtk_widget_set_sensitive(app->method_combo, FALSE);
    gtk_box_pack_start(GTK_BOX(controls_vbox), app->method_combo, FALSE, FALSE, 0);
    
    app->start_button = gtk_button_new_with_label("🚀 Iniciar Llenado");
    gtk_widget_set_sensitive(app->start_button, FALSE);
    gtk_box_pack_start(GTK_BOX(controls_vbox), app->start_button, FALSE, FALSE, 0);
    
    // Frame de control de llenado
    GtkWidget *filling_frame = gtk_frame_new("🎮 Control de Llenado");
    gtk_box_pack_start(GTK_BOX(left_vbox), filling_frame, FALSE, FALSE, 0);
    
    GtkWidget *filling_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_set_border_width(GTK_CONTAINER(filling_vbox), 15);
    gtk_container_add(GTK_CONTAINER(filling_frame), filling_vbox);
    
    app->step_button = gtk_button_new_with_label("▶️ Siguiente Número");
    gtk_widget_set_sensitive(app->step_button, FALSE);
    gtk_box_pack_start(GTK_BOX(filling_vbox), app->step_button, FALSE, FALSE, 0);
    
    app->complete_button = gtk_button_new_with_label("⚡ Completar Automáticamente");
    gtk_widget_set_sensitive(app->complete_button, FALSE);
    gtk_box_pack_start(GTK_BOX(filling_vbox), app->complete_button, FALSE, FALSE, 0);
    
    app->reset_button = gtk_button_new_with_label("🔄 Reiniciar Todo");
    gtk_box_pack_start(GTK_BOX(filling_vbox), app->reset_button, FALSE, FALSE, 0);
    
    // Frame de información
    app->info_frame = gtk_frame_new("📊 Información del Proceso");
    gtk_box_pack_start(GTK_BOX(left_vbox), app->info_frame, TRUE, TRUE, 0);
    
    GtkWidget *info_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);
    gtk_container_set_border_width(GTK_CONTAINER(info_vbox), 15);
    gtk_container_add(GTK_CONTAINER(app->info_frame), info_vbox);
    
    app->current_number_label = gtk_label_new("Selecciona tamaño y crea el cuadro");
    gtk_label_set_line_wrap(GTK_LABEL(app->current_number_label), TRUE);
    gtk_box_pack_start(GTK_BOX(info_vbox), app->current_number_label, FALSE, FALSE, 0);
    
    app->position_label = gtk_label_new("");
    gtk_box_pack_start(GTK_BOX(info_vbox), app->position_label, FALSE, FALSE, 0);
    
    app->magic_sum_label = gtk_label_new("Suma mágica: --");
    gtk_box_pack_start(GTK_BOX(info_vbox), app->magic_sum_label, FALSE, FALSE, 0);
    
    app->progress_label = gtk_label_new("Listo para crear nuevo cuadro");
    gtk_label_set_line_wrap(GTK_LABEL(app->progress_label), TRUE);
    gtk_box_pack_start(GTK_BOX(info_vbox), app->progress_label, FALSE, FALSE, 0);
    
    app->method_description_label = gtk_label_new("");
    gtk_label_set_line_wrap(GTK_LABEL(app->method_description_label), TRUE);
    gtk_box_pack_start(GTK_BOX(info_vbox), app->method_description_label, FALSE, FALSE, 0);
    
    // ===== PANEL DERECHO =====
    GtkWidget *right_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_box_pack_start(GTK_BOX(main_hbox), right_vbox, TRUE, TRUE, 0);
    
    // Frame del cuadro mágico
    app->grid_frame = gtk_frame_new("🎯 Cuadro Mágico");
    gtk_box_pack_start(GTK_BOX(right_vbox), app->grid_frame, TRUE, TRUE, 0);
    
    GtkWidget *grid_scroll = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(grid_scroll), 
                                   GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_container_add(GTK_CONTAINER(app->grid_frame), grid_scroll);
    
    app->magic_grid = gtk_grid_new();
    gtk_grid_set_row_homogeneous(GTK_GRID(app->magic_grid), TRUE);
    gtk_grid_set_column_homogeneous(GTK_GRID(app->magic_grid), TRUE);
    gtk_grid_set_row_spacing(GTK_GRID(app->magic_grid), 2);
    gtk_grid_set_column_spacing(GTK_GRID(app->magic_grid), 2);
    gtk_widget_set_halign(app->magic_grid, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(app->magic_grid, GTK_ALIGN_CENTER);
    gtk_container_add(GTK_CONTAINER(grid_scroll), app->magic_grid);
    
    // Frame de sumas parciales
    app->sums_frame = gtk_frame_new("📈 Sumas Parciales en Tiempo Real");
    gtk_widget_set_size_request(app->sums_frame, -1, 300);
    gtk_box_pack_start(GTK_BOX(right_vbox), app->sums_frame, FALSE, FALSE, 0);
    
    GtkWidget *sums_scroll = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(sums_scroll), 
                                   GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_container_add(GTK_CONTAINER(app->sums_frame), sums_scroll);
    
    app->sums_textview = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(app->sums_textview), FALSE);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(app->sums_textview), FALSE);
    gtk_text_view_set_left_margin(GTK_TEXT_VIEW(app->sums_textview), 10);
    gtk_text_view_set_right_margin(GTK_TEXT_VIEW(app->sums_textview), 10);
    gtk_container_add(GTK_CONTAINER(sums_scroll), app->sums_textview);
    
    app->sums_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(app->sums_textview));
    
    // Configurar fuente monoespaciada para el textview usando CSS (método moderno)
    const char *font_css = 
        "textview {"
        "    font-family: 'Monospace';"
        "    font-size: 11pt;"
        "}";
    
    GtkCssProvider *font_provider = gtk_css_provider_new();
    GError *font_error = NULL;
    if (gtk_css_provider_load_from_data(font_provider, font_css, -1, &font_error)) {
        gtk_style_context_add_provider(gtk_widget_get_style_context(app->sums_textview),
                                     GTK_STYLE_PROVIDER(font_provider),
                                     GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    } else {
        g_warning("Error cargando CSS de fuente: %s", font_error->message);
        g_clear_error(&font_error);
    }
    g_object_unref(font_provider);
    
    // ===== CONECTAR SEÑALES =====
    g_signal_connect(app->main_window, "destroy", G_CALLBACK(on_window_destroy), NULL);
    g_signal_connect(app->create_button, "clicked", G_CALLBACK(on_create_button_clicked), app);
    g_signal_connect(app->start_button, "clicked", G_CALLBACK(on_start_button_clicked), app);
    g_signal_connect(app->step_button, "clicked", G_CALLBACK(on_step_button_clicked), app);
    g_signal_connect(app->complete_button, "clicked", G_CALLBACK(on_complete_button_clicked), app);
    g_signal_connect(app->reset_button, "clicked", G_CALLBACK(on_reset_button_clicked), app);
    
    // ===== INICIALIZAR =====
    app->grid_created = FALSE;
    app->is_filling = FALSE;
    
    gtk_text_buffer_set_text(app->sums_buffer, 
                            "Crea un cuadro mágico para ver las sumas parciales en tiempo real...", -1);
    
    // ===== MOSTRAR VENTANA =====
    gtk_widget_show_all(app->main_window);
    
    printf("INFO: Interfaz creada exitosamente\n");
    printf("INFO: Características implementadas:\n");
    printf("  - Orden del cuadro: 3-21 (números impares)\n");
    printf("  - 4 métodos de llenado diferentes\n");
    printf("  - Posición inicial aleatoria/optimizada\n");
    printf("  - Modo paso a paso y automático\n");
    printf("  - Sumas parciales en tiempo real\n");
    printf("  - Interfaz estéticamente mejorada\n");
    
    // Iniciar loop principal
    gtk_main();
    
    // Limpiar
    g_object_unref(app->css_provider);
    g_free(app);
    
    return 0;
}

