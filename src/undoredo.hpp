#ifndef UNDOREDO_HPP
#define UNDOREDO_HPP

#include "tad.hpp"
#include <string>
#include <iosfwd>

// ===========================================================================
//  PROBLEMA 1 — Sistema de Undo/Redo de un editor de código.
//
//  El motor NO sabe si las pilas son de arreglo o de lista: recibe punteros
//  a IStack. Eso es precisamente la demostración de "TAD vs implementación"
//  que exige la Sección 6: el mismo motor funciona con ambas representaciones
//  sin cambiar una sola línea.
//
//  REGLAS DEL EDITOR (Sección 4 del enunciado):
//    R1. EDIT  -> aplicar al documento, apilar en Undo, y VACIAR Redo.
//    R2. UNDO  -> desapilar de Undo, revertir en el documento, apilar en Redo.
//                 Si Undo está vacía: no-op reportado, no error fatal.
//    R3. REDO  -> desapilar de Redo, reaplicar, apilar en Undo.
//                 Si Redo está vacía: no-op reportado.
// ===========================================================================

struct EstadisticasUndoRedo {
    std::size_t edicionesAplicadas = 0;
    std::size_t undosEfectivos     = 0;
    std::size_t undosNoOp          = 0;
    std::size_t redosEfectivos     = 0;
    std::size_t redosNoOp          = 0;
    std::size_t redosInvalidados   = 0;  // elementos descartados por la regla R1
};

class UndoRedoEngine {
public:
    // No toma posesión de las pilas: el llamador las crea y las destruye.
    UndoRedoEngine(IStack* pilaUndo, IStack* pilaRedo);

    // --- API pública -------------------------------------------------------
    void aplicarEdicion(const EditOp& op);  // R1
    bool deshacer();                        // R2 — false si fue no-op
    bool rehacer();                         // R3 — false si fue no-op

    const std::string&          documento()    const { return doc_; }
    const EstadisticasUndoRedo& estadisticas() const { return stats_; }
    std::size_t tamPilaUndo() const;
    std::size_t tamPilaRedo() const;

    // Procesa un archivo de eventos completo y escribe el log en 'log'.
    // Devuelve el número de líneas procesadas. (Parsing ya resuelto.)
    std::size_t procesarArchivo(const std::string& ruta, std::ostream& log);

private:
    // -----------------------------------------------------------------------
    //  TODO (núcleo evaluable)
    //
    //  aplicar(op):  muta doc_ según op.type.
    //      INSERT  -> insertar op.content en la posición op.pos
    //      DELETE  -> borrar op.previous.size() caracteres desde op.pos
    //      REPLACE -> sustituir op.previous por op.content en op.pos
    //      Deben validar que op.pos sea una posición válida y decidir qué
    //      hacer si no lo es (recortar, ignorar y reportar, ...). Justifíquenlo.
    //
    //  revertir(op): aplica la operación INVERSA.
    //      El inverso de INSERT es un DELETE del mismo texto en la misma
    //      posición; el de DELETE es un INSERT de op.previous; el de REPLACE
    //      es otro REPLACE intercambiando content y previous.
    //
    //  OJO para el análisis de complejidad: insertar o borrar en medio de una
    //  cadena es O(longitud del documento), NO O(1). Es decir, las operaciones
    //  de la PILA son O(1), pero la operación del EDITOR no lo es. Separen
    //  claramente ambos costos en la Sección 8.3; confundirlos es el error
    //  más común en esta práctica.
    // -----------------------------------------------------------------------
    void aplicar(const EditOp& op);
    void revertir(const EditOp& op);

    IStack*              undo_;
    IStack*              redo_;
    std::string          doc_;      // documento simulado (std::vector/std::string
                                    // permitidos aquí: es estructura auxiliar,
                                    // no el objeto de evaluación — Sección 7)
    EstadisticasUndoRedo stats_;
};

#endif // UNDOREDO_HPP
