#!/bin/bash
# Script de diagnóstico del entorno GTK

echo "============================================"
echo " DIAGNÓSTICO DEL ENTORNO GTK"
echo "============================================"

# Colores
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

echo -e "${BLUE}1. Información del sistema:${NC}"
echo "Sistema operativo: $(uname -a)"
echo "Distribución: $(lsb_release -d 2>/dev/null || cat /etc/os-release | grep PRETTY_NAME)"
echo ""

echo -e "${BLUE}2. Variables de entorno relacionadas con X11/Wayland:${NC}"
echo "DISPLAY: $DISPLAY"
echo "WAYLAND_DISPLAY: $WAYLAND_DISPLAY"
echo "XDG_SESSION_TYPE: $XDG_SESSION_TYPE"
echo "XDG_CURRENT_DESKTOP: $XDG_CURRENT_DESKTOP"
echo ""

echo -e "${BLUE}3. Verificando servidor gráfico:${NC}"
if [ -n "$DISPLAY" ]; then
    echo -e "${GREEN}✓ Variable DISPLAY configurada: $DISPLAY${NC}"
    if command -v xdpyinfo &> /dev/null; then
        if xdpyinfo &> /dev/null; then
            echo -e "${GREEN}✓ Servidor X funcionando${NC}"
        else
            echo -e "${RED}❌ Servidor X no responde${NC}"
        fi
    else
        echo -e "${YELLOW}⚠️ xdpyinfo no disponible para verificar X${NC}"
    fi
elif [ -n "$WAYLAND_DISPLAY" ]; then
    echo -e "${GREEN}✓ Variable WAYLAND_DISPLAY configurada: $WAYLAND_DISPLAY${NC}"
else
    echo -e "${RED}❌ No hay variables de display configuradas${NC}"
    echo "El programa GTK necesita un servidor gráfico (X11 o Wayland)"
fi
echo ""

echo -e "${BLUE}4. Verificando dependencias del compilador:${NC}"
if command -v gcc &> /dev/null; then
    echo -e "${GREEN}✓ GCC encontrado: $(gcc --version | head -n1)${NC}"
else
    echo -e "${RED}❌ GCC no encontrado${NC}"
fi

if command -v pkg-config &> /dev/null; then
    echo -e "${GREEN}✓ pkg-config encontrado${NC}"
else
    echo -e "${RED}❌ pkg-config no encontrado${NC}"
fi
echo ""

echo -e "${BLUE}5. Verificando librerías GTK:${NC}"
if pkg-config --exists gtk+-3.0; then
    echo -e "${GREEN}✓ GTK+3 encontrado: $(pkg-config --modversion gtk+-3.0)${NC}"
    echo "   Ruta de includes: $(pkg-config --cflags gtk+-3.0)"
    echo "   Librerías: $(pkg-config --libs gtk+-3.0)"
else
    echo -e "${RED}❌ GTK+3 no encontrado${NC}"
fi
echo ""

echo -e "${BLUE}6. Verificando archivos del proyecto:${NC}"
if [ -f "cuadros_magicos_interactivo.glade" ]; then
    echo -e "${GREEN}✓ cuadros_magicos_interactivo.glade encontrado${NC}"
    echo "   Tamaño: $(stat -c%s cuadros_magicos_interactivo.glade) bytes"
    echo "   Modificado: $(stat -c%y cuadros_magicos_interactivo.glade)"
else
    echo -e "${RED}❌ cuadros_magicos_interactivo.glade no encontrado${NC}"
fi

if [ -f "main_gtk_interactivo.c" ]; then
    echo -e "${GREEN}✓ main_gtk_interactivo.c encontrado${NC}"
else
    echo -e "${RED}❌ main_gtk_interactivo.c no encontrado${NC}"
fi

if [ -f "main_gtk_interactivo_fixed.c" ]; then
    echo -e "${GREEN}✓ main_gtk_interactivo_fixed.c encontrado${NC}"
else
    echo -e "${YELLOW}⚠️ main_gtk_interactivo_fixed.c no encontrado${NC}"
fi
echo ""

echo -e "${BLUE}7. Verificando ejecutables existentes:${NC}"
for exe in cuadros_magicos_interactivo cuadros_magicos_interactivo_fixed; do
    if [ -f "./$exe" ]; then
        if [ -x "./$exe" ]; then
            echo -e "${GREEN}✓ $exe encontrado y ejecutable${NC}"
            echo "   Tamaño: $(stat -c%s ./$exe) bytes"
            echo "   Modificado: $(stat -c%y ./$exe)"
        else
            echo -e "${YELLOW}⚠️ $exe encontrado pero no ejecutable${NC}"
        fi
    else
        echo -e "${RED}❌ $exe no encontrado${NC}"
    fi
done
echo ""

echo -e "${BLUE}8. Test de compilación simple:${NC}"
echo "Creando programa de prueba mínimo..."

cat > test_gtk_minimal.c << 'EOF'
#include <gtk/gtk.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    printf("Iniciando test GTK mínimo...\n");
    
    if (!gtk_init_check(&argc, &argv)) {
        printf("ERROR: gtk_init_check falló\n");
        return 1;
    }
    
    printf("GTK inicializado correctamente\n");
    
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    if (!window) {
        printf("ERROR: No se pudo crear ventana\n");
        return 1;
    }
    
    printf("Ventana creada correctamente\n");
    gtk_widget_destroy(window);
    printf("Test completado exitosamente\n");
    
    return 0;
}
EOF

echo "Compilando test mínimo..."
if gcc $(pkg-config --cflags gtk+-3.0) test_gtk_minimal.c $(pkg-config --libs gtk+-3.0) -o test_gtk_minimal 2>/dev/null; then
    echo -e "${GREEN}✓ Compilación del test exitosa${NC}"
    
    echo "Ejecutando test..."
    if ./test_gtk_minimal 2>/dev/null; then
        echo -e "${GREEN}✓ Test GTK mínimo ejecutado exitosamente${NC}"
    else
        echo -e "${RED}❌ Test GTK mínimo falló en ejecución${NC}"
        echo "Esto indica un problema con el entorno GTK"
    fi
    
    rm -f test_gtk_minimal test_gtk_minimal.c
else
    echo -e "${RED}❌ Error en compilación del test${NC}"
fi
echo ""

echo -e "${BLUE}9. Recomendaciones:${NC}"

if [ -z "$DISPLAY" ] && [ -z "$WAYLAND_DISPLAY" ]; then
    echo -e "${YELLOW}⚠️ No hay servidor gráfico disponible${NC}"
    echo "Ejecute uno de estos comandos según su entorno:"
    echo "  Para X11: export DISPLAY=:0"
    echo "  Para SSH con X forwarding: ssh -X usuario@servidor"
    echo "  Para ejecutar en consola pura: use la versión de consola"
fi

if ! pkg-config --exists gtk+-3.0; then
    echo -e "${YELLOW}⚠️ GTK+3 no está instalado${NC}"
    echo "Instale con:"
    echo "  Ubuntu/Debian: sudo apt install libgtk-3-dev"
    echo "  Fedora/RHEL: sudo dnf install gtk3-devel"
    echo "  Arch Linux: sudo pacman -S gtk3"
fi

echo ""
echo "============================================"
echo " FIN DEL DIAGNÓSTICO"
echo "============================================"







