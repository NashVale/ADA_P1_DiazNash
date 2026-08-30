#include "undoredo.hpp"

#include <fstream>
#include <sstream>
#include <ostream>

UndoRedoEngine::UndoRedoEngine(IStack* pilaUndo, IStack* pilaRedo)
    : undo_(pilaUndo), redo_(pilaRedo), doc_(), stats_() {}

std::size_t UndoRedoEngine::tamPilaUndo() const { return undo_->size(); }
std::size_t UndoRedoEngine::tamPilaRedo() const { return redo_->size(); }

// ---- Núcleo evaluable -----------------------------------------------------

void UndoRedoEngine::aplicar(const EditOp& op) {
    // TODO — ver contrato en undoredo.hpp
    (void)op;
}

void UndoRedoEngine::revertir(const EditOp& op) {
    // TODO — ver contrato en undoredo.hpp
    (void)op;
}

void UndoRedoEngine::aplicarEdicion(const EditOp& op) {
    // TODO (regla R1)
    //   1. aplicar(op)
    //   2. undo_->push(op)
    //   3. stats_.redosInvalidados += redo_->size();  redo_->clear();
    //   4. ++stats_.edicionesAplicadas;
    //
    //   El paso 3 es el que verifica el caso de prueba 4 de la Sección 11.
    (void)op;
}

bool UndoRedoEngine::deshacer() {
    // TODO (regla R2)
    //   EditOp op;
    //   if (!undo_->pop(op)) { ++stats_.undosNoOp; return false; }
    //   revertir(op); redo_->push(op); ++stats_.undosEfectivos; return true;
    return false;
}

bool UndoRedoEngine::rehacer() {
    // TODO (regla R3) — simétrico a deshacer(), pero SIN vaciar la pila Undo.
    return false;
}

// ---- Lectura del archivo de eventos (ya resuelto) --------------------------
//
// Formato de entrada (Sección 4):
//   EDIT <INSERT|DELETE|REPLACE> <posicion> <contenido> [<texto_previo>]
//   UNDO
//   REDO
//
// El campo <texto_previo> es una extensión nuestra: hace falta para poder
// revertir DELETE y REPLACE de forma autónoma. Documéntenlo en el README.
// Para INSERT se ignora; para DELETE, <contenido> puede ir vacío como "-".

std::size_t UndoRedoEngine::procesarArchivo(const std::string& ruta,
                                            std::ostream& log)
{
    std::ifstream in(ruta);
    if (!in) {
        log << "ERROR: no se pudo abrir el archivo " << ruta << "\n";
        return 0;
    }

    std::string linea;
    std::size_t nLinea = 0;

    while (std::getline(in, linea)) {
        ++nLinea;
        if (linea.empty() || linea[0] == '#') continue;

        std::istringstream ss(linea);
        std::string comando;
        ss >> comando;

        if (comando == "UNDO") {
            bool ok = deshacer();
            log << nLinea << "\tUNDO\t" << (ok ? "OK" : "NO-OP (pila Undo vacia)") << "\n";
        }
        else if (comando == "REDO") {
            bool ok = rehacer();
            log << nLinea << "\tREDO\t" << (ok ? "OK" : "NO-OP (pila Redo vacia)") << "\n";
        }
        else if (comando == "EDIT") {
            std::string tipo, contenido, previo;
            std::size_t pos = 0;
            ss >> tipo >> pos >> contenido;
            if (!(ss >> previo)) previo = "";
            if (contenido == "-") contenido = "";
            if (previo    == "-") previo    = "";

            EditOp op;
            if      (tipo == "INSERT")  op.type = EditType::INSERT;
            else if (tipo == "DELETE")  op.type = EditType::DELETE;
            else if (tipo == "REPLACE") op.type = EditType::REPLACE;
            else {
                log << nLinea << "\tERROR\ttipo de edicion desconocido: " << tipo << "\n";
                continue;
            }
            op.pos      = pos;
            op.content  = contenido;
            op.previous = previo;

            aplicarEdicion(op);
            log << nLinea << "\tEDIT\t" << tipo << " @" << pos << "\tOK\n";
        }
        else {
            log << nLinea << "\tERROR\tcomando desconocido: " << comando << "\n";
        }
    }
    return nLinea;
}
