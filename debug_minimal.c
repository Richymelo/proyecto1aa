/*
 * Programa de diagnóstico mínimo para identificar el segmentation fault
 */

#include <gtk/gtk.h>
#include <glib.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    printf("=== DIAGNÓSTICO DE SEGMENTATION FAULT ===\n");
    printf("Paso 1: Iniciando programa...\n");
    fflush(stdout);
    
    printf("Paso 2: Verificando versión de GTK...\n");
    printf("GTK Major: %d, Minor: %d, Micro: %d\n", 
           gtk_get_major_version(), gtk_get_minor_version(), gtk_get_micro_version());
    fflush(stdout);
    
    printf("Paso 3: Inicializando GTK...\n");
    fflush(stdout);
    
    // Esta línea puede ser la causante del segfault
    if (!gtk_init_check(&argc, &argv)) {
        printf("ERROR: No se pudo inicializar GTK\n");
        printf("Posibles causas:\n");
        printf("- No hay servidor X disponible\n");
        printf("- Variables de entorno DISPLAY no configuradas\n");
        printf("- Librerías GTK dañadas o incompatibles\n");
        return 1;
    }
    
    printf("Paso 4: GTK inicializado correctamente\n");
    fflush(stdout);
    
    printf("Paso 5: Creando builder...\n");
    fflush(stdout);
    
    GtkBuilder *builder = gtk_builder_new();
    if (!builder) {
        printf("ERROR: No se pudo crear el builder\n");
        return 1;
    }
    
    printf("Paso 6: Builder creado correctamente\n");
    fflush(stdout);
    
    printf("Paso 7: Verificando archivo .glade...\n");
    fflush(stdout);
    
    // Verificar si el archivo existe antes de cargarlo
    if (!g_file_test("cuadros_magicos_interactivo.glade", G_FILE_TEST_EXISTS)) {
        printf("ERROR: El archivo cuadros_magicos_interactivo.glade no existe\n");
        printf("Directorio actual: %s\n", g_get_current_dir());
        g_object_unref(builder);
        return 1;
    }
    
    printf("Paso 8: Archivo .glade encontrado\n");
    fflush(stdout);
    
    printf("Paso 9: Intentando cargar archivo .glade...\n");
    fflush(stdout);
    
    GError *error = NULL;
    if (!gtk_builder_add_from_file(builder, "cuadros_magicos_interactivo.glade", &error)) {
        printf("ERROR cargando archivo .glade: %s\n", error->message);
        g_clear_error(&error);
        g_object_unref(builder);
        return 1;
    }
    
    printf("Paso 10: Archivo .glade cargado exitosamente\n");
    fflush(stdout);
    
    printf("Paso 11: Obteniendo widget principal...\n");
    fflush(stdout);
    
    GtkWidget *main_window = GTK_WIDGET(gtk_builder_get_object(builder, "main_window"));
    if (!main_window) {
        printf("ERROR: No se pudo obtener main_window\n");
        g_object_unref(builder);
        return 1;
    }
    
    printf("Paso 12: Widget principal obtenido correctamente\n");
    fflush(stdout);
    
    printf("Paso 13: Conectando señal de cierre...\n");
    fflush(stdout);
    
    g_signal_connect(main_window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    
    printf("Paso 14: Mostrando ventana...\n");
    fflush(stdout);
    
    gtk_widget_show_all(main_window);
    
    printf("Paso 15: Ventana mostrada correctamente\n");
    printf("=== DIAGNÓSTICO COMPLETADO EXITOSAMENTE ===\n");
    printf("Si ves este mensaje, el problema no está en la inicialización básica\n");
    printf("Presiona Ctrl+C para cerrar o cierra la ventana\n");
    fflush(stdout);
    
    // Iniciar loop de eventos por solo 5 segundos para prueba
    printf("Iniciando loop de eventos por 5 segundos...\n");
    
    GMainContext *context = g_main_context_default();
    gint64 end_time = g_get_monotonic_time() + 5 * G_TIME_SPAN_SECOND;
    
    while (g_get_monotonic_time() < end_time) {
        g_main_context_iteration(context, FALSE);
        g_usleep(10000); // 10ms
    }
    
    printf("Test completado. Limpiando recursos...\n");
    
    // Limpiar
    g_object_unref(builder);
    
    printf("=== PROGRAMA TERMINADO CORRECTAMENTE ===\n");
    return 0;
}






