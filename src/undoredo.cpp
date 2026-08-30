#include "undoredo.hpp"

#include <fstream>
#include <sstream>
#include <ostream>

UndoRedoEngine::UndoRedoEngine(IStack* pilaUndo, IStack* pilaRedo)
    : undo_(pilaUndo), redo_(pilaRedo), doc_(), stats_() {}

std::size_t UndoRedoEngine::tamPilaUndo() const { return undo_->size(); }
std::size_t UndoRedoEngine::tamPilaRedo() const { return redo_->size(); }

void UndoRedoEngine::aplicar(const EditOp& op) {
    std::size_t pos = op.pos <= doc_.size() ? op.pos : doc_.size();

    switch (op.type) {
        case EditType::INSERT:
            doc_.insert(pos, op.content);
            break;

        case EditType::DELETE: {
            std::size_t cuantos = op.previous.size();
            if (pos + cuantos > doc_.size()) cuantos = doc_.size() - pos;
            doc_.erase(pos, cuantos);
            break;
        }

        case EditType::REPLACE: {
            std::size_t cuantos = op.previous.size();
            if (pos + cuantos > doc_.size()) cuantos = doc_.size() - pos;
            doc_.replace(pos, cuantos, op.content);
            break;
        }
    }
}

void UndoRedoEngine::revertir(const EditOp& op) {
    EditOp inv = op;

    switch (op.type) {
        case EditType::INSERT:
            inv.type     = EditType::DELETE;
            inv.previous = op.content;
            inv.content.clear();
            break;

        case EditType::DELETE:
            inv.type    = EditType::INSERT;
            inv.content = op.previous;
            break;

        case EditType::REPLACE:
            inv.type     = EditType::REPLACE;
            inv.content  = op.previous;
            inv.previous = op.content;
            break;
    }
    aplicar(inv);
}

void UndoRedoEngine::aplicarEdicion(const EditOp& op) {
    aplicar(op);
    undo_->push(op);

    stats_.redosInvalidados += redo_->size();
    redo_->clear();

    ++stats_.edicionesAplicadas;
}

bool UndoRedoEngine::deshacer() {
    EditOp op;
    if (!undo_->pop(op)) {
        ++stats_.undosNoOp;
        return false;
    }
    revertir(op);
    redo_->push(op);
    ++stats_.undosEfectivos;
    return true;
}

bool UndoRedoEngine::rehacer() {
    EditOp op;
    if (!redo_->pop(op)) {
        ++stats_.redosNoOp;
        return false;
    }
    aplicar(op);
    undo_->push(op);
    ++stats_.redosEfectivos;
    return true;
}

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
