# Generador de Cuadros Mágicos - Versión Interactiva

Este proyecto implementa un generador de cuadros mágicos con múltiples algoritmos y una interfaz gráfica **interactiva** que permite aprender paso a paso cómo se construyen los cuadros mágicos.

## 🎯 Características Principales

- **🎓 Modo Interactivo**: Aprende paso a paso cómo se llenan los cuadros mágicos
- **📊 Sumas en Tiempo Real**: Ve las sumas parciales de filas, columnas y diagonales actualizándose
- **🎲 Posición Aleatoria**: El algoritmo escoge automáticamente una casilla inicial aleatoria
- **⚡ Control Total**: Avanza paso a paso o completa automáticamente
- **📈 Visualización Dinámica**: El cuadro se actualiza en tiempo real con colores

## 🎮 Versiones Disponibles

### 1. **Versión Interactiva** (⭐ RECOMENDADA)
- Interfaz moderna e intuitiva
- Llenado paso a paso con retroalimentación visual
- Sumas parciales en tiempo real
- Controles para pausar/continuar/completar
- Archivo: `main_gtk_interactivo.c`

### 2. Versión GTK Simplificada
- Interfaz básica con generación completa
- Validación de resultados
- Archivo: `main_gtk_simple.c`

### 3. Versión de Consola
- Para uso en terminal
- Ideal para servidores sin interfaz gráfica
- Archivo: `main_console.c`

## 🔧 Algoritmos Implementados

### 1. Método Siamés (Clásico)
- Mueve en diagonal hacia arriba-derecha
- Si la casilla está ocupada, baja una fila
- El método más tradicional y conocido

### 2. Método De la Loubère
- Variante del método siamés
- Mueve en diagonal hacia abajo-izquierda
- Ajusta cuando encuentra obstáculos

### 3. Método En L
- Movimientos como el caballo del ajedrez
- Salta dos casillas arriba y una derecha
- Alternativa cuando hay bloqueos

### 4. Método Caracol
- Movimiento en espiral desde cualquier punto
- Llena en patrones circulares
- Enfoque único y visual

## 📋 Requisitos del Sistema

### Linux
- GCC (compilador C)
- GTK+3 development libraries
- pkg-config

#### Ubuntu/Debian
```bash
sudo apt update
sudo apt install build-essential libgtk-3-dev pkg-config
```

#### Fedora/RHEL
```bash
sudo dnf install gcc gtk3-devel pkgconf-devel
```

#### Arch Linux
```bash
sudo pacman -S base-devel gtk3 pkgconf
```

## 🚀 Compilación y Ejecución

### Linux (Método Recomendado)
```bash
# Dar permisos de ejecución
chmod +x compile_linux.sh

# Ejecutar script de compilación
./compile_linux.sh

# Seleccionar opción 2 (Versión GTK)
# Luego elegir opción 1 (Versión interactiva)
```

### Compilación Manual de la Versión Interactiva
```bash
# ⚠️ IMPORTANTE: El orden de las librerías importa en Linux
gcc -std=c99 $(pkg-config --cflags gtk+-3.0) main_gtk_interactivo.c $(pkg-config --libs gtk+-3.0) -o cuadros_magicos_interactivo
```

## 🎯 Cómo Usar la Versión Interactiva

### Paso 1: Crear el Cuadro
1. Selecciona un tamaño impar entre 3 y 21
2. Haz clic en "Crear Cuadro"
3. Se mostrará un grid vacío con la suma mágica objetivo

### Paso 2: Seleccionar Método
1. Escoge uno de los 4 algoritmos disponibles:
   - **Siamés**: Método clásico (diagonal arriba-derecha)
   - **De la Loubère**: Variante (diagonal abajo-izquierda)
   - **En L**: Movimiento de caballo de ajedrez
   - **Caracol**: Movimiento en espiral
2. Haz clic en "Iniciar Llenado"
3. El programa escoge una posición inicial aleatoria

### Paso 3: Llenado Interactivo
- **▶ Siguiente Paso**: Coloca el siguiente número y avanza una posición
- **⚡ Completar Automáticamente**: Llena todo el cuadro con animación
- **🔄 Reiniciar**: Vuelve a empezar desde el paso 1

### 📊 Visualización en Tiempo Real
- **Sumas Parciales**: Panel derecho muestra sumas de:
  - Todas las filas
  - Todas las columnas  
  - Diagonal principal
  - Diagonal secundaria
- **Progreso**: Contador "X/Y números"
- **Posición Actual**: Celda resaltada en amarillo
- **Celdas Llenas**: En azul claro
- **Validación**: ✓ cuando una suma alcanza el valor mágico

## ✨ Características Especiales

### 🎲 Posición Inicial Aleatoria
- Cada vez que inicias el llenado, se escoge una posición diferente
- Esto demuestra que los cuadros mágicos pueden empezar desde cualquier punto
- El resultado final siempre será válido

### 📈 Retroalimentación Visual
- **Colores dinámicos**: Las celdas cambian de color según su estado
- **Animaciones suaves**: Transiciones visuales durante el auto-completado
- **Indicadores claros**: Sabes siempre dónde estás en el proceso

### 🧠 Valor Educativo
- **Comprensión step-by-step**: Ve exactamente cómo funciona cada algoritmo
- **Comparación de métodos**: Prueba los 4 algoritmos y compara
- **Verificación matemática**: Las sumas se calculan en tiempo real

## 📁 Estructura del Proyecto

```
proyecto1aa/
├── main_gtk_interactivo.c        # 🌟 Versión interactiva (NUEVA)
├── cuadros_magicos_interactivo.glade  # 🌟 Interfaz interactiva (NUEVA)
├── main_gtk_simple.c             # Versión GTK simplificada
├── main_console.c                # Versión de consola
├── cuadros_magicos.c             # Funciones principales
├── cuadros_magicos.h             # Declaraciones
├── movimientos.h                 # Algoritmos de movimiento
├── Calculos.h                    # Funciones de cálculo
├── cuadros_magicos.glade         # Interfaz básica
├── compile_linux.sh              # 🔧 Script actualizado
└── README_INTERACTIVO.md         # Esta documentación
```

## 🐛 Solución de Problemas

### Error de Compilación: "undefined reference"
```bash
# El problema es el orden de las librerías. Usa:
gcc -std=c99 $(pkg-config --cflags gtk+-3.0) main_gtk_interactivo.c $(pkg-config --libs gtk+-3.0) -o cuadros_magicos_interactivo

# NO uses:
gcc -std=c99 $(pkg-config --cflags --libs gtk+-3.0) main_gtk_interactivo.c -o cuadros_magicos_interactivo
```

### Archivo .glade No Encontrado
```bash
# Verifica que el archivo existe:
ls cuadros_magicos_interactivo.glade

# Si no existe, asegúrate de estar en el directorio correcto:
pwd
# Debería mostrar: .../proyecto1aa
```

### Problemas de Dependencias GTK
```bash
# Verifica instalación:
pkg-config --modversion gtk+-3.0

# Si no está instalado:
# Ubuntu/Debian: sudo apt install libgtk-3-dev
# Fedora: sudo dnf install gtk3-devel
# Arch: sudo pacman -S gtk3
```

### El Programa No Inicia
```bash
# Verifica permisos:
chmod +x cuadros_magicos_interactivo

# Ejecuta desde terminal para ver errores:
./cuadros_magicos_interactivo
```

## 🎯 Casos de Uso

### Para Estudiantes
- Entender cómo funcionan los algoritmos de cuadros mágicos
- Ver la matemática en acción (sumas constantes)
- Experimentar con diferentes tamaños y métodos

### Para Profesores
- Demostrar conceptos de:
  - Algoritmos paso a paso
  - Estructuras de datos (matrices)
  - Programación en C
  - Interfaces gráficas

### Para Desarrolladores
- Ejemplo de aplicación GTK en C
- Manejo de eventos y callbacks
- Actualización dinámica de interfaces
- Separación entre lógica y presentación

## 🔮 Próximas Mejoras

- [ ] Modo de comparación lado a lado de algoritmos
- [ ] Exportar cuadros mágicos a imagen
- [ ] Historial de movimientos con deshacer
- [ ] Modo de desafío (adivinar el siguiente número)
- [ ] Estadísticas de rendimiento de algoritmos

## 📞 Soporte

Si encuentras problemas:
1. Verifica que tienes todas las dependencias instaladas
2. Compila usando el script `compile_linux.sh`
3. Revisa los errores en terminal
4. Consulta la sección de solución de problemas

## 📄 Licencia

Proyecto desarrollado para el curso de Análisis de Algoritmos.
Uso académico y educativo libre.

---

**¡Disfruta explorando el fascinante mundo de los cuadros mágicos! 🎩✨**
