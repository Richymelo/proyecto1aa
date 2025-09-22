# Generador de Cuadros Mágicos - Linux

Este proyecto implementa varios algoritmos para generar cuadros mágicos, incluyendo el algoritmo de Robert Kurosaka, con una interfaz gráfica desarrollada en GTK3. **Diseñado exclusivamente para sistemas Linux**.

## 🧮 Algoritmos Implementados

1. **🌟 Algoritmo de Kurosaka** - Movimiento noreste con break-move (algoritmo principal)
2. **Método Siamés** - Movimiento diagonal clásico
3. **Método De la Loubère** - Variante del método siamés
4. **Método L** - Movimiento en forma de caballo de ajedrez
5. **Método Alterno** - Variante diagonal alternativa

## ✨ Características

- 🖥️ **Doble interfaz**: Consola y gráfica GTK3
- 📊 **Visualización interactiva** del cuadro mágico
- ✅ **Validación matemática completa** - Filas, columnas y diagonales
- 🎯 **Cuadros de 3x3 hasta 21x21** (solo tamaños impares)
- 🚀 **Optimizado para Linux** - Todas las distribuciones principales
- 🔇 **Sin salida en consola** en la versión GTK

## 🚀 Instalación Rápida

### Ubuntu/Debian
```bash
sudo apt update
sudo apt install build-essential pkg-config libgtk-3-dev
make run-gtk
```

### Fedora/RHEL
```bash
sudo dnf install gcc pkgconf-devel gtk3-devel
make run-gtk
```

### Arch Linux
```bash
sudo pacman -S base-devel pkgconf gtk3
make run-gtk
```

### Instalación Automática (Cualquier distribución)
```bash
chmod +x install_linux.sh
./install_linux.sh
```

## 🔨 Compilación

### Método 1: Makefile (Recomendado)
```bash
make help                    # Ver todas las opciones
make install-deps-auto       # Instalar dependencias automáticamente
make all-versions           # Compilar ambas versiones
make run                    # Ejecutar versión consola
make run-gtk                # Ejecutar versión gráfica
```

### Método 2: Script Interactivo
```bash
chmod +x compile_linux.sh
./compile_linux.sh
```

### Método 3: Manual
```bash
# Versión consola (sin dependencias GTK)
gcc -std=c99 compile_simple.c -o cuadros_magicos_console

# Versión GTK (requiere GTK+3)
gcc -std=c99 $(pkg-config --cflags --libs gtk+-3.0) main_gtk_simple.c -o cuadros_magicos_gtk
```

## 🎮 Uso

### 🖥️ Versión Consola
```bash
./cuadros_magicos_console
```
1. Seleccionar algoritmo (1-5)
2. Ingresar tamaño del cuadro (impar, 3-21)
3. Ver resultado con validación detallada

### 🖼️ Versión GTK
```bash
./cuadros_magicos_gtk
```
1. **Panel superior**: Configurar tamaño y algoritmo
2. **Botón "Generar"**: Crear cuadro mágico
3. **Área central**: Visualizar el cuadro en grid
4. **Botón "Validar"**: Verificar correctitud
5. **Panel inferior**: Ver estado y validación

## 🧮 Algoritmo de Kurosaka

Basado en la especificación de Robert T. Kurosaka (incluida en `Kurosaka.pdf`):

1. **Inicio**: Centro de la primera fila
2. **Movimiento**: Noreste (arriba + derecha)
3. **Break-move**: Hacia abajo cuando encuentra celda ocupada
4. **Edge-wrapping**: Manejo de bordes con aritmética modular

## 📁 Estructura del Proyecto

```
proyecto1aa/
├── compile_simple.c         # Versión consola (todo-en-uno)
├── main_gtk_simple.c        # Versión GTK (todo-en-uno)
├── cuadros_magicos.glade    # Interfaz Glade
├── Makefile                 # Compilación para Linux
├── compile_linux.sh        # Script interactivo
├── install_linux.sh        # Instalador automático
├── README.md                # Esta documentación
├── Kurosaka.pdf            # Especificación del algoritmo
├── movimientos.h            # Algoritmos originales (referencia)
├── Calculos.h               # Funciones auxiliares (referencia)
└── Ordenamientos.c          # Algoritmos de ordenamiento (referencia)
```

## 🔧 Solución de Problemas

### Error: "gcc: command not found"
```bash
# Ubuntu/Debian
sudo apt install build-essential

# Fedora/RHEL
sudo dnf install gcc

# Arch Linux
sudo pacman -S base-devel
```

### Error: "Package gtk+-3.0 was not found"
```bash
# Ubuntu/Debian
sudo apt install libgtk-3-dev

# Fedora/RHEL
sudo dnf install gtk3-devel

# Arch Linux
sudo pacman -S gtk3
```

### La interfaz GTK no aparece
```bash
# Verificar entorno gráfico
echo $DISPLAY

# Si está en SSH, usar X11 forwarding
ssh -X usuario@servidor

# Alternativamente, usar versión consola
./cuadros_magicos_console
```

## 🐧 Distribuciones Compatibles

| Distribución | Estado | Comando de instalación |
|-------------|--------|----------------------|
| Ubuntu 18.04+ | ✅ Completo | `sudo apt install build-essential libgtk-3-dev` |
| Debian 10+ | ✅ Completo | `sudo apt install build-essential libgtk-3-dev` |
| Fedora 30+ | ✅ Completo | `sudo dnf install gcc gtk3-devel` |
| RHEL/CentOS 8+ | ✅ Completo | `sudo dnf install gcc gtk3-devel` |
| Arch Linux | ✅ Completo | `sudo pacman -S base-devel gtk3` |
| openSUSE | ✅ Completo | `sudo zypper install gcc gtk3-devel` |
| Alpine Linux | ✅ Consola | `apk add gcc musl-dev` |

## 📊 Características Técnicas

- **Lenguaje**: C (estándar C99)
- **Interfaz gráfica**: GTK3 con Glade
- **Compilador**: GCC con optimización -O2
- **Dependencias**: mínimas (solo libc para consola, +GTK3 para gráfica)
- **Memoria**: < 1MB en ejecución
- **Arquitecturas**: x86_64, ARM64, i386

## 🎯 Comandos Útiles

```bash
# Ver ayuda del Makefile
make help

# Verificar dependencias GTK
make check-gtk

# Limpiar archivos compilados
make clean

# Instalar dependencias automáticamente
make install-deps-auto

# Compilar solo versión consola
make cuadros_magicos_console

# Compilar solo versión GTK
make cuadros_magicos_gtk
```

---

**Desarrollado como parte del curso de Análisis de Algoritmos**  
**Implementación completa del algoritmo de Robert T. Kurosaka para cuadros mágicos**