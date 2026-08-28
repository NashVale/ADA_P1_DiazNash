# Esqueleto del informe técnico

Máximo 8 páginas sin contar portada (Sección 13). Las 14 secciones son
obligatorias y van en este orden. El pseudocódigo **no** puede sustituirse por
código C++.

---

## 1. Portada
Universidad, curso, título de la práctica, integrantes en orden alfabético, fecha.

## 2. Introducción
Qué se construyó y qué se midió. Media página.

## 3. Contexto de los dos problemas
Por qué el editor es LIFO y el firewall es FIFO. No repetir el enunciado: la
pregunta que se responde es *por qué esa estructura y no otra*.

## 4. Fundamentación teórica
Lo exigido en la Sección 6 del enunciado:

- **Pila:** LIFO; `push`/`pop`/`top`; arreglo dinámico vs. lista enlazada;
  "pila llena" con capacidad fija vs. redimensionamiento; aplicaciones reales
  (evaluación de expresiones, pila de llamadas, backtracking, recorrido DFS).
- **Cola:** FIFO; `enqueue`/`dequeue`/`front`/`rear`; arreglo circular vs. lista;
  **cómo se distingue cola llena de cola vacía** cuando `head == tail`.
- **Listas enlazadas:** nodo, enlace, cabeza, recorrido, inserción, eliminación,
  búsqueda; acceso indexado vs. secuencial; overhead de un puntero por nodo.

## 5. Diseño de las soluciones (TAD e implementaciones)
Transcribir el contrato de `IStack` e `IQueue` (está en `src/tad.hpp`) y mostrar
que las cuatro clases concretas lo satisfacen. Discutir **qué cambia** (costo
puntual, memoria, localidad) y **qué permanece igual** (interfaz, semántica,
casos límite) desde el punto de vista de quien usa la estructura.

Evidencia experimental disponible: `run_tests.sh` comprueba que las dos
representaciones de pila producen logs idénticos sobre los 7 casos.

### Decisiones de diseño a justificar explícitamente

| Decisión | Opciones | Elegida | Justificación |
|---|---|---|---|
| Crecimiento de la pila de arreglo | ×2 / ×1.5 / +k | | |
| Lleno vs. vacío en la cola circular | contador `n_` / sacrificar una celda / bandera | | |
| Borde de la ventana `t = t0 + T` | `marca < t−T` / `marca <= t−T` | | |
| Orden de comprobación en el firewall | tasa antes que búfer / al revés | | |
| Fallo de operación | `bool` + parámetro de salida / excepción | | |
| Posición inválida en una edición | recortar / ignorar y reportar | | |

## 6. Pseudocódigo de las operaciones fundamentales
`push`, `pop`, `grow`, `enqueue`, `dequeue`, `purgarExpirados`, `deshacer`,
`rehacer`. Independiente del C++.

## 7. Análisis de complejidad

### 7.1 Tabla — Problema 1 (Pila)

| Operación | Mejor | Promedio | Peor | Espacial | Justificación |
|---|---|---|---|---|---|
| `push` (arreglo, sin redimensionar) | | | | | |
| `push` (arreglo, con redimensionamiento) | | | | | costo puntual vs. amortizado sobre n |
| `push` (lista enlazada) | | | | | |
| `pop` / `top` (ambas) | | | | | |
| `isEmpty` | | | | | |
| `clear` (invalidar Redo) | | | | | |

### 7.2 Tabla — Problema 2 (Cola)

| Operación | Mejor | Promedio | Peor | Espacial | Justificación |
|---|---|---|---|---|---|
| `enqueue` (circular) | | | | | |
| `dequeue` (circular) | | | | | |
| Purga de marcas expiradas | | | | | puntual vs. amortizado sobre n paquetes |
| `isFull` / `isEmpty` | | | | | regla para distinguir llena de vacía |

Todo en notación O, Ω o Θ, con el conteo de operaciones elementales que lo
sustenta. No se aceptan respuestas cualitativas.

### 7.3 Análisis global
Por cada problema: complejidad temporal total sobre n eventos/paquetes,
complejidad espacial en el peor caso, y si hace falta argumento amortizado.

**Dos trampas que conviene no pisar:**

1. Las operaciones de la **pila** son O(1), pero insertar o borrar en medio de
   una cadena es O(|documento|). El costo total del Problema 1 no es O(n) por
   culpa del *editor*, no de la pila. Separar ambos costos.
2. Cada `EditOp` guarda dos cadenas, así que la pila ocupa O(Σ|texto|), no O(n).

**Argumento amortizado de la purga (obligatorio):** una llamada puede purgar
hasta k marcas, luego el peor caso puntual es O(n); pero cada marca se encola
una vez y se desencola a lo sumo una vez, así que el total de purgas sobre toda
la ejecución está acotado por n y el costo amortizado por paquete es O(1). Se
evidencia con `marcasPurgadasTot` ≤ n mientras `purgaMaximaPuntual` ≫ 1.

## 8. Casos de prueba
Los 14 de la Sección 11, con entrada, salida esperada y salida obtenida.
Referenciar `tests/p1/` y `tests/p2/`.

## 9. Experimentación
n ∈ {1 000, 10 000, 100 000, 1 000 000}, 5 repeticiones, media y desviación
estándar en ms con dos decimales, más la métrica de dominio de cada problema.
Declarar máquina, compilador y nivel de optimización.

## 10. Comparación teórico-experimental
¿El crecimiento medido es consistente con lo deducido? Constantes ocultas;
jerarquía de memoria (por qué el arreglo suele ganarle a la lista aunque ambos
sean O(1)); evidencia visual del comportamiento amortizado.

Comparaciones que dan buen material y ya están instrumentadas:
- `-O0` vs. `-O2` (`make debug` vs. `make`).
- Crecimiento ×2 vs. `+k`: `+k` deja de ser O(1) amortizado y se ve en la gráfica.
- Llamada virtual a través de `IStack*` vs. clase concreta: ambos O(1), distinta
  constante.

## 11. Conclusiones

## 12. Uso de herramientas de IA
Obligatorio y con citación (Sección 15). Registrar qué se consultó, con qué
herramienta y qué se hizo con la respuesta. Los usos permitidos son consultas
conceptuales, errores de compilación, retroalimentación sobre código ya escrito,
discusión de alternativas de diseño y apoyo de redacción.

## 13. Referencias

## 14. Contribución individual
Por integrante: qué diseñó, qué implementó, qué probó, qué analizó y qué
documentó. No se aceptan descripciones genéricas.
