#include <stdio.h>
#include <stdlib.h>
#include "cuadros_magicos.h"
#include "movimientos.h"  // Incluir aquí las funciones de movimientos

void mostrar_menu() {
    printf("\n=== GENERADOR DE CUADROS MÁGICOS ===\n");
    printf("1. Algoritmo de Kurosaka (Noreste)\n");
    printf("2. Método Siamés (Diagonal)\n");
    printf("3. Método De la Loubère\n");
    printf("4. Método L\n");
    printf("5. Método Alterno\n");
    printf("0. Salir\n");
    printf("=====================================\n");
}

TipoAlgoritmo obtener_algoritmo_consola() {
    int opcion;
    
    do {
        mostrar_menu();
        printf("Seleccione un algoritmo (0-5): ");
        
        if (scanf("%d", &opcion) != 1) {
            printf("Error: Entrada inválida.\n");
            // Limpiar buffer de entrada
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            continue;
        }
        
        switch (opcion) {
            case 1: return ALGORITMO_KUROSAKA;
            case 2: return ALGORITMO_SIAMES;
            case 3: return ALGORITMO_LOUBERE;
            case 4: return ALGORITMO_L;
            case 5: return ALGORITMO_ALTERNO;
            case 0: 
                printf("¡Hasta luego!\n");
                exit(0);
            default:
                printf("Opción inválida. Intente de nuevo.\n");
        }
    } while (1);
}

int obtener_tamaño_consola() {
    int tamaño;
    
    do {
        printf("\nIngrese el tamaño del cuadro (número impar entre 3 y %d): ", max);
        
        if (scanf("%d", &tamaño) != 1) {
            printf("Error: Entrada inválida.\n");
            // Limpiar buffer de entrada
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            continue;
        }
        
        if (tamaño < 3 || tamaño > max) {
            printf("Error: El tamaño debe estar entre 3 y %d.\n", max);
            continue;
        }
        
        if (tamaño % 2 == 0) {
            printf("Error: El tamaño debe ser un número impar.\n");
            continue;
        }
        
        return tamaño;
        
    } while (1);
}

void mostrar_estadisticas_detalladas(CuadroMagico* cuadro) {
    if (!cuadro) {
        printf("Error: Cuadro mágico inválido.\n");
        return;
    }
    
    int n = cuadro->tamaño;
    int suma_esperada = cuadro->suma_magica;
    
    printf("\n=== ESTADÍSTICAS DETALLADAS ===\n");
    printf("Tamaño: %dx%d\n", n, n);
    printf("Suma mágica esperada: %d\n", suma_esperada);
    printf("Estado: %s\n", cuadro->es_valido ? "VÁLIDO ✓" : "INVÁLIDO ✗");
    
    // Verificar sumas de filas
    printf("\nSumas por fila:\n");
    for (int i = 0; i < n; i++) {
        int suma = 0;
        for (int j = 0; j < n; j++) {
            suma += cuadro->matriz[i][j];
        }
        printf("  Fila %d: %d %s\n", i+1, suma, 
               (suma == suma_esperada) ? "✓" : "✗");
    }
    
    // Verificar sumas de columnas
    printf("\nSumas por columna:\n");
    for (int j = 0; j < n; j++) {
        int suma = 0;
        for (int i = 0; i < n; i++) {
            suma += cuadro->matriz[i][j];
        }
        printf("  Columna %d: %d %s\n", j+1, suma, 
               (suma == suma_esperada) ? "✓" : "✗");
    }
    
    // Verificar diagonal principal
    int suma_diag_principal = 0;
    for (int i = 0; i < n; i++) {
        suma_diag_principal += cuadro->matriz[i][i];
    }
    printf("\nDiagonal principal: %d %s\n", suma_diag_principal,
           (suma_diag_principal == suma_esperada) ? "✓" : "✗");
    
    // Verificar diagonal secundaria
    int suma_diag_secundaria = 0;
    for (int i = 0; i < n; i++) {
        suma_diag_secundaria += cuadro->matriz[i][n - 1 - i];
    }
    printf("Diagonal secundaria: %d %s\n", suma_diag_secundaria,
           (suma_diag_secundaria == suma_esperada) ? "✓" : "✗");
    
    printf("===============================\n");
}

void mostrar_informacion_algoritmo(TipoAlgoritmo algoritmo) {
    printf("\n=== INFORMACIÓN DEL ALGORITMO ===\n");
    
    switch (algoritmo) {
        case ALGORITMO_KUROSAKA:
            printf("Algoritmo: Kurosaka\n");
            printf("Descripción: Movimiento noreste (arriba-derecha)\n");
            printf("Break-move: Hacia abajo cuando encuentra celda ocupada\n");
            printf("Basado en: Especificación de Robert T. Kurosaka\n");
            break;
        case ALGORITMO_SIAMES:
            printf("Algoritmo: Siamés\n");
            printf("Descripción: Movimiento diagonal izquierda-abajo\n");
            printf("Break-move: Hacia abajo cuando encuentra celda ocupada\n");
            printf("Características: Algoritmo clásico tradicional\n");
            break;
        case ALGORITMO_LOUBERE:
            printf("Algoritmo: De la Loubère\n");
            printf("Descripción: Movimiento diagonal derecha-arriba\n");
            printf("Break-move: Hacia arriba cuando encuentra celda ocupada\n");
            printf("Características: Variante del método siamés\n");
            break;
        case ALGORITMO_L:
            printf("Algoritmo: Método L\n");
            printf("Descripción: Movimiento en L (como caballo de ajedrez)\n");
            printf("Break-move: Hacia abajo cuando encuentra celda ocupada\n");
            printf("Características: Movimiento único en forma de L\n");
            break;
        case ALGORITMO_ALTERNO:
            printf("Algoritmo: Alterno\n");
            printf("Descripción: Movimiento diagonal derecha-abajo\n");
            printf("Break-move: Hacia abajo cuando encuentra celda ocupada\n");
            printf("Características: Variante alternativa de diagonales\n");
            break;
    }
    printf("================================\n");
}

int main() {
    printf("=== BIENVENIDO AL GENERADOR DE CUADROS MÁGICOS ===\n");
    printf("Implementación de algoritmos para cuadros mágicos\n");
    printf("Incluyendo el algoritmo de Robert T. Kurosaka\n");
    printf("==================================================\n");
    
    while (1) {
        // Obtener algoritmo
        TipoAlgoritmo algoritmo = obtener_algoritmo_consola();
        
        // Mostrar información del algoritmo
        mostrar_informacion_algoritmo(algoritmo);
        
        // Obtener tamaño
        int tamaño = obtener_tamaño_consola();
        
        printf("\nGenerando cuadro mágico %dx%d...\n", tamaño, tamaño);
        
        // Generar cuadro mágico
        CuadroMagico* cuadro = crear_cuadro_magico(tamaño, algoritmo);
        
        if (cuadro) {
            // Mostrar el cuadro
            imprimir_cuadro_magico(cuadro);
            
            // Mostrar estadísticas detalladas
            mostrar_estadisticas_detalladas(cuadro);
            
            // Preguntar si desea continuar
            printf("\n¿Desea generar otro cuadro mágico? (s/n): ");
            char respuesta;
            scanf(" %c", &respuesta);
            
            if (respuesta != 's' && respuesta != 'S') {
                liberar_cuadro_magico(cuadro);
                printf("\n¡Gracias por usar el generador de cuadros mágicos!\n");
                break;
            }
            
            // Liberar cuadro actual
            liberar_cuadro_magico(cuadro);
            
        } else {
            printf("Error: No se pudo generar el cuadro mágico.\n");
            printf("Verifique que el tamaño sea válido (impar, entre 3 y %d).\n", max);
        }
    }
    
    return 0;
}
