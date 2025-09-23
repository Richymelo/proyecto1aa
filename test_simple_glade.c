/*
 * Test usando el archivo .glade simplificado
 */

#include <gtk/gtk.h>
#include <glib.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    printf("=== TEST CON ARCHIVO GLADE SIMPLIFICADO ===\n");
    printf("Paso 1: Iniciando programa...\n");
    fflush(stdout);
    
    printf("Paso 2: Inicializando GTK...\n");
    fflush(stdout);
    
    if (!gtk_init_check(&argc, &argv)) {
        printf("ERROR: No se pudo inicializar GTK\n");
        return 1;
    }
    
    printf("Paso 3: GTK inicializado correctamente\n");
    fflush(stdout);
    
    printf("Paso 4: Creando builder...\n");
    fflush(stdout);
    
    GtkBuilder *builder = gtk_builder_new();
    if (!builder) {
        printf("ERROR: No se pudo crear el builder\n");
        return 1;
    }
    
    printf("Paso 5: Verificando archivo .glade simplificado...\n");
    fflush(stdout);
    
    if (!g_file_test("cuadros_magicos_simple.glade", G_FILE_TEST_EXISTS)) {
        printf("ERROR: El archivo cuadros_magicos_simple.glade no existe\n");
        g_object_unref(builder);
        return 1;
    }
    
    printf("Paso 6: Intentando cargar archivo .glade simplificado...\n");
    fflush(stdout);
    
    GError *error = NULL;
    if (!gtk_builder_add_from_file(builder, "cuadros_magicos_simple.glade", &error)) {
        printf("ERROR cargando archivo .glade simplificado: %s\n", error->message);
        g_clear_error(&error);
        g_object_unref(builder);
        return 1;
    }
    
    printf("Paso 7: ¡Archivo .glade simplificado cargado exitosamente!\n");
    fflush(stdout);
    
    printf("Paso 8: Obteniendo widget principal...\n");
    fflush(stdout);
    
    GtkWidget *main_window = GTK_WIDGET(gtk_builder_get_object(builder, "main_window"));
    if (!main_window) {
        printf("ERROR: No se pudo obtener main_window\n");
        g_object_unref(builder);
        return 1;
    }
    
    printf("Paso 9: Widget principal obtenido correctamente\n");
    fflush(stdout);
    
    printf("Paso 10: Verificando otros widgets importantes...\n");
    fflush(stdout);
    
    // Verificar widgets críticos
    const char* widgets[] = {
        "size_spin", "create_grid_button", "method_frame", "fill_frame",
        "start_filling_button", "next_step_button", "auto_complete_button",
        "reset_button", "magic_square_grid", "sums_textview",
        "current_number_label", "position_label", "magic_sum_label",
        "progress_label", "siames_radio", "louber_radio", "l_radio", "caracol_radio"
    };
    
    int found_widgets = 0;
    int total_widgets = sizeof(widgets) / sizeof(widgets[0]);
    
    for (int i = 0; i < total_widgets; i++) {
        GtkWidget *widget = GTK_WIDGET(gtk_builder_get_object(builder, widgets[i]));
        if (widget) {
            printf("  ✓ %s encontrado\n", widgets[i]);
            found_widgets++;
        } else {
            printf("  ❌ %s NO encontrado\n", widgets[i]);
        }
    }
    
    printf("Widgets encontrados: %d/%d\n", found_widgets, total_widgets);
    fflush(stdout);
    
    printf("Paso 11: Conectando señal de cierre...\n");
    fflush(stdout);
    
    g_signal_connect(main_window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    
    printf("Paso 12: Mostrando ventana...\n");
    fflush(stdout);
    
    gtk_widget_show_all(main_window);
    
    printf("=== TEST COMPLETADO EXITOSAMENTE ===\n");
    printf("La ventana debería estar visible ahora.\n");
    printf("Cierra la ventana para terminar el programa.\n");
    fflush(stdout);
    
    // Iniciar loop de eventos
    gtk_main();
    
    printf("Programa terminado correctamente\n");
    
    // Limpiar
    g_object_unref(builder);
    
    return 0;
}




