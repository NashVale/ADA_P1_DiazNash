# Decisiones de diseño y sus justificaciones

## 1. Política de crecimiento de la pila sobre arreglo — `grow()`

**Elegida:** duplicación, `nuevaCap = cap_ * 2`.

**Justificación:** con duplicación, hacer n `push` consecutivos cuesta
`1 + 2 + 4 + ... + n < 2n` traslados en total, luego el costo **amortizado**
por operación es O(1), aunque el peor caso puntual de un `push` concreto sea
Θ(n).

**Alternativa descartada:** crecer en una constante (`cap_ + k`). El total
pasa a ser del orden de `n²/(2k)` traslados, es decir **O(n) amortizado por
push**. Es el contraejemplo ideal para la Sección 10: si hay tiempo, medir
ambas y graficar la diferencia.

**Evidencia medida:** con capacidad inicial 8 y 200 `push`, se obtienen
5 redimensionamientos y 248 copias acumuladas — exactamente lo que predice
la serie geométrica.

---

## 2. Uso de `std::move` en el traslado

Mover las cadenas en vez de copiarlas reduce la **constante**, no el orden
asintótico: siguen siendo n traslados. Es un buen ejemplo de "constante
oculta por la notación asintótica" para la Sección 10.

---

## 3. Cómo distinguir cola llena de cola vacía — `QueueCircular`

**Elegida:** contador explícito `n_`. Vacía ⟺ `n_ == 0`; llena ⟺ `n_ == cap_`.

**Justificación:** con solo `head_` y `tail_`, la condición `head_ == tail_`
es **ambigua**: se cumple tanto con la cola vacía como con la cola llena. El
contador desambigua sin perder capacidad útil y mantiene `size()` en O(1).

**Alternativas descartadas:** sacrificar una celda (`(tail_+1) % cap_ == head_`)
obliga a reservar C+1 posiciones para admitir C paquetes; una bandera booleana
exige actualizarla en cada operación y es más fácil de desincronizar.

---

## 4. Borde de la ventana deslizante — `purgarExpirados()`

**Elegida:** ventana **abierta por la izquierda**, intervalo `(t−T, t]`. Una
marca expira cuando `ts <= t_actual − T`.

**Consecuencia:** un paquete que llega exactamente en `t0 + T` ya **no** cuenta
al de `t0`. La Sección 5 del enunciado exige elegir explícitamente y justificar;
la convención contraria (`ts < t_actual − T`, intervalo cerrado `[t−T, t]`) es
igual de defendible siempre que se declare.

**Argumento amortizado (obligatorio, Sección 8.3):** una sola llamada puede
purgar hasta k marcas, luego el peor caso puntual es O(n). Pero cada marca se
encola exactamente una vez y se desencola a lo sumo una vez en toda la
ejecución, así que el total de purgas sobre n paquetes está acotado por n:
el costo **amortizado por paquete es O(1)**. Se evidencia comprobando que
`marcasPurgadasTot <= n` mientras `purgaMaximaPuntual >> 1`.

---

## 5. Orden de comprobación en el firewall — `procesar()`

**Elegido:** primero el límite de tasa, después la capacidad del búfer.

**Justificación:** el limitador es la defensa exterior del firewall; un origen
abusivo debe frenarse aunque haya sitio libre en el búfer. Invertir el orden
cambia las estadísticas finales, así que la decisión debe declararse.

**Decisión asociada:** solo los paquetes **aceptados** dejan marca en la
ventana. Un paquete descartado no consumió recursos de procesamiento, luego no
debería penalizar al origen. La postura contraria (registrar todo intento) es
más estricta frente a un ataque de denegación de servicio y también se defiende.

---

## 6. Posición inválida en una edición — `aplicar()`

**Elegida:** recortar al final del documento (`pos = min(op.pos, doc_.size())`).

**Justificación:** ninguna edición se pierde en silencio y el motor nunca
aborta, que es lo que exige la Sección 7. La alternativa (ignorar la operación
y reportarla) también es válida, pero rompe la simetría entre `aplicar()` y
`revertir()`.

---

## 7. Contrato de fallo: `bool` + parámetro de salida

Las operaciones que pueden fallar (`pop` sobre pila vacía, `enqueue` sobre cola
llena) **no lanzan excepción ni abortan**: devuelven `bool` y escriben el
resultado en un parámetro de salida. Cumple el requisito de la Sección 7
("códigos de retorno claros, no se aceptan fallos silenciosos ni caídas").
Las excepciones son una alternativa válida, pero cambian el costo constante
de la operación.

---

## 8. Nota para el análisis de complejidad (error frecuente)

Las operaciones de la **pila** son O(1), pero insertar o borrar en medio de una
`std::string` es O(|documento|). El costo total del Problema 1 **no** es lineal,
y la causa es el editor, no la pila. Ambos costos deben separarse en la
Sección 8.3.

Además, cada `EditOp` guarda dos cadenas (`content` y `previous`), luego la
complejidad **espacial** de la pila es O(Σ|texto|), no O(n).
