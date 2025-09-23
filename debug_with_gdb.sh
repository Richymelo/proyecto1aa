#!/bin/bash
# Script para debuggear el segmentation fault con gdb

echo "=============================================="
echo " DEBUG CON GDB"
echo "=============================================="

# Compilar con símbolos de debug
echo "Compilando con símbolos de debug..."
gcc -g -std=c99 $(pkg-config --cflags gtk+-3.0) test_simple_glade.c $(pkg-config --libs gtk+-3.0) -o test_simple_glade_debug

if [ $? -ne 0 ]; then
    echo "ERROR: Falló la compilación con debug"
    exit 1
fi

echo "✓ Compilación exitosa"

# Verificar si gdb está disponible
if ! command -v gdb &> /dev/null; then
    echo "ERROR: gdb no está instalado"
    echo "Instala con: sudo apt install gdb"
    exit 1
fi

echo "✓ gdb encontrado"

# Crear script de gdb
cat > gdb_commands.txt << 'EOF'
set pagination off
set logging file segfault_log.txt
set logging on
run
where
info registers
info stack
quit
EOF

echo "Ejecutando con gdb..."
echo "El programa puede tardar un momento y terminar con segfault (esperado)"

# Ejecutar con gdb
gdb -batch -x gdb_commands.txt ./test_simple_glade_debug

echo ""
echo "=============================================="
echo " RESULTADOS DEL DEBUG"
echo "=============================================="

if [ -f "segfault_log.txt" ]; then
    echo "Información del segfault:"
    cat segfault_log.txt
    echo ""
else
    echo "No se generó log de segfault (posiblemente el programa funcionó)"
fi

# También intentar con valgrind si está disponible
if command -v valgrind &> /dev/null; then
    echo "=============================================="
    echo " ANÁLISIS CON VALGRIND"
    echo "=============================================="
    
    echo "Ejecutando con valgrind..."
    timeout 30 valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all --track-origins=yes ./test_simple_glade_debug 2>&1 | head -50
    
    echo ""
fi

# Limpiar archivos temporales
rm -f gdb_commands.txt segfault_log.txt

echo "=============================================="
echo " FIN DEL DEBUG"
echo "=============================================="

