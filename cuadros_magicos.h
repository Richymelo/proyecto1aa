#ifndef CUADROS_MAGICOS_H
#define CUADROS_MAGICOS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Definir max aquí para evitar dependencia de movimientos.h
#define max 21

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

// Funciones principales
CuadroMagico* crear_cuadro_magico(int n, TipoAlgoritmo algoritmo);
void liberar_cuadro_magico(CuadroMagico* cuadro);
bool validar_cuadro_magico(CuadroMagico* cuadro);
void imprimir_cuadro_magico(CuadroMagico* cuadro);

// Implementación del algoritmo de Kurosaka
int metodo_kurosaka(int matriz[max][max], int n, int fila, int columna);
CuadroMagico* generar_kurosaka(int n);

// Funciones auxiliares
void limpiar_matriz(int matriz[max][max], int n);
int calcular_suma_magica(int n);
bool validar_suma_fila(int matriz[max][max], int n, int fila, int suma_esperada);
bool validar_suma_columna(int matriz[max][max], int n, int columna, int suma_esperada);
bool validar_suma_diagonal_principal(int matriz[max][max], int n, int suma_esperada);
bool validar_suma_diagonal_secundaria(int matriz[max][max], int n, int suma_esperada);

// Función para obtener la posición de inicio según el algoritmo
void obtener_posicion_inicio(int n, TipoAlgoritmo algoritmo, int* fila, int* columna);

#endif // CUADROS_MAGICOS_H
