#include <gtk/gtk.h>
#include <glib.h>
#include "cuadros_magicos.h"

// Incluir las funciones de movimientos directamente para evitar conflictos
// Método Siamés clásico: mueve en diagonal izquierda-abajo y ajusta si está ocupado
int metodoSiames_gtk(int matriz[21][21], int n, int fila, int columna) {
    int nuevaFila = (fila - 1 + n) % n;
    int nuevaColumna = (columna + 1) % n;

    if (matriz[nuevaFila][nuevaColumna] != 0) { 
        nuevaFila = (fila + 1) % n;
        nuevaColumna = columna;
    }

    return nuevaFila * 21 + nuevaColumna;
}

// Método en L: movimiento en forma de caballo del ajedrez
int metodoEnL_gtk(int matriz[21][21], int n, int fila, int columna) {
    int nuevaFila = (fila - 2 + n) % n;
    int nuevaColumna = (columna + 1) % n;
    if (matriz[nuevaFila][nuevaColumna] != 0) {
        nuevaFila = (fila + 1) % n;
        nuevaColumna = columna;
    }
    return nuevaFila * 21 + nuevaColumna;
}

// Método De la Loubère: mueve en diagonal derecha-arriba y ajusta si está ocupado
int metodoLouber_gtk(int matriz[21][21], int n, int fila, int columna) {
    int nuevaFila = (fila + 1) % n;
    int nuevaColumna = (columna - 1 + n) % n;

    if (matriz[nuevaFila][nuevaColumna] != 0) {
        nuevaFila = (fila - 1 + n) % n;
        nuevaColumna = columna;
    }

    return nuevaFila * 21 + nuevaColumna;
}

// Método Alterno de Diagonales: variante del método siamés
int metodoAlterno_gtk(int matriz[21][21], int n, int fila, int columna) {
    int nuevaFila = (fila - 1 + n) % n;
    int nuevaColumna = (columna - 1 + n) % n;
    if (matriz[nuevaFila][nuevaColumna] != 0) {
        nuevaFila = (fila + 1) % n;
        nuevaColumna = columna;
    }
    return nuevaFila * 21 + nuevaColumna;
}

// Estructura para manejar los widgets de la aplicación
typedef struct {
    GtkWidget *main_window;
    GtkWidget *size_spin;
    GtkWidget *magic_square_grid;
    GtkWidget *status_label;
    GtkWidget *validate_button;
    
    // Radio buttons para algoritmos
    GtkWidget *kurosaka_radio;
    GtkWidget *siames_radio;
    GtkWidget *loubere_radio;
    GtkWidget *l_radio;
    GtkWidget *alterno_radio;
    
    // Cuadro mágico actual
    CuadroMagico *cuadro_actual;
} AppWidgets;

// Variables globales
static AppWidgets *app_widgets = NULL;

// Función para limpiar el grid del cuadro mágico
void limpiar_grid(GtkWidget *grid) {
    GList *children = gtk_container_get_children(GTK_CONTAINER(grid));
    for (GList *iter = children; iter != NULL; iter = g_list_next(iter)) {
        gtk_widget_destroy(GTK_WIDGET(iter->data));
    }
    g_list_free(children);
}

// Función para mostrar el cuadro mágico en el grid
void mostrar_cuadro_en_grid(CuadroMagico *cuadro, GtkWidget *grid) {
    if (!cuadro || !grid) return;
    
    limpiar_grid(grid);
    
    int n = cuadro->tamaño;
    
    // Crear y agregar labels para cada celda
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            char texto[10];
            snprintf(texto, sizeof(texto), "%d", cuadro->matriz[i][j]);
            
            GtkWidget *label = gtk_label_new(texto);
            gtk_widget_set_size_request(label, 60, 60);
            
            // Crear un frame para cada celda
            GtkWidget *frame = gtk_frame_new(NULL);
            gtk_frame_set_shadow_type(GTK_FRAME(frame), GTK_SHADOW_IN);
            gtk_container_add(GTK_CONTAINER(frame), label);
            
            // Agregar al grid
            gtk_grid_attach(GTK_GRID(grid), frame, j, i, 1, 1);
        }
    }
    
    gtk_widget_show_all(grid);
}

// Función para obtener el algoritmo seleccionado
TipoAlgoritmo obtener_algoritmo_seleccionado(AppWidgets *widgets) {
    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widgets->kurosaka_radio))) {
        return ALGORITMO_KUROSAKA;
    } else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widgets->siames_radio))) {
        return ALGORITMO_SIAMES;
    } else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widgets->loubere_radio))) {
        return ALGORITMO_LOUBERE;
    } else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widgets->l_radio))) {
        return ALGORITMO_L;
    } else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widgets->alterno_radio))) {
        return ALGORITMO_ALTERNO;
    }
    return ALGORITMO_KUROSAKA; // Por defecto
}

// Callback para el botón "Generar"
void on_generate_button_clicked(GtkWidget *widget, gpointer data) {
    AppWidgets *widgets = (AppWidgets*)data;
    
    // Obtener el tamaño seleccionado
    int tamaño = (int)gtk_spin_button_get_value(GTK_SPIN_BUTTON(widgets->size_spin));
    
    // Verificar que sea impar
    if (tamaño % 2 == 0) {
        gtk_label_set_text(GTK_LABEL(widgets->status_label), 
                          "Error: El tamaño debe ser un número impar.");
        return;
    }
    
    // Obtener el algoritmo seleccionado
    TipoAlgoritmo algoritmo = obtener_algoritmo_seleccionado(widgets);
    
    // Liberar cuadro anterior si existe
    if (widgets->cuadro_actual) {
        liberar_cuadro_magico(widgets->cuadro_actual);
    }
    
    // Generar nuevo cuadro mágico
    widgets->cuadro_actual = crear_cuadro_magico(tamaño, algoritmo);
    
    if (widgets->cuadro_actual) {
        // Mostrar el cuadro en el grid
        mostrar_cuadro_en_grid(widgets->cuadro_actual, widgets->magic_square_grid);
        
        // Actualizar status
        char status_text[200];
        const char *nombre_algoritmo = "";
        switch (algoritmo) {
            case ALGORITMO_KUROSAKA: nombre_algoritmo = "Kurosaka"; break;
            case ALGORITMO_SIAMES: nombre_algoritmo = "Siamés"; break;
            case ALGORITMO_LOUBERE: nombre_algoritmo = "De la Loubère"; break;
            case ALGORITMO_L: nombre_algoritmo = "Método L"; break;
            case ALGORITMO_ALTERNO: nombre_algoritmo = "Alterno"; break;
        }
        
        snprintf(status_text, sizeof(status_text), 
                "Cuadro %dx%d generado con algoritmo %s. Suma mágica: %d", 
                tamaño, tamaño, nombre_algoritmo, widgets->cuadro_actual->suma_magica);
        gtk_label_set_text(GTK_LABEL(widgets->status_label), status_text);
        
        // Habilitar botón de validación
        gtk_widget_set_sensitive(widgets->validate_button, TRUE);
        
        // Imprimir en consola también
        imprimir_cuadro_magico(widgets->cuadro_actual);
    } else {
        gtk_label_set_text(GTK_LABEL(widgets->status_label), 
                          "Error: No se pudo generar el cuadro mágico.");
    }
}

// Callback para el botón "Validar"
void on_validate_button_clicked(GtkWidget *widget, gpointer data) {
    AppWidgets *widgets = (AppWidgets*)data;
    
    if (!widgets->cuadro_actual) {
        gtk_label_set_text(GTK_LABEL(widgets->status_label), 
                          "Error: No hay cuadro mágico para validar.");
        return;
    }
    
    bool es_valido = validar_cuadro_magico(widgets->cuadro_actual);
    widgets->cuadro_actual->es_valido = es_valido;
    
    char status_text[200];
    snprintf(status_text, sizeof(status_text), 
            "Validación completada: El cuadro es %s", 
            es_valido ? "VÁLIDO ✓" : "INVÁLIDO ✗");
    gtk_label_set_text(GTK_LABEL(widgets->status_label), status_text);
    
    // Mostrar resultado detallado en consola
    printf("\n=== RESULTADO DE VALIDACIÓN ===\n");
    printf("Cuadro %dx%d: %s\n", 
           widgets->cuadro_actual->tamaño, 
           widgets->cuadro_actual->tamaño,
           es_valido ? "VÁLIDO" : "INVÁLIDO");
    
    if (es_valido) {
        printf("✓ Todas las filas suman %d\n", widgets->cuadro_actual->suma_magica);
        printf("✓ Todas las columnas suman %d\n", widgets->cuadro_actual->suma_magica);
        printf("✓ Las diagonales suman %d\n", widgets->cuadro_actual->suma_magica);
    }
    printf("==============================\n\n");
}

// Callback para el botón "Limpiar"
void on_clear_button_clicked(GtkWidget *widget, gpointer data) {
    AppWidgets *widgets = (AppWidgets*)data;
    
    // Limpiar el grid
    limpiar_grid(widgets->magic_square_grid);
    
    // Liberar cuadro actual
    if (widgets->cuadro_actual) {
        liberar_cuadro_magico(widgets->cuadro_actual);
        widgets->cuadro_actual = NULL;
    }
    
    // Actualizar status
    gtk_label_set_text(GTK_LABEL(widgets->status_label), 
                      "Listo para generar cuadro mágico...");
    
    // Deshabilitar botón de validación
    gtk_widget_set_sensitive(widgets->validate_button, FALSE);
}

// Callback para cerrar la aplicación
void on_main_window_destroy(GtkWidget *widget, gpointer data) {
    AppWidgets *widgets = (AppWidgets*)data;
    
    // Liberar cuadro actual
    if (widgets->cuadro_actual) {
        liberar_cuadro_magico(widgets->cuadro_actual);
    }
    
    gtk_main_quit();
}

// Función para conectar las señales
void conectar_señales(GtkBuilder *builder, AppWidgets *widgets) {
    // Obtener widgets del builder
    widgets->main_window = GTK_WIDGET(gtk_builder_get_object(builder, "main_window"));
    widgets->size_spin = GTK_WIDGET(gtk_builder_get_object(builder, "size_spin"));
    widgets->magic_square_grid = GTK_WIDGET(gtk_builder_get_object(builder, "magic_square_grid"));
    widgets->status_label = GTK_WIDGET(gtk_builder_get_object(builder, "status_label"));
    widgets->validate_button = GTK_WIDGET(gtk_builder_get_object(builder, "validate_button"));
    
    widgets->kurosaka_radio = GTK_WIDGET(gtk_builder_get_object(builder, "kurosaka_radio"));
    widgets->siames_radio = GTK_WIDGET(gtk_builder_get_object(builder, "siames_radio"));
    widgets->loubere_radio = GTK_WIDGET(gtk_builder_get_object(builder, "loubere_radio"));
    widgets->l_radio = GTK_WIDGET(gtk_builder_get_object(builder, "l_radio"));
    widgets->alterno_radio = GTK_WIDGET(gtk_builder_get_object(builder, "alterno_radio"));
    
    // Conectar señales de botones
    GtkWidget *generate_button = GTK_WIDGET(gtk_builder_get_object(builder, "generate_button"));
    GtkWidget *clear_button = GTK_WIDGET(gtk_builder_get_object(builder, "clear_button"));
    
    g_signal_connect(generate_button, "clicked", G_CALLBACK(on_generate_button_clicked), widgets);
    g_signal_connect(widgets->validate_button, "clicked", G_CALLBACK(on_validate_button_clicked), widgets);
    g_signal_connect(clear_button, "clicked", G_CALLBACK(on_clear_button_clicked), widgets);
    g_signal_connect(widgets->main_window, "destroy", G_CALLBACK(on_main_window_destroy), widgets);
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);
    
    // Crear estructura de widgets
    app_widgets = g_malloc(sizeof(AppWidgets));
    app_widgets->cuadro_actual = NULL;
    
    // Cargar la interfaz desde el archivo Glade
    GtkBuilder *builder = gtk_builder_new();
    GError *error = NULL;
    
    if (!gtk_builder_add_from_file(builder, "cuadros_magicos.glade", &error)) {
        g_printerr("Error al cargar la interfaz: %s\n", error->message);
        g_clear_error(&error);
        return 1;
    }
    
    // Conectar señales
    conectar_señales(builder, app_widgets);
    
    // Mostrar la ventana principal
    gtk_widget_show_all(app_widgets->main_window);
    
    // Inicializar status
    gtk_label_set_text(GTK_LABEL(app_widgets->status_label), 
                      "Listo para generar cuadro mágico...");
    
    printf("=== GENERADOR DE CUADROS MÁGICOS ===\n");
    printf("Algoritmos disponibles:\n");
    printf("• Kurosaka (Movimiento noreste)\n");
    printf("• Siamés (Diagonal clásico)\n");
    printf("• De la Loubère\n");
    printf("• Método L\n");
    printf("• Alterno\n");
    printf("=====================================\n\n");
    
    // Ejecutar el bucle principal de GTK
    gtk_main();
    
    // Limpiar
    g_object_unref(builder);
    g_free(app_widgets);
    
    return 0;
}
