/*
 * Test para el archivo .glade ultra-simplificado
 */

#include <gtk/gtk.h>
#include <glib.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    printf("=== TEST ARCHIVO GLADE ULTRA-SIMPLIFICADO ===\n");
    
    if (!gtk_init_check(&argc, &argv)) {
        printf("ERROR: No se pudo inicializar GTK\n");
        return 1;
    }
    
    printf("✓ GTK inicializado\n");
    
    GtkBuilder *builder = gtk_builder_new();
    if (!builder) {
        printf("ERROR: No se pudo crear builder\n");
        return 1;
    }
    
    printf("✓ Builder creado\n");
    
    if (!g_file_test("cuadros_magicos_ultra_simple.glade", G_FILE_TEST_EXISTS)) {
        printf("ERROR: Archivo cuadros_magicos_ultra_simple.glade no existe\n");
        g_object_unref(builder);
        return 1;
    }
    
    printf("✓ Archivo encontrado\n");
    
    printf("Intentando cargar archivo ultra-simplificado...\n");
    fflush(stdout);
    
    GError *error = NULL;
    if (!gtk_builder_add_from_file(builder, "cuadros_magicos_ultra_simple.glade", &error)) {
        printf("ERROR cargando archivo: %s\n", error->message);
        g_clear_error(&error);
        g_object_unref(builder);
        return 1;
    }
    
    printf("✓ ¡ARCHIVO CARGADO EXITOSAMENTE!\n");
    
    // Verificar widgets principales
    GtkWidget *main_window = GTK_WIDGET(gtk_builder_get_object(builder, "main_window"));
    if (!main_window) {
        printf("ERROR: main_window no encontrado\n");
        g_object_unref(builder);
        return 1;
    }
    
    printf("✓ main_window encontrado\n");
    
    // Conectar señal de cierre
    g_signal_connect(main_window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    
    // Mostrar ventana
    gtk_widget_show_all(main_window);
    printf("✓ Ventana mostrada\n");
    
    printf("=== TEST COMPLETADO - La aplicación debería estar visible ===\n");
    printf("Cierra la ventana para terminar\n");
    
    // Iniciar loop de eventos
    gtk_main();
    
    g_object_unref(builder);
    return 0;
}


