/*
                Esta sección contiene las declaraciones de las funciones de movimientos
                en las cuales se pueden colocar los números. Está el método
                Siames, el movimiento en L, la espiral y el método alterno
                al Siames.
*/

#ifndef MOVIMIENTOS_H
#define MOVIMIENTOS_H

#define max 21  // Tamaño máximo para la matriz

// Declaraciones de las funciones de movimientos
int metodoSiames(int matriz[max][max], int n, int fila, int columna);
int metodoEnL(int matriz[max][max], int n, int fila, int columna);
int metodoLouber(int matriz[max][max], int n, int fila, int columna);
int metodoAlterno(int matriz[max][max], int n, int fila, int columna);

#endif // MOVIMIENTOS_H