# Generador de Cuadros Mágicos

Proyecto de Análisis de Algoritmos - Generador de cuadros mágicos con múltiples algoritmos de llenado.

## Descripción

Un cuadro mágico es una matriz cuadrada donde la suma de números en cualquier fila, columna o diagonal es idéntica. Este proyecto implementa varios algoritmos para generar cuadros mágicos de orden impar.

## Versiones Disponibles

### 1. Versión Automática (`cuadros_magicos_gtk`)
- Interfaz gráfica GTK
- Generación automática del cuadro completo
- 5 algoritmos diferentes
- Validación en tiempo real

### 2. Versión Interactiva (`cuadros_magicos_completo`)
- Interfaz gráfica avanzada
- Modo paso a paso
- Sumas parciales en tiempo real
- 4 algoritmos de llenado
- Visualización del proceso

### 3. Versión de Consola (`cuadros_magicos_consola`)
- Interfaz de línea de comandos
- Estadísticas detalladas
- Múltiples algoritmos

## Algoritmos Implementados

1. **Método Siamés**: Diagonal arriba-derecha, ajuste hacia abajo
2. **Método en L**: Movimiento tipo caballo de ajedrez
3. **Diagonal Principal**: Llenado sistemático por diagonal
4. **Diagonal Secundaria**: Variante de diagonal
5. **Método De la Loubère**: Diagonal abajo-izquierda (solo versión automática)

## Compilación

### Requisitos
- GCC
- GTK+3 (para versiones gráficas)
- pkg-config

### Instalar dependencias
```bash
# Ubuntu/Debian
sudo apt install build-essential libgtk-3-dev pkg-config
```

### Compilar
```bash
chmod +x compilar.sh
./compilar.sh
```

## Uso

```bash
# Versión automática
./cuadros_magicos_gtk

# Versión interactiva
./cuadros_magicos_completo

# Versión de consola
./cuadros_magicos_consola
```

## Características

- Tamaños de 3x3 hasta 21x21 (números impares)
- Validación automática de sumas
- Interfaz gráfica moderna
- Múltiples algoritmos de construcción
- Visualización en tiempo real

## Estructura del Proyecto

```
proyecto1aa/
├── main_gtk_simple.c                      # Versión automática
├── cuadros_magicos_interactivo_completo.c # Versión interactiva  
├── main_console.c                          # Versión de consola
├── cuadros_magicos.c                       # Algoritmos base
├── movimientos.c                           # Funciones de movimiento
├── compilar.sh                             # Script de compilación
└── README_PROYECTO.md                      # Esta documentación
```

## Notas de Implementación

- Los cuadros mágicos generados son validados automáticamente
- La suma mágica para un cuadro n×n es: n(n²+1)/2
- Solo se soportan tamaños impares (limitación de los algoritmos implementados)
- Las interfaces gráficas usan GTK+3 para compatibilidad con sistemas Linux modernos
