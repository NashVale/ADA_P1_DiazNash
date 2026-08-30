#ifndef QUEUE_CIRCULAR_HPP
#define QUEUE_CIRCULAR_HPP

#include "tad.hpp"

// ===========================================================================
//  Cola circular sobre arreglo de capacidad FIJA C (búfer de recepción).
//
//  Representación:  bloque crudo Packet* datos_ de tamaño cap_.
//                   head_ = índice del frente (próximo a salir)
//                   tail_ = índice donde se escribirá el próximo elemento
//                   Los índices avanzan módulo cap_ y "dan la vuelta".
//
//  EL PROBLEMA CLÁSICO (la Sección 6 lo exige explícitamente):
//  con solo head_ y tail_, la condición  head_ == tail_  es AMBIGUA: ocurre
//  tanto con la cola vacía como con la cola llena. Hay tres soluciones
//  estándar; deben elegir UNA y justificarla en el informe:
//
//    (a) Sacrificar una celda: se considera llena cuando
//        (tail_ + 1) % cap_ == head_. Capacidad útil = cap_ - 1.
//        Ventaja: sin estado extra. Desventaja: hay que reservar C+1 celdas
//        para admitir C paquetes, o documentar que la capacidad real es C-1.
//
//    (b) Contador explícito n_ de elementos. Vacía <=> n_ == 0,
//        llena <=> n_ == cap_. Ventaja: capacidad útil completa y size() en
//        O(1) directo. Desventaja: un campo más que mantener coherente.
//
//    (c) Bandera booleana "llena", actualizada en cada enqueue/dequeue.
//
//  Este esqueleto declara n_ pensando en la opción (b), que es la más simple
//  de sustentar oralmente, pero pueden cambiarla si justifican por qué.
// ===========================================================================
class QueueCircular : public IQueue {
public:
    explicit QueueCircular(std::size_t capacidad);
    ~QueueCircular() override;

    QueueCircular(const QueueCircular&)            = delete;
    QueueCircular& operator=(const QueueCircular&) = delete;

    bool        enqueue(const Packet& p)   override;
    bool        dequeue(Packet& out)       override;
    bool        front(Packet& out)   const override;
    bool        isEmpty()            const override;
    bool        isFull()             const override;
    std::size_t size()               const override;

    const char* nombreRepresentacion() const override {
        return "Cola circular / arreglo fijo";
    }

    std::size_t capacidad()      const { return cap_; }
    // Métrica exigida por la Sección 5: ocupación máxima alcanzada.
    std::size_t ocupacionMaxima() const { return ocupMax_; }

private:
    Packet*     datos_;
    std::size_t cap_;
    std::size_t head_;
    std::size_t tail_;
    std::size_t n_;
    std::size_t ocupMax_;
};

#endif // QUEUE_CIRCULAR_HPP
