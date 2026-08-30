#include "queue_list.hpp"

QueueList::QueueList()
    : frente_(nullptr), final_(nullptr), n_(0), nodosCreados_(0) {}

QueueList::~QueueList() { clear(); }

bool        QueueList::isEmpty() const { return frente_ == nullptr; }
bool        QueueList::isFull()  const { return false; }  // sin capacidad fija
std::size_t QueueList::size()    const { return n_; }

void QueueList::clear() {
    Nodo* actual = frente_;
    while (actual != nullptr) {
        Nodo* sig = actual->sig;
        delete actual;
        actual = sig;
    }
    frente_ = nullptr;
    final_  = nullptr;
    n_ = 0;
}

// ---- Núcleo evaluable -----------------------------------------------------

bool QueueList::enqueue(const Packet& p) {
    // TODO
    //   Crear nodo. Si la cola está vacía, frente_ y final_ apuntan al nuevo.
    //   Si no: final_->sig = nuevo;  final_ = nuevo;
    //   ++n_; ++nodosCreados_; return true.
    //
    //   CASO LÍMITE CLÁSICO: insertar en cola vacía. Si olvidan actualizar
    //   frente_, la cola queda corrupta y no lo notarán hasta el primer
    //   dequeue. Cúbranlo con un test dedicado.
    (void)p;
    return false;
}

bool QueueList::dequeue(Packet& out) {
    // TODO
    //   Si isEmpty() -> false.
    //   Si no: out = frente_->dato; avanzar frente_; delete del nodo viejo;
    //          --n_;
    //   CASO LÍMITE: si la cola queda vacía, final_ debe volver a nullptr,
    //   o quedará colgando y el próximo enqueue escribirá sobre memoria
    //   liberada. Este es el bug más frecuente de esta estructura.
    (void)out;
    return false;
}

bool QueueList::front(Packet& out) const {
    // TODO
    (void)out;
    return false;
}
