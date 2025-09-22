/*
 * Archivo de compilación simplificada para evitar problemas de linkeo
 * Incluye todo el código necesario en un solo archivo
 */

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
// (arriba y a la izquierda) y ajusta si está ocupado
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

// Método Alterno de Diagonales: variante del método siamés,
// mueve en diagonal derecha-abajo y ajusta si está ocupado
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

// Imprime el cuadro mágico en la consola
void imprimir_cuadro_magico(CuadroMagico* cuadro) {
    if (!cuadro) {
        printf("Cuadro mágico inválido\n");
        return;
    }
    
    int n = cuadro->tamaño;
    
    printf("\nCuadro Mágico %dx%d (Suma mágica: %d)\n", n, n, cuadro->suma_magica);
    printf("Estado: %s\n", cuadro->es_valido ? "VÁLIDO" : "INVÁLIDO");
    printf("─────────────────────────────────\n");
    
    // Encontrar el ancho máximo para alineación
    int ancho_max = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int ancho = snprintf(NULL, 0, "%d", cuadro->matriz[i][j]);
            if (ancho > ancho_max) ancho_max = ancho;
        }
    }
    
    // Imprimir la matriz
    for (int i = 0; i < n; i++) {
        printf("│ ");
        for (int j = 0; j < n; j++) {
            printf("%*d ", ancho_max, cuadro->matriz[i][j]);
        }
        printf("│\n");
    }
    printf("─────────────────────────────────\n\n");
}

// ============= PROGRAMA PRINCIPAL =============

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
