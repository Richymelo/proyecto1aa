/*
 * Generador de Cuadros Mágicos - Versión GTK
 * Proyecto de Análisis de Algoritmos
 * 
 * Implementación gráfica que genera cuadros mágicos automáticamente
 * usando diferentes algoritmos de llenado.
 */

#include <gtk/gtk.h>
#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define max 21  // Tamaño máximo para la matriz

// ============= FUNCIONES DE MOVIMIENTOS =============

// Método Siamés clásico: mueve en diagonal izquierda-abajo y ajusta si está ocupado
int metodoSiames(int matriz[max][max], int n, int fila, int columna) {
    int nuevaFila = (fila - 1 + n) % n;
    int nuevaColumna = (columna + 1) % n;

    if (matriz[nuevaFila][nuevaColumna] != 0) { 
        nuevaFila = (fila + 1) % n;
        nuevaColumna = columna;
    }

    return nuevaFila * max + nuevaColumna;
}

// Método en L: movimiento en forma de caballo del ajedrez
int metodoEnL(int matriz[max][max], int n, int fila, int columna) {
    int nuevaFila = (fila - 2 + n) % n;
    int nuevaColumna = (columna + 1) % n;
    if (matriz[nuevaFila][nuevaColumna] != 0) {
        nuevaFila = (fila + 1) % n;
        nuevaColumna = columna;
    }
    return nuevaFila * max + nuevaColumna;
}

// Método De la Loubère: mueve en diagonal derecha-arriba y ajusta si está ocupado
int metodoLouber(int matriz[max][max], int n, int fila, int columna) {
    int nuevaFila = (fila + 1) % n;
    int nuevaColumna = (columna - 1 + n) % n;

    if (matriz[nuevaFila][nuevaColumna] != 0) {
        nuevaFila = (fila - 1 + n) % n;
        nuevaColumna = columna;
    }

    return nuevaFila * max + nuevaColumna;
}

// Método Alterno de Diagonales: variante del método siamés
int metodoAlterno(int matriz[max][max], int n, int fila, int columna) {
    int nuevaFila = (fila - 1 + n) % n;
    int nuevaColumna = (columna - 1 + n) % n;
    if (matriz[nuevaFila][nuevaColumna] != 0) {
        nuevaFila = (fila + 1) % n;
        nuevaColumna = columna;
    }
    return nuevaFila * max + nuevaColumna;
}

// ============= ALGORITMO DE KUROSAKA =============

// Implementación del algoritmo de Kurosaka (movimiento noreste con break-move)
int metodo_kurosaka(int matriz[max][max], int n, int fila, int columna) {
    // Movimiento noreste: arriba (-1) y derecha (+1)
    int nuevaFila = (fila - 1 + n) % n;
    int nuevaColumna = (columna + 1) % n;
    
    // Si la celda está ocupada, aplicar break-move (mover hacia abajo)
    if (matriz[nuevaFila][nuevaColumna] != 0) {
        nuevaFila = (fila + 1) % n;
        nuevaColumna = columna;
    }
    
    return nuevaFila * max + nuevaColumna;
}

// ============= ESTRUCTURAS Y ENUMS =============

// Estructura para almacenar información del cuadro mágico
typedef struct {
    int matriz[max][max];
    int tamaño;
    int suma_magica;
    bool es_valido;
} CuadroMagico;

// Enumeración para los diferentes algoritmos
typedef enum {
    ALGORITMO_KUROSAKA,
    ALGORITMO_SIAMES,
    ALGORITMO_LOUBERE,
    ALGORITMO_L,
    ALGORITMO_ALTERNO
} TipoAlgoritmo;

// Estructura para manejar los widgets de la aplicación
typedef struct {
    GtkWidget *main_window;
    GtkWidget *size_spin;
    GtkWidget *magic_square_grid;
    GtkWidget *status_label;
    GtkWidget *validation_label;
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

// ============= FUNCIONES AUXILIARES =============

// Limpia la matriz inicializándola con ceros
void limpiar_matriz(int matriz[max][max], int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            matriz[i][j] = 0;
        }
    }
}

// Calcula la suma mágica para un cuadro de tamaño n
int calcular_suma_magica(int n) {
    return (n * (n * n + 1)) / 2;
}

// Obtiene la posición de inicio según el algoritmo seleccionado
void obtener_posicion_inicio(int n, TipoAlgoritmo algoritmo, int* fila, int* columna) {
    // Para evitar warning de parámetro no usado
    (void)algoritmo;
    
    // Todos los algoritmos empiezan en el centro de la primera fila
    *fila = 0;
    *columna = n / 2;
}

// Valida si la suma de una fila es correcta
bool validar_suma_fila(int matriz[max][max], int n, int fila, int suma_esperada) {
    int suma = 0;
    for (int j = 0; j < n; j++) {
        suma += matriz[fila][j];
    }
    return suma == suma_esperada;
}

// Valida si la suma de una columna es correcta
bool validar_suma_columna(int matriz[max][max], int n, int columna, int suma_esperada) {
    int suma = 0;
    for (int i = 0; i < n; i++) {
        suma += matriz[i][columna];
    }
    return suma == suma_esperada;
}

// Valida si la suma de la diagonal principal es correcta
bool validar_suma_diagonal_principal(int matriz[max][max], int n, int suma_esperada) {
    int suma = 0;
    for (int i = 0; i < n; i++) {
        suma += matriz[i][i];
    }
    return suma == suma_esperada;
}

// Valida si la suma de la diagonal secundaria es correcta
bool validar_suma_diagonal_secundaria(int matriz[max][max], int n, int suma_esperada) {
    int suma = 0;
    for (int i = 0; i < n; i++) {
        suma += matriz[i][n - 1 - i];
    }
    return suma == suma_esperada;
}

// Valida si el cuadro es realmente mágico
bool validar_cuadro_magico(CuadroMagico* cuadro) {
    if (!cuadro) return false;
    
    int n = cuadro->tamaño;
    int suma_esperada = cuadro->suma_magica;
    
    // Validar todas las filas
    for (int i = 0; i < n; i++) {
        if (!validar_suma_fila(cuadro->matriz, n, i, suma_esperada)) {
            return false;
        }
    }
    
    // Validar todas las columnas
    for (int j = 0; j < n; j++) {
        if (!validar_suma_columna(cuadro->matriz, n, j, suma_esperada)) {
            return false;
        }
    }
    
    // Validar diagonal principal
    if (!validar_suma_diagonal_principal(cuadro->matriz, n, suma_esperada)) {
        return false;
    }
    
    // Validar diagonal secundaria
    if (!validar_suma_diagonal_secundaria(cuadro->matriz, n, suma_esperada)) {
        return false;
    }
    
    return true;
}

// Crea un cuadro mágico usando el algoritmo especificado
CuadroMagico* crear_cuadro_magico(int n, TipoAlgoritmo algoritmo) {
    if (n % 2 == 0 || n < 3 || n > max) {
        return NULL; // Solo para cuadros impares
    }
    
    CuadroMagico* cuadro = (CuadroMagico*)malloc(sizeof(CuadroMagico));
    if (!cuadro) return NULL;
    
    cuadro->tamaño = n;
    cuadro->suma_magica = calcular_suma_magica(n);
    limpiar_matriz(cuadro->matriz, n);
    
    int fila, columna;
    obtener_posicion_inicio(n, algoritmo, &fila, &columna);
    
    // Función de movimiento según el algoritmo
    int (*funcion_movimiento)(int[max][max], int, int, int) = NULL;
    
    switch (algoritmo) {
        case ALGORITMO_KUROSAKA:
            funcion_movimiento = metodo_kurosaka;
            break;
        case ALGORITMO_SIAMES:
            funcion_movimiento = metodoSiames;
            break;
        case ALGORITMO_LOUBERE:
            funcion_movimiento = metodoLouber;
            break;
        case ALGORITMO_L:
            funcion_movimiento = metodoEnL;
            break;
        case ALGORITMO_ALTERNO:
            funcion_movimiento = metodoAlterno;
            break;
        default:
            funcion_movimiento = metodo_kurosaka;
            break;
    }
    
    // Generar el cuadro mágico
    for (int numero = 1; numero <= n * n; numero++) {
        cuadro->matriz[fila][columna] = numero;
        
        if (numero < n * n) { // No calcular siguiente posición para el último número
            int siguiente_pos = funcion_movimiento(cuadro->matriz, n, fila, columna);
            fila = siguiente_pos / max;
            columna = siguiente_pos % max;
        }
    }
    
    cuadro->es_valido = validar_cuadro_magico(cuadro);
    return cuadro;
}

// Libera la memoria del cuadro mágico
void liberar_cuadro_magico(CuadroMagico* cuadro) {
    if (cuadro) {
        free(cuadro);
    }
}

// Función imprimir_cuadro_magico removida - solo interfaz gráfica

// ============= FUNCIONES GTK =============

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
        
        // Limpiar validación anterior
        gtk_label_set_text(GTK_LABEL(widgets->validation_label), "-");
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
    
    // Actualizar label de validación
    gtk_label_set_text(GTK_LABEL(widgets->validation_label), 
                      es_valido ? "VÁLIDO ✓" : "INVÁLIDO ✗");
    
    // Actualizar status con información detallada
    char status_text[200];
    snprintf(status_text, sizeof(status_text), 
            "Validación completada - Suma mágica: %d", 
            widgets->cuadro_actual->suma_magica);
    gtk_label_set_text(GTK_LABEL(widgets->status_label), status_text);
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
    
    // Limpiar validación
    gtk_label_set_text(GTK_LABEL(widgets->validation_label), "-");
    
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
    widgets->validation_label = GTK_WIDGET(gtk_builder_get_object(builder, "validation_label"));
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
    
    // Ejecutar el bucle principal de GTK
    gtk_main();
    
    // Limpiar
    g_object_unref(builder);
    g_free(app_widgets);
    
    return 0;
}
