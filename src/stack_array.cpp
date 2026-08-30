#include "stack_array.hpp"

#include <utility>

StackArray::StackArray(std::size_t capacidadInicial)
    : datos_(nullptr), cap_(capacidadInicial ? capacidadInicial : 1),
      n_(0), resizes_(0), copias_(0)
{
    datos_ = new EditOp[cap_];
}

StackArray::~StackArray() {
    delete[] datos_;
    datos_ = nullptr;
}

bool        StackArray::isEmpty() const { return n_ == 0; }
std::size_t StackArray::size()    const { return n_; }

void StackArray::clear() {
    for (std::size_t i = 0; i < n_; ++i) datos_[i] = EditOp{};
    n_ = 0;
}

void StackArray::push(const EditOp& op) {
    if (n_ == cap_) grow();
    datos_[n_] = op;
    ++n_;
}

bool StackArray::pop(EditOp& out) {
    if (isEmpty()) return false;

    --n_;
    out = datos_[n_];
    datos_[n_] = EditOp{};
    return true;
}

bool StackArray::top(EditOp& out) const {
    if (isEmpty()) return false;

    out = datos_[n_ - 1];
    return true;
}

void StackArray::grow() {

    std::size_t nuevaCap = cap_ * 2;

    EditOp* nuevo = new EditOp[nuevaCap];

    for (std::size_t i = 0; i < n_; ++i)
        nuevo[i] = std::move(datos_[i]);

    delete[] datos_;
    datos_ = nuevo;
    cap_   = nuevaCap;

    ++resizes_;
    copias_ += n_;
}
