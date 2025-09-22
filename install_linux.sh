#!/bin/bash
# Script de instalación automática para Linux
# Generador de Cuadros Mágicos con algoritmo de Kurosaka

set -e  # Salir en caso de error

# Colores
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
PURPLE='\033[0;35m'
NC='\033[0m' # No Color

# Banner
echo -e "${PURPLE}"
echo "╔══════════════════════════════════════════════════════════════╗"
echo "║               GENERADOR DE CUADROS MÁGICOS                  ║"
echo "║                 Instalador para Linux                       ║"
echo "║                                                              ║"
echo "║  Incluye el algoritmo de Robert T. Kurosaka                 ║"
echo "╚══════════════════════════════════════════════════════════════╝"
echo -e "${NC}"

# Detectar distribución
detect_distro() {
    if [ -f /etc/os-release ]; then
        . /etc/os-release
        DISTRO=$ID
        VERSION=$VERSION_ID
    elif command -v lsb_release >/dev/null 2>&1; then
        DISTRO=$(lsb_release -si | tr '[:upper:]' '[:lower:]')
        VERSION=$(lsb_release -sr)
    else
        DISTRO="unknown"
        VERSION="unknown"
    fi
}

# Función para instalar dependencias
install_dependencies() {
    echo -e "${BLUE}📦 Instalando dependencias para $DISTRO...${NC}"
    
    case $DISTRO in
        ubuntu|debian)
            echo "🔄 Actualizando paquetes..."
            sudo apt update
            echo "📥 Instalando build-essential, pkg-config y GTK+3..."
            sudo apt install -y build-essential pkg-config libgtk-3-dev
            ;;
        fedora|rhel|centos)
            echo "📥 Instalando gcc, pkgconf-devel y gtk3-devel..."
            if command -v dnf >/dev/null 2>&1; then
                sudo dnf install -y gcc pkgconf-devel gtk3-devel
            else
                sudo yum install -y gcc pkgconf-devel gtk3-devel
            fi
            ;;
        arch|manjaro)
            echo "📥 Instalando base-devel, pkgconf y gtk3..."
            sudo pacman -S --needed --noconfirm base-devel pkgconf gtk3
            ;;
        opensuse*|sled|sles)
            echo "📥 Instalando gcc, pkg-config y gtk3-devel..."
            sudo zypper install -y gcc pkg-config gtk3-devel
            ;;
        alpine)
            echo "📥 Instalando gcc, musl-dev, pkg-config y gtk+3.0-dev..."
            sudo apk add gcc musl-dev pkgconfig gtk+3.0-dev
            ;;
        *)
            echo -e "${YELLOW}⚠️  Distribución no reconocida: $DISTRO${NC}"
            echo "Instale manualmente:"
            echo "- gcc (compilador C)"
            echo "- pkg-config"
            echo "- gtk+3 development headers"
            return 1
            ;;
    esac
}

# Verificar instalación
verify_installation() {
    echo -e "${BLUE}🔍 Verificando instalación...${NC}"
    
    # Verificar gcc
    if command -v gcc >/dev/null 2>&1; then
        echo -e "${GREEN}✓ gcc encontrado: $(gcc --version | head -n1)${NC}"
    else
        echo -e "${RED}✗ gcc no encontrado${NC}"
        return 1
    fi
    
    # Verificar pkg-config
    if command -v pkg-config >/dev/null 2>&1; then
        echo -e "${GREEN}✓ pkg-config encontrado${NC}"
    else
        echo -e "${RED}✗ pkg-config no encontrado${NC}"
        return 1
    fi
    
    # Verificar GTK
    if pkg-config --exists gtk+-3.0; then
        GTK_VERSION=$(pkg-config --modversion gtk+-3.0)
        echo -e "${GREEN}✓ GTK+3 encontrado: versión $GTK_VERSION${NC}"
    else
        echo -e "${YELLOW}⚠️  GTK+3 no encontrado (solo funcionará versión consola)${NC}"
    fi
}

# Compilar proyecto
compile_project() {
    echo -e "${BLUE}🔨 Compilando proyecto...${NC}"
    
    # Compilar versión consola
    echo "📝 Compilando versión de consola..."
    if gcc -Wall -Wextra -std=c99 compile_simple.c -o cuadros_magicos_console; then
        echo -e "${GREEN}✓ Versión consola compilada${NC}"
    else
        echo -e "${RED}✗ Error compilando versión consola${NC}"
        return 1
    fi
    
    # Compilar versión GTK si está disponible
    if pkg-config --exists gtk+-3.0; then
        echo "🖼️  Compilando versión GTK..."
        if gcc -Wall -Wextra -std=c99 \
            $(pkg-config --cflags gtk+-3.0) \
            main_gtk_simple.c \
            -o cuadros_magicos_gtk \
            $(pkg-config --libs gtk+-3.0); then
            echo -e "${GREEN}✓ Versión GTK compilada${NC}"
            GTK_COMPILED=true
        else
            echo -e "${YELLOW}⚠️  Error compilando versión GTK${NC}"
            GTK_COMPILED=false
        fi
    else
        echo -e "${YELLOW}⚠️  GTK no disponible, solo versión consola${NC}"
        GTK_COMPILED=false
    fi
}

# Crear scripts de ejecución
create_launch_scripts() {
    echo -e "${BLUE}📝 Creando scripts de ejecución...${NC}"
    
    # Script para versión consola
    cat > run_console.sh << 'EOF'
#!/bin/bash
echo "=== Generador de Cuadros Mágicos - Consola ==="
echo "Algoritmo de Robert T. Kurosaka implementado"
echo ""
./cuadros_magicos_console
EOF
    chmod +x run_console.sh
    
    # Script para versión GTK (si existe)
    if [ "$GTK_COMPILED" = true ]; then
        cat > run_gtk.sh << 'EOF'
#!/bin/bash
echo "=== Generador de Cuadros Mágicos - GTK ==="
echo "Iniciando interfaz gráfica..."
echo ""
./cuadros_magicos_gtk
EOF
        chmod +x run_gtk.sh
    fi
    
    echo -e "${GREEN}✓ Scripts creados${NC}"
}

# Mostrar instrucciones finales
show_final_instructions() {
    echo ""
    echo -e "${GREEN}🎉 ¡Instalación completada exitosamente!${NC}"
    echo ""
    echo -e "${BLUE}📋 Archivos generados:${NC}"
    echo "• cuadros_magicos_console  - Versión de consola"
    if [ "$GTK_COMPILED" = true ]; then
        echo "• cuadros_magicos_gtk      - Versión gráfica GTK"
    fi
    echo "• run_console.sh           - Script para ejecutar consola"
    if [ "$GTK_COMPILED" = true ]; then
        echo "• run_gtk.sh               - Script para ejecutar GTK"
    fi
    echo ""
    echo -e "${BLUE}🚀 Para ejecutar:${NC}"
    echo "• Versión consola:  ./run_console.sh  o  ./cuadros_magicos_console"
    if [ "$GTK_COMPILED" = true ]; then
        echo "• Versión gráfica:  ./run_gtk.sh      o  ./cuadros_magicos_gtk"
    fi
    echo ""
    echo -e "${BLUE}🧮 Algoritmos disponibles:${NC}"
    echo "1. Kurosaka (Noreste) - Algoritmo principal del proyecto"
    echo "2. Siamés (Diagonal)"
    echo "3. De la Loubère"
    echo "4. Método L"
    echo "5. Alterno"
    echo ""
    echo -e "${BLUE}📖 Documentación adicional:${NC}"
    echo "• README_LINUX.md - Guía completa para Linux"
    echo "• Kurosaka.pdf    - Especificación del algoritmo"
    echo ""
    echo -e "${GREEN}¡Disfrute generando cuadros mágicos!${NC}"
}

# Función principal
main() {
    echo -e "${BLUE}🔍 Detectando sistema...${NC}"
    detect_distro
    echo "Sistema detectado: $DISTRO $VERSION"
    echo ""
    
    # Verificar si ya tiene las dependencias
    if command -v gcc >/dev/null 2>&1 && command -v pkg-config >/dev/null 2>&1; then
        echo -e "${GREEN}✓ Dependencias básicas ya instaladas${NC}"
        verify_installation
    else
        echo -e "${YELLOW}📦 Instalando dependencias...${NC}"
        if ! install_dependencies; then
            echo -e "${RED}❌ Error instalando dependencias${NC}"
            exit 1
        fi
        verify_installation
    fi
    
    echo ""
    if ! compile_project; then
        echo -e "${RED}❌ Error en la compilación${NC}"
        exit 1
    fi
    
    echo ""
    create_launch_scripts
    
    echo ""
    show_final_instructions
}

# Verificar que estamos en el directorio correcto
if [ ! -f "compile_simple.c" ] || [ ! -f "main_gtk_simple.c" ]; then
    echo -e "${RED}❌ Error: No se encuentran los archivos fuente${NC}"
    echo "Asegúrese de ejecutar este script desde el directorio del proyecto"
    exit 1
fi

# Ejecutar instalación
main
