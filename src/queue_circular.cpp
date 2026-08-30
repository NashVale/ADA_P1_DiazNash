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

// ---- Núcleo evaluable -----------------------------------------------------

bool QueueCircular::isEmpty() const {
    // TODO — depende de la estrategia elegida (ver el .hpp).
    //   Opción (b): return n_ == 0;
    return true;
}

bool QueueCircular::isFull() const {
    // TODO — depende de la estrategia elegida.
    //   Opción (b): return n_ == cap_;
    //   Opción (a): return (tail_ + 1) % cap_ == head_;
    //
    //  Justifiquen en la tabla 8.2 por qué esto es Theta(1): es una
    //  comparación aritmética, sin recorrido.
    return false;
}

bool QueueCircular::enqueue(const Packet& p) {
    // TODO
    //   Si isFull() -> return false   (paquete DESCARTADO por búfer lleno,
    //                                  comportamiento real de saturación)
    //   Si no:
    //     datos_[tail_] = p;
    //     tail_ = (tail_ + 1) % cap_;      <-- aquí está la circularidad
    //     ++n_;
    //     if (n_ > ocupMax_) ocupMax_ = n_;
    //     return true;
    //
    //  NOTA de rendimiento para la Sección 10: el operador % es una división
    //  entera. Si cap_ es potencia de 2 puede sustituirse por una máscara
    //  (& (cap_-1)), o por un if. Midan la diferencia: es otra "constante
    //  oculta" que no cambia el O(1) pero sí el tiempo real.
    (void)p;
    return false;
}

bool QueueCircular::dequeue(Packet& out) {
    // TODO
    //   Si isEmpty() -> return false   (caso límite obligatorio: dequeue
    //                                   sobre cola vacía, debe reportarse)
    //   Si no: out = datos_[head_]; head_ = (head_ + 1) % cap_; --n_; true
    (void)out;
    return false;
}

bool QueueCircular::front(Packet& out) const {
    // TODO
    (void)out;
    return false;
}
