#include "stack_list.hpp"

StackList::StackList() : cabeza_(nullptr), n_(0), nodosCreados_(0) {}

StackList::~StackList() { clear(); }

bool        StackList::isEmpty() const { return cabeza_ == nullptr; }
std::size_t StackList::size()    const { return n_; }

void StackList::clear() {
    Nodo* actual = cabeza_;
    while (actual != nullptr) {
        Nodo* sig = actual->sig;
        delete actual;
        actual = sig;
    }
    cabeza_ = nullptr;
    n_ = 0;
}

void StackList::push(const EditOp& op) {

    cabeza_ = new Nodo(op, cabeza_);
    ++n_;
    ++nodosCreados_;
}

bool StackList::pop(EditOp& out) {
    if (isEmpty()) return false;

    Nodo* viejo = cabeza_;
    out = viejo->dato;
    cabeza_ = viejo->sig;
    delete viejo;
    --n_;
    return true;
}

bool StackList::top(EditOp& out) const {
    if (isEmpty()) return false;

    out = cabeza_->dato;
    return true;
}
