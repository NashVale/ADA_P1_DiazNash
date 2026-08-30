#include "queue_circular.hpp"

QueueCircular::QueueCircular(std::size_t capacidad)
    : datos_(nullptr), cap_(capacidad ? capacidad : 1),
      head_(0), tail_(0), n_(0), ocupMax_(0)
{
    datos_ = new Packet[cap_];
}

QueueCircular::~QueueCircular() {
    delete[] datos_;
    datos_ = nullptr;
}

std::size_t QueueCircular::size() const { return n_; }

bool QueueCircular::isEmpty() const {
    return n_ == 0;
}

bool QueueCircular::isFull() const {
    return n_ == cap_;
}

bool QueueCircular::enqueue(const Packet& p) {
    if (isFull()) return false;

    datos_[tail_] = p;
    tail_ = (tail_ + 1) % cap_;
    ++n_;

    if (n_ > ocupMax_) ocupMax_ = n_;
    return true;
}

bool QueueCircular::dequeue(Packet& out) {
    if (isEmpty()) return false;

    out = datos_[head_];
    head_ = (head_ + 1) % cap_;
    --n_;
    return true;
}

bool QueueCircular::front(Packet& out) const {
    if (isEmpty()) return false;

    out = datos_[head_];
    return true;
}
