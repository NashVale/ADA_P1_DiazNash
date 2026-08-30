#ifndef QUEUE_LIST_HPP
#define QUEUE_LIST_HPP

#include "tad.hpp"

// ===========================================================================
//  Cola sobre lista enlazada simple, SIN capacidad fija.
//
//  Representación:  frente_ = primer nodo (próximo a salir)
//                   final_  = último nodo (donde se inserta)
//                   Mantener AMBOS punteros es lo que hace que enqueue sea
//                   Theta(1); con solo frente_ habría que recorrer toda la
//                   lista y enqueue sería Theta(n). Es un punto excelente
//                   para la tabla 8.2 y para la sustentación oral.
//
//  Uso previsto en esta práctica: la COLA DE MARCAS DE TIEMPO del limitador
//  de tasa. Se elige lista enlazada porque su tamaño no está acotado a
//  priori (depende de cuántos paquetes caigan dentro de la ventana T), y
//  porque así cumplimos el requisito de la Sección 7 de tener una segunda
//  representación de Cola con comparación explícita de costos.
//
//  ALTERNATIVA A DISCUTIR EN EL INFORME: la ventana también podría vivir en
//  una cola circular de capacidad L (nunca hacen falta más de L marcas para
//  decidir). Comparen: ¿qué gana y qué pierde cada opción?
//
//  Nota: se reutiliza Packet aunque la ventana solo necesita el timestamp.
//  Eso simplifica el TAD, pero desperdicia memoria. Si prefieren una cola de
//  long long, tendrán que parametrizar por tipo (extensión bonus con
//  template<typename T>). Justifiquen la decisión que tomen.
// ===========================================================================
class QueueList : public IQueue {
public:
    QueueList();
    ~QueueList() override;

    QueueList(const QueueList&)            = delete;
    QueueList& operator=(const QueueList&) = delete;

    bool        enqueue(const Packet& p)   override;
    bool        dequeue(Packet& out)       override;
    bool        front(Packet& out)   const override;
    bool        isEmpty()            const override;
    bool        isFull()             const override;  // siempre false
    std::size_t size()               const override;

    const char* nombreRepresentacion() const override {
        return "Cola / lista enlazada";
    }

    void clear();
    std::size_t nodosCreados() const { return nodosCreados_; }

private:
    struct Nodo {
        Packet dato;
        Nodo*  sig;
        explicit Nodo(const Packet& d, Nodo* s = nullptr) : dato(d), sig(s) {}
    };

    Nodo*       frente_;
    Nodo*       final_;
    std::size_t n_;
    std::size_t nodosCreados_;
};

#endif // QUEUE_LIST_HPP
