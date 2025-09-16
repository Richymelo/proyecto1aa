/*
                Esta sección contiene las funciones de los movimientos
                en las cuales se pueden colocar los números. Está el método
                Siames, el movimiento en L, la espiral y el método alterno
                al Siames.
*/

#define max 21  // Tamaño máximo para la matriz

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