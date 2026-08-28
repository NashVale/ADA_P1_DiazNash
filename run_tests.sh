#!/usr/bin/env bash
# ===========================================================================
#  Ejecuta los 14 casos de prueba obligatorios de la Sección 11 y guarda
#  cada salida en results/tests/ para adjuntarla al informe.
#
#  El Problema 1 se corre con LAS DOS representaciones de pila: si el TAD
#  está bien implementado, la salida debe ser IDÉNTICA para 'array' y
#  'lista'. Esa igualdad es, en sí misma, la evidencia experimental de la
#  distinción TAD vs. implementación que pide la Sección 6.
#
#  Los parámetros C, T y L de cada caso del Problema 2 están fijados aquí a
#  propósito para AISLAR la causa del rechazo que ese caso quiere probar.
# ===========================================================================
set -u

BIN=./ada_p1
OUT=results/tests
mkdir -p "$OUT"

if [ ! -x "$BIN" ]; then
    echo "ERROR: no existe $BIN. Ejecute 'make' primero."
    exit 1
fi

echo "==================== PROBLEMA 1 ===================="
for f in tests/p1/*.txt; do
    caso=$(basename "$f" .txt)
    for rep in array lista; do
        echo "--- $caso [$rep]"
        $BIN p1 "$f" "$rep" "$OUT/${caso}_${rep}.log" > "$OUT/${caso}_${rep}.out"
        tail -n 12 "$OUT/${caso}_${rep}.out"
    done
    # Si el TAD está bien implementado, ambas representaciones coinciden.
    if diff -q "$OUT/${caso}_array.log" "$OUT/${caso}_lista.log" > /dev/null; then
        echo "    [OK]   array y lista producen el mismo log"
    else
        echo "    [FALLA] array y lista DIFIEREN -> revisar la implementacion"
    fi
done

echo
echo "==================== PROBLEMA 2 ===================="
# caso                            C      T       L
run_p2() {
    local f="$1" C="$2" T="$3" L="$4"
    local caso; caso=$(basename "$f" .txt)
    echo "--- $caso  (C=$C T=$T L=$L)"
    $BIN p2 "$f" "$C" "$T" "$L" "$OUT/${caso}.log" > "$OUT/${caso}.out"
    tail -n 12 "$OUT/${caso}.out"
}

run_p2 tests/p2/01_flujo_normal.txt          8   1000     5
run_p2 tests/p2/02_buffer_vacio.txt          8   1000     5
run_p2 tests/p2/03_un_paquete.txt            8   1000     5
run_p2 tests/p2/04_buffer_lleno_mas_uno.txt  4   100000   1000
run_p2 tests/p2/05_rafaga_excede_L.txt       100 1000     3
run_p2 tests/p2/06_dequeue_buffer_vacio.txt  8   1000     5
run_p2 tests/p2/07_borde_ventana.txt         8   1000     1

echo
echo "Salidas completas en $OUT/"
