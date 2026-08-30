#ifndef GENERATOR_HPP
#define GENERATOR_HPP

#include <cstddef>
#include <string>

// ===========================================================================
//  Generadores de instancias sintéticas DETERMINISTAS (Sección 9 y 12).
//
//  Todo depende de una semilla explícita: misma semilla => mismo archivo,
//  en cualquier máquina. Documenten la semilla usada en el README.md, tal
//  como exige la Sección 17.
//
//  Se usa std::mt19937 (Mersenne Twister) con distribuciones de <random>.
//  NO usen rand(): su implementación varía entre compiladores y rompería la
//  reproducibilidad.
// ===========================================================================

// Problema 1: genera n eventos EDIT/UNDO/REDO.
//   pctUndo, pctRedo: porcentajes enteros (0-100) de eventos de cada comando;
//   el resto son ediciones.
void generarEventosEditor(const std::string& rutaSalida,
                          std::size_t n,
                          unsigned semilla,
                          int pctUndo = 20,
                          int pctRedo = 10);

// Problema 2: genera n paquetes con marcas de tiempo NO DECRECIENTES.
//   Se intercalan ráfagas para que el limitador de tasa se active de verdad;
//   si el tráfico fuera uniforme y por debajo de L, nunca se probaría el
//   camino de rechazo y el experimento no diría nada interesante.
void generarPaquetes(const std::string& rutaSalida,
                     std::size_t n,
                     unsigned semilla,
                     int gapMedioMs = 5);

#endif // GENERATOR_HPP
