# Práctica 1 — Estructuras unidimensionales

**Nash Díaz** · Trabajo de Estructuras de Datos y Algoritmos 💗

Dos programas que resuelven problemas reales usando pilas y colas construidas
desde cero, sin `std::stack`, `std::queue`, `std::deque` ni `std::list`.

---

## Qué hace

**Problema 1 — Undo/Redo de un editor de código.**
Lee un archivo de ediciones y comandos, y mantiene dos pilas: una para deshacer
y otra para rehacer. Deshacer siempre revierte lo último que pasó, que es el
comportamiento LIFO de toda la vida. Si escribes algo nuevo después de un
deshacer, el historial de rehacer se borra, igual que en cualquier editor real.

**Problema 2 — Búfer y limitador de tasa de un firewall.**
Lee un flujo de paquetes y decide qué hacer con cada uno. Los guarda en un búfer
circular de capacidad fija, y si está lleno los descarta. Además lleva una
ventana deslizante de tiempo: si en los últimos T milisegundos ya llegaron L
paquetes, rechaza el siguiente por exceso de tasa.

Cada estructura está implementada en **dos versiones distintas** (arreglo y
lista enlazada) sobre la misma interfaz, para mostrar que el tipo abstracto no
depende de cómo esté hecho por dentro.

---

## Qué hay adentro

```
src/        el código: dos pilas, dos colas, la lógica de cada problema
tests/      14 casos de prueba, 7 por problema
data/       aquí caen los archivos generados (no se versionan, pesan mucho)
results/    los tiempos medidos
report/     el informe y las decisiones de diseño
```

---

## Cómo compilarlo

**Windows (PowerShell):**

```powershell
g++ -std=c++17 -O2 -o ada_p1.exe src/generator.cpp src/main.cpp src/queue_circular.cpp src/queue_list.cpp src/ratelimiter.cpp src/stack_array.cpp src/stack_list.cpp src/undoredo.cpp
```

**Linux o Mac:**

```bash
make
```

Si no dice nada, compiló bien.

---

## Cómo usarlo

```
ada_p1 gen1  <n> <semilla> <salida>              genera eventos de edición
ada_p1 gen2  <n> <semilla> <salida>              genera paquetes de red
ada_p1 p1    <entrada> <array|lista> [log]       corre el editor
ada_p1 p2    <entrada> <C> <T_ms> <L> [log]      corre el firewall
ada_p1 bench <n> <semilla> <repeticiones>        mide tiempos
```

Ejemplo:

```powershell
.\ada_p1.exe p1 tests/p1/03_una_edicion_undo_redo.txt array
.\ada_p1.exe p2 tests/p2/05_rafaga_excede_L.txt 100 1000 3
```

En el firewall, `C` es la capacidad del búfer, `T_ms` el tamaño de la ventana en
milisegundos y `L` cuántos paquetes se permiten dentro de esa ventana. Los tres
se pasan por consola, no están fijos en el código.

---

## Casos de prueba

```bash
bash run_tests.sh
```

Corre los 14 casos y guarda las salidas en `results/tests/`. Cada archivo de
`tests/` trae en su cabecera el resultado que se espera y, en el firewall, con
qué parámetros hay que ejecutarlo.

El script corre el Problema 1 con las dos pilas y compara los logs. Si el tipo
abstracto está bien hecho, ambos tienen que salir idénticos — y salen.

---

## Experimentos

**Semilla: `20260830`.** Con esa semilla los datos generados son siempre los
mismos, así que cualquiera puede repetir estas mediciones y obtener lo mismo.

```powershell
.\ada_p1.exe bench 1000 20260830 5    | Tee-Object results\experimentos.txt
.\ada_p1.exe bench 10000 20260830 5   | Tee-Object -Append results\experimentos.txt
.\ada_p1.exe bench 100000 20260830 5  | Tee-Object -Append results\experimentos.txt
.\ada_p1.exe bench 1000000 20260830 5 | Tee-Object -Append results\experimentos.txt
```

Cada tamaño se corre 5 veces y se reporta media y desviación estándar.

### Resultados medidos

| n | Pila arreglo | Pila lista | Firewall |
|---:|---:|---:|---:|
| 1 000 | 4.38 ms (sd 8.51) | 0.58 ms (sd 0.03) | 0.63 ms (sd 0.03) |
| 10 000 | 10.93 ms (sd 7.34) | 5.92 ms (sd 0.71) | 8.87 ms (sd 1.84) |
| 100 000 | 78.45 ms (sd 20.88) | 73.15 ms (sd 10.96) | 60.00 ms (sd 3.90) |
| 1 000 000 | 1560.12 ms (sd 13.43) | 1699.60 ms (sd 202.53) | 1055.90 ms (sd 409.97) |

### Qué muestran estos números

**El firewall se mantiene casi plano** por paquete (entre 0.6 y 1.06
microsegundos). Esa estabilidad es la confirmación de que la purga de la ventana
es O(1) amortizado: aunque una llamada suelta pueda borrar muchas marcas, cada
marca entra y sale una sola vez en toda la ejecución.

**El editor se dispara al final**, pero no por culpa de la pila. Las operaciones
de la pila siguen siendo O(1); lo que crece es el costo de insertar en medio del
documento, que depende de su longitud. Con un millón de ediciones el documento
es enorme y ese costo domina.

**El arreglo le gana a la lista en n grande** (1560 contra 1699 ms). Los dos son
O(1) por operación, así que la diferencia es localidad de memoria: el arreglo
guarda todo junto y aprovecha la caché, mientras la lista dispersa los nodos.

**En n = 1000 la medición es ruido.** La desviación estándar de la pila de
arreglo es el doble de la media, porque a ese tamaño el trabajo real es tan
pequeño que lo que se mide es el arranque del programa. Por eso el enunciado
pide llegar hasta 10⁵ y 10⁶.

---

## Decisiones de diseño

Las que hubo que tomar y no venían dadas por el enunciado:

- La pila de arreglo **duplica** su capacidad al llenarse.
- La cola circular usa un **contador** para distinguir llena de vacía, porque
  con solo los índices la condición es ambigua.
- La ventana de tiempo es **abierta por la izquierda**: un paquete que llega
  exactamente en `t0 + T` ya no cuenta al de `t0`.
- El límite de tasa se comprueba **antes** que la capacidad del búfer.
- Solo los paquetes aceptados dejan marca en la ventana.

Cada una está argumentada, con sus alternativas, en
[`report/DECISIONES_DE_DISENO.md`](report/DECISIONES_DE_DISENO.md).

---

## Condiciones de ejecución

Windows · GCC 13.1.0 · compilado con `-O2` · medido con
`std::chrono::high_resolution_clock`.

Procesador: 11th Gen Intel(R) Core(TM) i3-1115G4 @ 3.00GHz

---

## Uso de herramientas de IA

Se usó Claude (Anthropic) para discutir alternativas de diseño, revisar el
código y apoyar la redacción del informe en formato LATEX. 
