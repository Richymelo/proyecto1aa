/*
 * Test para verificar si el problema está en el parser XML
 */

#include <gtk/gtk.h>
#include <glib.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    printf("=== TEST DEL PARSER XML ===\n");
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
    
    printf("Paso 5: Test 1 - Cargar XML mínimo desde string...\n");
    fflush(stdout);
    
    // Test con XML mínimo en memoria
    const char *minimal_xml = 
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
        "<interface>\n"
        "  <requires lib=\"gtk+\" version=\"3.20\"/>\n"
        "  <object class=\"GtkWindow\" id=\"test_window\">\n"
        "    <property name=\"title\">Test Window</property>\n"
        "    <property name=\"default_width\">400</property>\n"
        "    <property name=\"default_height\">300</property>\n"
        "  </object>\n"
        "</interface>\n";
    
    GError *error = NULL;
    
    // Intentar cargar XML desde string
    if (!gtk_builder_add_from_string(builder, minimal_xml, -1, &error)) {
        printf("ERROR en Test 1 - XML desde string: %s\n", error->message);
        g_clear_error(&error);
        g_object_unref(builder);
        return 1;
    }
    
    printf("✓ Test 1 exitoso - XML mínimo cargado desde string\n");
    fflush(stdout);
    
    // Verificar que el widget se creó
    GtkWidget *test_window = GTK_WIDGET(gtk_builder_get_object(builder, "test_window"));
    if (!test_window) {
        printf("ERROR: No se pudo obtener test_window\n");
        g_object_unref(builder);
        return 1;
    }
    
    printf("✓ Widget test_window obtenido correctamente\n");
    fflush(stdout);
    
    // Limpiar builder para el siguiente test
    g_object_unref(builder);
    
    printf("Paso 6: Test 2 - Crear archivo XML mínimo y cargarlo...\n");
    fflush(stdout);
    
    // Crear archivo XML mínimo
    FILE *file = fopen("test_minimal.glade", "w");
    if (!file) {
        printf("ERROR: No se pudo crear archivo test_minimal.glade\n");
        return 1;
    }
    
    fprintf(file, "%s", minimal_xml);
    fclose(file);
    
    printf("✓ Archivo test_minimal.glade creado\n");
    fflush(stdout);
    
    // Crear nuevo builder
    builder = gtk_builder_new();
    if (!builder) {
        printf("ERROR: No se pudo crear el segundo builder\n");
        return 1;
    }
    
    printf("Paso 7: Intentando cargar archivo XML mínimo...\n");
    fflush(stdout);
    
    // Esta línea puede causar segfault si hay problema con lectura de archivos
    if (!gtk_builder_add_from_file(builder, "test_minimal.glade", &error)) {
        printf("ERROR en Test 2 - cargando archivo: %s\n", error->message);
        g_clear_error(&error);
        g_object_unref(builder);
        return 1;
    }
    
    printf("✓ Test 2 exitoso - Archivo XML mínimo cargado\n");
    fflush(stdout);
    
    printf("Paso 8: Test 3 - Cargar archivo .glade simplificado con verificación de contenido...\n");
    fflush(stdout);
    
    // Verificar que el archivo existe y es legible
    if (!g_file_test("cuadros_magicos_simple.glade", G_FILE_TEST_EXISTS)) {
        printf("ERROR: cuadros_magicos_simple.glade no existe\n");
        g_object_unref(builder);
        return 1;
    }
    
    if (!g_file_test("cuadros_magicos_simple.glade", G_FILE_TEST_IS_REGULAR)) {
        printf("ERROR: cuadros_magicos_simple.glade no es un archivo regular\n");
        g_object_unref(builder);
        return 1;
    }
    
    // Verificar tamaño del archivo
    GStatBuf file_stat;
    if (g_stat("cuadros_magicos_simple.glade", &file_stat) != 0) {
        printf("ERROR: No se pudo obtener información del archivo\n");
        g_object_unref(builder);
        return 1;
    }
    
    printf("✓ Archivo cuadros_magicos_simple.glade existe (tamaño: %ld bytes)\n", file_stat.st_size);
    fflush(stdout);
    
    // Leer contenido del archivo para verificar
    gchar *file_contents = NULL;
    gsize file_length = 0;
    
    if (!g_file_get_contents("cuadros_magicos_simple.glade", &file_contents, &file_length, &error)) {
        printf("ERROR leyendo contenido del archivo: %s\n", error->message);
        g_clear_error(&error);
        g_object_unref(builder);
        return 1;
    }
    
    printf("✓ Contenido del archivo leído (longitud: %zu bytes)\n", file_length);
    printf("✓ Primeros 100 caracteres: %.100s...\n", file_contents);
    fflush(stdout);
    
    // Limpiar builder anterior
    g_object_unref(builder);
    builder = gtk_builder_new();
    
    printf("Paso 9: Intentando cargar desde contenido en memoria...\n");
    fflush(stdout);
    
    // Intentar cargar desde el contenido leído
    if (!gtk_builder_add_from_string(builder, file_contents, file_length, &error)) {
        printf("ERROR cargando desde contenido: %s\n", error->message);
        g_clear_error(&error);
        g_free(file_contents);
        g_object_unref(builder);
        return 1;
    }
    
    printf("✓ Archivo cargado exitosamente desde contenido en memoria\n");
    fflush(stdout);
    
    g_free(file_contents);
    
    printf("Paso 10: Ahora intentando cargar directamente desde archivo...\n");
    fflush(stdout);
    
    // Crear nuevo builder para test final
    g_object_unref(builder);
    builder = gtk_builder_new();
    
    // ESTE ES EL PUNTO CRÍTICO - donde ocurre el segfault
    if (!gtk_builder_add_from_file(builder, "cuadros_magicos_simple.glade", &error)) {
        printf("ERROR en carga final: %s\n", error->message);
        g_clear_error(&error);
        g_object_unref(builder);
        return 1;
    }
    
    printf("=== TODOS LOS TESTS EXITOSOS ===\n");
    printf("Si ves este mensaje, el problema no está en el parser XML básico\n");
    
    // Limpiar
    g_object_unref(builder);
    remove("test_minimal.glade");
    
    return 0;
}


