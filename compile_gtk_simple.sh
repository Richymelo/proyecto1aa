#!/bin/bash
# Script para compilar la versión GTK simplificada

echo "=========================================="
echo " Compilando Generador de Cuadros Mágicos"
echo "           Versión GTK"
echo "=========================================="

# Verificar si estamos en MSYS2
if [[ ! "$MSYSTEM" ]]; then
    echo "ADVERTENCIA: Este script está diseñado para MSYS2"
    echo "Asegúrese de ejecutarlo desde el terminal MSYS2"
fi

# Verificar dependencias GTK
echo "Verificando dependencias..."

if ! command -v pkg-config &> /dev/null; then
    echo "ERROR: pkg-config no está instalado"
    echo "Instale con: pacman -S mingw-w64-ucrt-x86_64-pkg-config"
    exit 1
fi

if ! pkg-config --exists gtk+-3.0; then
    echo "ERROR: GTK+3 no está instalado"
    echo "Instale con: pacman -S mingw-w64-ucrt-x86_64-gtk3"
    exit 1
fi

echo "✓ pkg-config encontrado"
echo "✓ GTK+3 encontrado, versión: $(pkg-config --modversion gtk+-3.0)"

# Verificar si existe el archivo glade
if [ ! -f "cuadros_magicos.glade" ]; then
    echo "ERROR: No se encuentra el archivo cuadros_magicos.glade"
    echo "Asegúrese de que esté en el mismo directorio"
    exit 1
fi

echo "✓ Archivo cuadros_magicos.glade encontrado"

# Compilar
echo ""
echo "Compilando..."

gcc -Wall -Wextra -std=c99 \
    $(pkg-config --cflags gtk+-3.0) \
    main_gtk_simple.c \
    -o cuadros_magicos_gtk.exe \
    $(pkg-config --libs gtk+-3.0)

# Verificar resultado
if [ $? -eq 0 ]; then
    echo ""
    echo "🎉 ¡Compilación exitosa!"
    echo "Ejecutable creado: cuadros_magicos_gtk.exe"
    echo ""
    echo "Para ejecutar:"
    echo "./cuadros_magicos_gtk.exe"
    echo ""
    
    read -p "¿Desea ejecutar el programa ahora? (s/n): " ejecutar
    if [[ $ejecutar =~ ^[Ss]$ ]]; then
        echo ""
        echo "Iniciando interfaz gráfica..."
        ./cuadros_magicos_gtk.exe
    fi
else
    echo ""
    echo "❌ Error en la compilación"
    echo "Verifique que todas las dependencias estén instaladas"
    exit 1
fi
