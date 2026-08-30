#include "stack_list.hpp"

StackList::StackList() : cabeza_(nullptr), n_(0), nodosCreados_(0) {}

StackList::~StackList() { clear(); }

bool        StackList::isEmpty() const { return cabeza_ == nullptr; }
std::size_t StackList::size()    const { return n_; }

void StackList::clear() {
    // Ya resuelto: recorrido y liberación nodo a nodo.
    // OJO — no usen recursión aquí: con n = 10^6 nodos desbordarían la pila
    // de llamadas. Es un caso límite real que conviene mencionar en el informe.
    Nodo* actual = cabeza_;
    while (actual != nullptr) {
        Nodo* sig = actual->sig;
        delete actual;
        actual = sig;
    }
    cabeza_ = nullptr;
    n_ = 0;
}

// ---- Núcleo evaluable -----------------------------------------------------

void StackList::push(const EditOp& op) {
    // TODO
    //   cabeza_ = new Nodo(op, cabeza_);  ++n_;  ++nodosCreados_;
    //   Complejidad: Theta(1) en el peor caso — no hay redimensionamiento.
    //   Espacial: Theta(1) adicional por elemento, más el puntero.
    (void)op;
}

bool StackList::pop(EditOp& out) {
    // TODO
    //   Si isEmpty() -> false
    //   Si no: guardar cabeza_, out = cabeza_->dato, cabeza_ = cabeza_->sig,
    //          delete del nodo viejo, --n_, return true.
    //   CUIDADO con el orden: no lean cabeza_->sig después del delete.
    (void)out;
    return false;
}

bool StackList::top(EditOp& out) const {
    // TODO
    (void)out;
    return false;
}
