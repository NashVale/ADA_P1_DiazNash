#include "queue_list.hpp"

QueueList::QueueList()
    : frente_(nullptr), final_(nullptr), n_(0), nodosCreados_(0) {}

QueueList::~QueueList() { clear(); }

bool        QueueList::isEmpty() const { return frente_ == nullptr; }
bool        QueueList::isFull()  const { return false; }
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

bool QueueList::enqueue(const Packet& p) {
    Nodo* nuevo = new Nodo(p);

    if (isEmpty()) frente_ = nuevo;
    else           final_->sig = nuevo;

    final_ = nuevo;
    ++n_;
    ++nodosCreados_;
    return true;
}

bool QueueList::dequeue(Packet& out) {
    if (isEmpty()) return false;

    Nodo* viejo = frente_;
    out = viejo->dato;
    frente_ = viejo->sig;
    delete viejo;
    --n_;

    if (frente_ == nullptr) final_ = nullptr;
    return true;
}

bool QueueList::front(Packet& out) const {
    if (isEmpty()) return false;

    out = frente_->dato;
    return true;
}
