# Makefile para el Generador de Cuadros Mágicos con GTK
# Optimizado para sistemas Linux

# Configuración del compilador
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g -O2

# Verificar que estamos en Linux
UNAME_S := $(shell uname -s)
ifneq ($(UNAME_S),Linux)
    $(error Este proyecto está optimizado solo para Linux. Sistema detectado: $(UNAME_S))
endif

# Configuración para Linux
GTKFLAGS = $(shell pkg-config --cflags --libs gtk+-3.0 2>/dev/null || echo "")
CONSOLE_FLAGS = 
EXT = 

# Directorios
SRCDIR = .
OBJDIR = obj
BINDIR = bin

# Archivos fuente y objetivos
GTK_SOURCE = main_gtk_simple.c
GTK_TARGET = cuadros_magicos_gtk

CONSOLE_SOURCE = compile_simple.c
CONSOLE_TARGET = cuadros_magicos_console

# Regla principal
all: $(CONSOLE_TARGET)

# Compilar ambas versiones
all-versions: $(CONSOLE_TARGET) $(GTK_TARGET)

# Crear directorios si no existen
$(OBJDIR):
	mkdir -p $(OBJDIR)

$(BINDIR):
	mkdir -p $(BINDIR)

# Compilar versión de consola
$(CONSOLE_TARGET): $(CONSOLE_SOURCE)
	$(CC) $(CFLAGS) $(CONSOLE_SOURCE) -o $(CONSOLE_TARGET)

# Compilar versión GTK
$(GTK_TARGET): $(GTK_SOURCE)
	@if [ -z "$(GTKFLAGS)" ]; then \
		echo "ERROR: GTK+3 no está disponible. Instale libgtk-3-dev (Ubuntu) o gtk3-devel (Fedora)"; \
		exit 1; \
	fi
	$(CC) $(CFLAGS) $(shell pkg-config --cflags gtk+-3.0) $(GTK_SOURCE) -o $(GTK_TARGET) $(shell pkg-config --libs gtk+-3.0)

# Verificar dependencias GTK
check-gtk:
	@echo "Verificando GTK+3..."
	@if pkg-config --exists gtk+-3.0; then \
		echo "✓ GTK+3 encontrado, versión: $$(pkg-config --modversion gtk+-3.0)"; \
	else \
		echo "✗ GTK+3 no encontrado"; \
		echo "Instale con:"; \
		echo "  Ubuntu/Debian: sudo apt install libgtk-3-dev"; \
		echo "  Fedora/RHEL:   sudo dnf install gtk3-devel"; \
		echo "  Arch Linux:    sudo pacman -S gtk3"; \
	fi

# Instalar dependencias en Ubuntu/Debian
install-deps:
	sudo apt-get update
	sudo apt-get install -y build-essential pkg-config libgtk-3-dev glade

# Instalar dependencias en Fedora/CentOS
install-deps-fedora:
	sudo dnf install -y gcc pkg-config gtk3-devel glade

# Instalar dependencias en Arch Linux
install-deps-arch:
	sudo pacman -S base-devel pkg-config gtk3 glade

# Ejecutar versión de consola
run: $(CONSOLE_TARGET)
	./$(CONSOLE_TARGET)

# Ejecutar versión GTK
run-gtk: $(GTK_TARGET)
	./$(GTK_TARGET)

# Limpiar archivos compilados
clean:
	rm -f $(GTK_TARGET) $(CONSOLE_TARGET) *.o

# Limpiar todo
distclean: clean
	rm -rf $(OBJDIR) $(BINDIR)

# Detectar sistema e instalar dependencias
install-deps-auto:
	@echo "Detectando sistema operativo..."
	@if [ "$(UNAME_S)" = "Linux" ]; then \
		if command -v apt-get >/dev/null 2>&1; then \
			echo "Sistema Ubuntu/Debian detectado"; \
			sudo apt-get update && sudo apt-get install -y build-essential pkg-config libgtk-3-dev; \
		elif command -v dnf >/dev/null 2>&1; then \
			echo "Sistema Fedora/RHEL detectado"; \
			sudo dnf install -y gcc pkgconf-devel gtk3-devel; \
		elif command -v pacman >/dev/null 2>&1; then \
			echo "Sistema Arch Linux detectado"; \
			sudo pacman -S --needed base-devel pkgconf gtk3; \
		else \
			echo "Distribución no reconocida. Instale manualmente:"; \
			echo "- gcc, pkg-config, gtk3-dev"; \
		fi \
	else \
		echo "Distribución no reconocida. Instale manualmente:"; \
		echo "- gcc, pkg-config, gtk3-dev"; \
	fi

# Ayuda
help:
	@echo "Makefile para Generador de Cuadros Mágicos"
	@echo ""
	@echo "Objetivos disponibles:"
	@echo "  all                 - Compilar versión de consola"
	@echo "  all-versions        - Compilar ambas versiones (consola y GTK)"
	@echo "  run                 - Compilar y ejecutar versión de consola"
	@echo "  run-gtk             - Compilar y ejecutar versión GTK"
	@echo "  clean               - Limpiar archivos compilados"
	@echo "  distclean           - Limpiar todo"
	@echo "  install-deps        - Instalar dependencias (Ubuntu/Debian)"
	@echo "  install-deps-fedora - Instalar dependencias (Fedora/RHEL)"
	@echo "  install-deps-arch   - Instalar dependencias (Arch Linux)"
	@echo "  install-deps-auto   - Detectar sistema e instalar automáticamente"
	@echo "  check-gtk           - Verificar instalación de GTK"
	@echo "  help                - Mostrar esta ayuda"
	@echo ""
	@echo "Sistema: Linux (optimizado)"

# Declarar objetivos que no son archivos
.PHONY: all clean distclean run install-deps install-deps-fedora install-deps-arch check-gtk help
