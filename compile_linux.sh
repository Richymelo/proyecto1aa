#!/bin/bash
# Script de compilación para Linux

echo "============================================="
echo " Generador de Cuadros Mágicos - Linux"
echo "============================================="

# Colores para mejor visualización
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Función para verificar dependencias
check_dependencies() {
    echo -e "${BLUE}Verificando dependencias...${NC}"
    
    # Verificar gcc
    if ! command -v gcc &> /dev/null; then
        echo -e "${RED}ERROR: gcc no está instalado${NC}"
        echo "Instale con:"
        echo "  Ubuntu/Debian: sudo apt install build-essential"
        echo "  Fedora/RHEL:   sudo dnf install gcc"
        echo "  Arch Linux:    sudo pacman -S base-devel"
        return 1
    fi
    echo -e "${GREEN}✓ gcc encontrado${NC}"
    
    # Verificar pkg-config
    if ! command -v pkg-config &> /dev/null; then
        echo -e "${RED}ERROR: pkg-config no está instalado${NC}"
        echo "Instale con:"
        echo "  Ubuntu/Debian: sudo apt install pkg-config"
        echo "  Fedora/RHEL:   sudo dnf install pkgconf-devel"
        echo "  Arch Linux:    sudo pacman -S pkgconf"
        return 1
    fi
    echo -e "${GREEN}✓ pkg-config encontrado${NC}"
    
    return 0
}

# Función para verificar GTK
check_gtk() {
    if ! pkg-config --exists gtk+-3.0; then
        echo -e "${RED}ERROR: GTK+3 no está instalado${NC}"
        echo "Instale con:"
        echo "  Ubuntu/Debian: sudo apt install libgtk-3-dev"
        echo "  Fedora/RHEL:   sudo dnf install gtk3-devel"
        echo "  Arch Linux:    sudo pacman -S gtk3"
        return 1
    fi
    
    echo -e "${GREEN}✓ GTK+3 encontrado, versión: $(pkg-config --modversion gtk+-3.0)${NC}"
    return 0
}

# Función para compilar versión de consola
compile_console() {
    echo -e "${BLUE}Compilando versión de consola...${NC}"
    
    gcc -Wall -Wextra -std=c99 compile_simple.c -o cuadros_magicos_console
    
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}✓ Versión de consola compilada exitosamente${NC}"
        echo "Ejecutable: ./cuadros_magicos_console"
        return 0
    else
        echo -e "${RED}❌ Error compilando versión de consola${NC}"
        return 1
    fi
}

# Función para compilar versión GTK
compile_gtk() {
    echo -e "${BLUE}Compilando versiones GTK...${NC}"
    
    # Verificar archivos glade
    if [ ! -f "cuadros_magicos.glade" ]; then
        echo -e "${RED}ERROR: No se encuentra cuadros_magicos.glade${NC}"
        return 1
    fi
    
    if [ ! -f "cuadros_magicos_interactivo.glade" ]; then
        echo -e "${RED}ERROR: No se encuentra cuadros_magicos_interactivo.glade${NC}"
        return 1
    fi
    
    # Compilar versión simplificada
    echo "Compilando versión simplificada..."
    gcc -Wall -Wextra -std=c99 \
        $(pkg-config --cflags gtk+-3.0) \
        main_gtk_simple.c \
        $(pkg-config --libs gtk+-3.0) \
        -o cuadros_magicos_gtk_simple
    
    simple_ok=$?
    
    # Compilar versión interactiva
    echo "Compilando versión interactiva..."
    gcc -Wall -Wextra -std=c99 \
        $(pkg-config --cflags gtk+-3.0) \
        main_gtk_interactivo.c \
        $(pkg-config --libs gtk+-3.0) \
        -o cuadros_magicos_interactivo
    
    interactive_ok=$?
    
    # Reportar resultados
    if [ $simple_ok -eq 0 ]; then
        echo -e "${GREEN}✓ Versión GTK simplificada compilada exitosamente${NC}"
    else
        echo -e "${RED}❌ Error compilando versión GTK simplificada${NC}"
    fi
    
    if [ $interactive_ok -eq 0 ]; then
        echo -e "${GREEN}✓ Versión GTK interactiva compilada exitosamente${NC}"
    else
        echo -e "${RED}❌ Error compilando versión GTK interactiva${NC}"
    fi
    
    if [ $interactive_ok -eq 0 ]; then
        echo ""
        echo -e "${GREEN}🎉 Ejecutables creados:${NC}"
        echo "  - ./cuadros_magicos_gtk_simple (versión simplificada)"
        echo "  - ./cuadros_magicos_interactivo (versión interactiva - RECOMENDADA)"
        echo ""
        echo -e "${YELLOW}NOTA: Asegúrese de que los archivos .glade estén en el mismo directorio${NC}"
        return 0
    elif [ $simple_ok -eq 0 ]; then
        echo ""
        echo -e "${YELLOW}Solo versión simplificada disponible${NC}"
        echo "Ejecutable: ./cuadros_magicos_gtk_simple"
        return 0
    else
        return 1
    fi
}

# Función principal
main() {
    # Verificar dependencias básicas
    if ! check_dependencies; then
        exit 1
    fi
    
    echo ""
    echo "Seleccione qué compilar:"
    echo "1) Versión de consola"
    echo "2) Versión GTK"
    echo "3) Ambas versiones"
    echo "4) Limpiar archivos compilados"
    echo "5) Instalar dependencias (Ubuntu/Debian)"
    echo "6) Instalar dependencias (Fedora/RHEL)"
    echo "7) Instalar dependencias (Arch Linux)"
    echo "8) Salir"
    
    read -p "Opción (1-8): " opcion
    
    case $opcion in
        1)
            compile_console
            if [ $? -eq 0 ]; then
                read -p "¿Ejecutar versión de consola? (s/n): " ejecutar
                if [[ $ejecutar =~ ^[Ss]$ ]]; then
                    echo ""
                    ./cuadros_magicos_console
                fi
            fi
            ;;
        2)
            if check_gtk; then
                compile_gtk
                if [ $? -eq 0 ]; then
                    echo ""
                    echo "Versiones disponibles:"
                    echo "1) Versión interactiva (recomendada)"
                    echo "2) Versión simplificada"
                    read -p "¿Qué versión ejecutar? (1/2/n): " version
                    case $version in
                        1)
                            if [ -f "./cuadros_magicos_interactivo" ]; then
                                echo "Iniciando versión interactiva..."
                                ./cuadros_magicos_interactivo
                            else
                                echo -e "${RED}Versión interactiva no disponible${NC}"
                            fi
                            ;;
                        2)
                            if [ -f "./cuadros_magicos_gtk_simple" ]; then
                                echo "Iniciando versión simplificada..."
                                ./cuadros_magicos_gtk_simple
                            else
                                echo -e "${RED}Versión simplificada no disponible${NC}"
                            fi
                            ;;
                        *)
                            echo "No ejecutando..."
                            ;;
                    esac
                fi
            fi
            ;;
        3)
            compile_console
            console_ok=$?
            echo ""
            
            if check_gtk; then
                compile_gtk
                gtk_ok=$?
            else
                gtk_ok=1
            fi
            
            echo ""
            if [ $console_ok -eq 0 ] && [ $gtk_ok -eq 0 ]; then
                echo -e "${GREEN}🎉 Ambas versiones compiladas exitosamente${NC}"
            elif [ $console_ok -eq 0 ]; then
                echo -e "${YELLOW}⚠️  Solo la versión de consola compilada (GTK no disponible)${NC}"
            else
                echo -e "${RED}❌ Error en la compilación${NC}"
            fi
            ;;
        4)
            echo "Limpiando archivos compilados..."
            rm -f cuadros_magicos_console cuadros_magicos_gtk cuadros_magicos_gtk_simple cuadros_magicos_interactivo *.o
            echo -e "${GREEN}✓ Archivos limpiados${NC}"
            ;;
        5)
            echo "Instalando dependencias para Ubuntu/Debian..."
            sudo apt update
            sudo apt install -y build-essential pkg-config libgtk-3-dev
            ;;
        6)
            echo "Instalando dependencias para Fedora/RHEL..."
            sudo dnf install -y gcc pkgconf-devel gtk3-devel
            ;;
        7)
            echo "Instalando dependencias para Arch Linux..."
            sudo pacman -S base-devel pkgconf gtk3
            ;;
        8)
            echo "Saliendo..."
            exit 0
            ;;
        *)
            echo -e "${RED}Opción inválida${NC}"
            exit 1
            ;;
    esac
}

# Hacer el script ejecutable
chmod +x "$0" 2>/dev/null

# Ejecutar función principal
main

