#!/bin/bash
# Script de prueba de compilación específico para Linux

echo "=============================================="
echo " Test de Compilación - Cuadros Mágicos Linux"
echo "=============================================="

# Colores
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

# Función para verificar dependencias
check_dependencies() {
    echo -e "${BLUE}Verificando dependencias...${NC}"
    
    # Verificar GCC
    if ! command -v gcc &> /dev/null; then
        echo -e "${RED}❌ GCC no encontrado${NC}"
        return 1
    fi
    echo -e "${GREEN}✓ GCC encontrado: $(gcc --version | head -n1)${NC}"
    
    # Verificar pkg-config
    if ! command -v pkg-config &> /dev/null; then
        echo -e "${RED}❌ pkg-config no encontrado${NC}"
        return 1
    fi
    echo -e "${GREEN}✓ pkg-config encontrado${NC}"
    
    # Verificar GTK+3
    if ! pkg-config --exists gtk+-3.0; then
        echo -e "${RED}❌ GTK+3 no encontrado${NC}"
        return 1
    fi
    echo -e "${GREEN}✓ GTK+3 encontrado: $(pkg-config --modversion gtk+-3.0)${NC}"
    
    return 0
}

# Función para verificar archivos necesarios
check_files() {
    echo -e "${BLUE}Verificando archivos...${NC}"
    
    if [ ! -f "main_gtk_interactivo.c" ]; then
        echo -e "${RED}❌ main_gtk_interactivo.c no encontrado${NC}"
        return 1
    fi
    echo -e "${GREEN}✓ main_gtk_interactivo.c encontrado${NC}"
    
    if [ ! -f "cuadros_magicos_interactivo.glade" ]; then
        echo -e "${RED}❌ cuadros_magicos_interactivo.glade no encontrado${NC}"
        return 1
    fi
    echo -e "${GREEN}✓ cuadros_magicos_interactivo.glade encontrado${NC}"
    
    return 0
}

# Función de compilación con verbose
compile_interactive() {
    echo -e "${BLUE}Compilando versión interactiva...${NC}"
    echo ""
    echo "Comando de compilación:"
    echo "gcc -Wall -Wextra -std=c99 \$(pkg-config --cflags gtk+-3.0) main_gtk_interactivo.c \$(pkg-config --libs gtk+-3.0) -o cuadros_magicos_interactivo"
    echo ""
    
    # Mostrar flags que se van a usar
    echo "Flags de compilación:"
    echo "CFLAGS: $(pkg-config --cflags gtk+-3.0)"
    echo "LDFLAGS: $(pkg-config --libs gtk+-3.0)"
    echo ""
    
    # Compilar
    gcc -Wall -Wextra -std=c99 \
        $(pkg-config --cflags gtk+-3.0) \
        main_gtk_interactivo.c \
        $(pkg-config --libs gtk+-3.0) \
        -o cuadros_magicos_interactivo
    
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}✓ Compilación exitosa${NC}"
        echo "Ejecutable creado: ./cuadros_magicos_interactivo"
        
        # Verificar que el ejecutable funciona
        if [ -x "./cuadros_magicos_interactivo" ]; then
            echo -e "${GREEN}✓ Ejecutable tiene permisos correctos${NC}"
            
            # Verificar dependencias del ejecutable
            echo ""
            echo "Dependencias del ejecutable:"
            ldd ./cuadros_magicos_interactivo 2>/dev/null | head -10 || echo "ldd no disponible o ejecutable estático"
            
            return 0
        else
            echo -e "${RED}❌ Ejecutable no tiene permisos de ejecución${NC}"
            return 1
        fi
    else
        echo -e "${RED}❌ Error en la compilación${NC}"
        return 1
    fi
}

# Función principal
main() {
    # Verificar que estamos en el directorio correcto
    if [ ! -f "main_gtk_interactivo.c" ]; then
        echo -e "${YELLOW}Cambiando al directorio proyecto1aa...${NC}"
        if [ -d "proyecto1aa" ]; then
            cd proyecto1aa
        else
            echo -e "${RED}❌ No se encuentra el directorio del proyecto${NC}"
            exit 1
        fi
    fi
    
    # Verificar dependencias
    if ! check_dependencies; then
        echo ""
        echo -e "${YELLOW}Para instalar dependencias:${NC}"
        echo "Ubuntu/Debian: sudo apt install build-essential libgtk-3-dev pkg-config"
        echo "Fedora/RHEL:   sudo dnf install gcc gtk3-devel pkgconf-devel"
        echo "Arch Linux:    sudo pacman -S base-devel gtk3 pkgconf"
        exit 1
    fi
    
    echo ""
    
    # Verificar archivos
    if ! check_files; then
        exit 1
    fi
    
    echo ""
    
    # Compilar
    if compile_interactive; then
        echo ""
        echo -e "${GREEN}🎉 ¡Test de compilación exitoso!${NC}"
        echo ""
        echo "Para ejecutar:"
        echo -e "${BLUE}./cuadros_magicos_interactivo${NC}"
        echo ""
        
        read -p "¿Desea ejecutar el programa ahora? (s/n): " ejecutar
        if [[ $ejecutar =~ ^[Ss]$ ]]; then
            echo ""
            echo -e "${BLUE}Iniciando programa...${NC}"
            ./cuadros_magicos_interactivo
        fi
    else
        echo ""
        echo -e "${RED}❌ Test de compilación falló${NC}"
        echo ""
        echo "Consejos para solucionar problemas:"
        echo "1. Verifique que todas las dependencias estén instaladas"
        echo "2. Asegúrese de que los archivos .glade estén en el directorio actual"
        echo "3. Verifique los permisos de los archivos"
        echo "4. Consulte los errores de compilación arriba"
        exit 1
    fi
}

# Hacer ejecutable este script
chmod +x "$0" 2>/dev/null

# Ejecutar
main "$@"

