# ADA — Práctica 1: Estructuras Unidimensionales

Universidad EAFIT — Estructuras de Datos y Algoritmos I

> **Estado: esqueleto.** Las operaciones marcadas con `TODO` en `src/` están sin
> implementar. Ver la sección "Qué falta" al final.

## Integrantes
- Nash Díaz Quessep

## Descripción

Dos simuladores construidos sobre estructuras unidimensionales implementadas
desde cero, sin `std::stack`, `std::queue`, `std::deque` ni `std::list`.

- **Problema 1 — Undo/Redo de un editor de código.** Dos pilas (LIFO): una de
  deshacer y otra de rehacer. Una edición nueva invalida el historial de rehacer.
- **Problema 2 — Búfer y limitador de tasa de un firewall.** Una cola circular de
  capacidad fija `C` como búfer de recepción, y una cola de marcas de tiempo que
  implementa un limitador por ventana deslizante de `T` ms y `L` paquetes.

## Estructura del repositorio

```
ADA_P1_Apellido1_Apellido2_Apellido3/
├── README.md
├── Makefile
├── data/                 instancias sintéticas generadas (no versionadas)
├── src/
│   ├── tad.hpp           contratos IStack e IQueue + tipos EditOp y Packet
│   ├── stack_array.*     pila sobre arreglo dinámico
│   ├── stack_list.*      pila sobre lista enlazada simple
│   ├── undoredo.*        lógica del Problema 1
│   ├── queue_circular.*  cola circular sobre arreglo de capacidad fija
│   ├── queue_list.*      cola sobre lista enlazada
│   ├── ratelimiter.*     lógica del Problema 2
│   ├── generator.*       generación determinista de instancias
│   └── main.cpp          orquestador y arnés de medición
├── run_tests.sh          ejecuta los 14 casos obligatorios
├── tests/
│   ├── p1/               7 casos del Problema 1 (Sección 11)
│   └── p2/               7 casos del Problema 2 (Sección 11)
├── results/              salidas de experimentación (tiempos, logs, reportes)
└── report/
    ├── ESQUELETO_INFORME.md
    └── Informe.pdf
```

`tad.hpp` es un archivo añadido sobre el árbol sugerido en el enunciado: aísla la
declaración del TAD de sus dos implementaciones, que es exactamente lo que exige
la Sección 6.

## Requisitos

- Compilador con soporte de C++17 (`g++` ≥ 7, `clang++` ≥ 6)
- Nada más: sin bibliotecas externas

## Compilación

```bash
g++ -std=c++17 -O2 -o ada_p1 src/*.cpp
```

o bien `make`.

## Ejecución

```bash
# Generar instancias (semilla explícita, reproducible)
./ada_p1 gen1 100000 20260828 data/eventos_100k.txt
./ada_p1 gen2 100000 20260828 data/paquetes_100k.txt

# Problema 1, con cada una de las dos representaciones de pila
./ada_p1 p1 data/eventos_100k.txt array results/p1_array.log
./ada_p1 p1 data/eventos_100k.txt lista results/p1_lista.log

# Problema 2 — C, T y L son argumentos, no constantes del programa
./ada_p1 p2 data/paquetes_100k.txt 1024 1000 200 results/p2.log
```

### Formato de entrada

**Problema 1** — un evento por línea:

```
EDIT <INSERT|DELETE|REPLACE> <posición> <contenido> <texto_previo>
UNDO
REDO
```

El campo `<texto_previo>` es una extensión sobre el formato del enunciado. Es
necesario para revertir `DELETE` y `REPLACE` sin re-simular el documento desde
el inicio; el guion `-` representa cadena vacía. Justificar esta decisión en el
informe.

**Problema 2** — un paquete por línea: `<timestamp_ms> <tamaño_bytes>`, con
marcas de tiempo no decrecientes.

## Casos de prueba

```bash
make tests          # o: bash run_tests.sh
```

Ejecuta los 7 casos de cada problema y deja log y reporte de cada uno en
`results/tests/`. Cada archivo de `tests/` lleva en su cabecera el resultado
esperado y, en el Problema 2, los valores de `C`, `T` y `L` con los que debe
ejecutarse: están elegidos para **aislar la causa** de cada rechazo (p. ej. el
caso de búfer lleno usa `L` alto para que el limitador de tasa no interfiera).

El Problema 1 se corre con las dos representaciones de pila y el script
**verifica que ambas produzcan el mismo log**. Esa igualdad es la evidencia
experimental de la distinción TAD vs. implementación que pide la Sección 6, y
conviene citarla en el informe.

## Reproducción de los experimentos

**Semilla fija utilizada: `20260828`** (cambiar aquí si el equipo usa otra, y
mantenerla consistente con el informe).

```bash
for n in 1000 10000 100000 1000000; do
  ./ada_p1 bench $n 20260828 5
done
```

`bench` genera las instancias, ejecuta cada configuración 5 veces y reporta media
y desviación estándar en milisegundos con dos decimales, como exige la Sección 9.

> **Advertencia metodológica:** el tiempo reportado por `bench` incluye la lectura
> y el parseo del archivo, que son O(n) y dominan las constantes. Para aislar el
> costo real de las estructuras conviene cargar los eventos a memoria primero y
> cronometrar solo el bucle de operaciones. Esa decisión debe explicarse en la
> Sección 10 del informe.

## Resumen de resultados principales

_(Completar tras la experimentación: tabla n vs. tiempo medio ± sd para las tres
configuraciones, y la evidencia del comportamiento amortizado.)_

## Qué falta por implementar

Todos los `TODO` de `src/` corresponden a las operaciones que el enunciado evalúa:

| Archivo | Pendiente |
|---------|-----------|
| `stack_array.cpp` | `push`, `pop`, `top`, `grow` (política de crecimiento) |
| `stack_list.cpp` | `push`, `pop`, `top` |
| `queue_circular.cpp` | `enqueue`, `dequeue`, `front`, `isEmpty`, `isFull` |
| `queue_list.cpp` | `enqueue`, `dequeue`, `front` |
| `undoredo.cpp` | `aplicar`, `revertir`, `aplicarEdicion`, `deshacer`, `rehacer` |
| `ratelimiter.cpp` | `purgarExpirados`, `procesar` |

Cada `TODO` incluye el contrato de la operación y las decisiones de diseño que
hay que tomar y justificar.

## Uso de herramientas de IA

_(Sección 15 del enunciado: la citación es obligatoria. Registrar aquí y en el
informe qué se consultó, con qué herramienta y qué se hizo con la respuesta.)_
