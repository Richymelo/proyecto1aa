#!/bin/bash
# Script de compilación para Cuadros Mágicos
# Proyecto de Análisis de Algoritmos

echo "=================================="
echo "  COMPILADOR DE CUADROS MÁGICOS"
echo "=================================="

# Verificar que GTK esté instalado
if ! pkg-config --exists gtk+-3.0; then
    echo "ERROR: GTK+3 no está instalado"
    echo "Instale con: sudo apt install libgtk-3-dev"
    exit 1
fi

echo "Compilando versiones..."

# Compilar versión automática (GTK Simple)
echo "- Versión automática..."
gcc -std=c99 $(pkg-config --cflags gtk+-3.0) main_gtk_simple.c $(pkg-config --libs gtk+-3.0) -o cuadros_magicos_gtk

# Compilar versión interactiva
echo "- Versión interactiva..."
gcc -std=c99 $(pkg-config --cflags gtk+-3.0) cuadros_magicos_interactivo_completo.c $(pkg-config --libs gtk+-3.0) -o cuadros_magicos_completo

# Compilar versión de consola (si se desea)
echo "- Versión de consola..."
gcc -std=c99 main_console.c cuadros_magicos.c movimientos.c -o cuadros_magicos_consola

echo ""
echo "¡Compilación completada!"
echo ""
echo "Ejecutables creados:"
echo "  ./cuadros_magicos_gtk       (Versión automática)"
echo "  ./cuadros_magicos_completo  (Versión paso a paso)"
echo "  ./cuadros_magicos_consola   (Versión de consola)"
echo ""

