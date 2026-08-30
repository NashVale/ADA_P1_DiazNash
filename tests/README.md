# Casos de prueba obligatorios (Sección 11)

Cada archivo corresponde a un ítem de la sección 11 del enunciado.
Los parámetros `C`, `T` y `L` del Problema 2 van en la cabecera comentada de
cada archivo, porque el enunciado prohíbe codificarlos fijos en el programa.

## Ejecución

```bash
./ada_p1 p1 tests/p1/01_secuencia_mixta.txt array   results/p1_01_array.log
./ada_p1 p1 tests/p1/01_secuencia_mixta.txt lista   results/p1_01_lista.log
./ada_p1 p2 tests/p2/05_rafaga_excede_L.txt 100 1000 5 results/p2_05.log
```

## Resultado esperado — a completar por el equipo

Esta tabla es parte del entregable: para cada caso deben escribir qué esperan
**antes** de ejecutar, y luego contrastar. Un caso de prueba sin resultado
esperado documentado no prueba nada.

| Caso | Qué verifica | Resultado esperado |
|------|--------------|--------------------|
| p1_01 | Secuencia mixta | |
| p1_02 | UNDO sobre pila vacía → NO-OP, sin caída | |
| p1_03 | Ciclo edición→undo→redo completo | |
| p1_04 | Un EDIT invalida el Redo (regla R1) | |
| p1_05 | N undos consecutivos vacían la pila | |
| p1_06 | REDO de más → NO-OP reportado | |
| p1_07 | Crecimiento de capacidad del arreglo | |
| p2_01 | Camino feliz | |
| p2_02 | Consulta sobre búfer vacío | |
| p2_03 | Un solo paquete | |
| p2_04 | Búfer exactamente lleno + 1 | |
| p2_05 | Ráfaga que supera L dentro de T | |
| p2_06 | Dequeue sobre búfer vacío | |
| p2_07 | Borde t = t0 + T (decisión del equipo) | |

**Los dos casos que más se preguntan en sustentación son p1_04 y p2_07.**
El primero porque es la regla que distingue un editor real de una pila
cualquiera; el segundo porque no tiene respuesta única y obliga a justificar.
