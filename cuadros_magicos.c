#include "cuadros_magicos.h"

// No incluir movimientos.h aquí para evitar definiciones múltiples

// Declaraciones de las funciones de movimientos.h para que el compilador las conozca
extern int metodoSiames(int matriz[21][21], int n, int fila, int columna);
extern int metodoEnL(int matriz[21][21], int n, int fila, int columna);
extern int metodoLouber(int matriz[21][21], int n, int fila, int columna);
extern int metodoAlterno(int matriz[21][21], int n, int fila, int columna);

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
    switch (algoritmo) {
        case ALGORITMO_KUROSAKA:
            // Kurosaka: centro de la primera fila
            *fila = 0;
            *columna = n / 2;
            break;
        case ALGORITMO_SIAMES:
            // Siamés: centro de la primera fila
            *fila = 0;
            *columna = n / 2;
            break;
        case ALGORITMO_LOUBERE:
            // De la Loubère: centro de la primera fila
            *fila = 0;
            *columna = n / 2;
            break;
        case ALGORITMO_L:
            // Método L: centro de la primera fila
            *fila = 0;
            *columna = n / 2;
            break;
        case ALGORITMO_ALTERNO:
            // Alterno: centro de la primera fila
            *fila = 0;
            *columna = n / 2;
            break;
        default:
            *fila = 0;
            *columna = n / 2;
            break;
    }
}

// Genera un cuadro mágico usando el algoritmo de Kurosaka
CuadroMagico* generar_kurosaka(int n) {
    if (n % 2 == 0 || n < 3 || n > max) {
        return NULL; // Solo para cuadros impares entre 3 y max
    }
    
    CuadroMagico* cuadro = (CuadroMagico*)malloc(sizeof(CuadroMagico));
    if (!cuadro) return NULL;
    
    cuadro->tamaño = n;
    cuadro->suma_magica = calcular_suma_magica(n);
    limpiar_matriz(cuadro->matriz, n);
    
    int fila, columna;
    obtener_posicion_inicio(n, ALGORITMO_KUROSAKA, &fila, &columna);
    
    // Colocar los números del 1 al n²
    for (int numero = 1; numero <= n * n; numero++) {
        cuadro->matriz[fila][columna] = numero;
        
        if (numero < n * n) { // No calcular siguiente posición para el último número
            int siguiente_pos = metodo_kurosaka(cuadro->matriz, n, fila, columna);
            fila = siguiente_pos / max;
            columna = siguiente_pos % max;
        }
    }
    
    cuadro->es_valido = validar_cuadro_magico(cuadro);
    return cuadro;
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
