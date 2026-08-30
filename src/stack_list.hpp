#ifndef STACK_LIST_HPP
#define STACK_LIST_HPP

#include "tad.hpp"

// ===========================================================================
//  Pila sobre lista enlazada simple.
//
//  Representación:  cabeza_ apunta al TOPE. Cada push crea un nodo y lo
//                   inserta en la cabeza; cada pop elimina la cabeza.
//                   No existe "pila llena": la capacidad la limita el sistema.
//
//  Invariantes:     isEmpty()  <=>  cabeza_ == nullptr
//                   n_ == número de nodos alcanzables desde cabeza_
//
//  Contraste con StackArray (obligatorio en la Sección 6):
//    - No hay redimensionamiento, luego push es Theta(1) en el PEOR caso,
//      no solo amortizado. A cambio, cada elemento paga el overhead de un
//      puntero y una llamada a new/delete por operación.
//    - Los nodos quedan dispersos en el heap: peor localidad de caché. Esa es
//      la razón por la que el arreglo suele ganar en la práctica aunque ambos
//      sean O(1). Es exactamente lo que pide discutir la Sección 10.
// ===========================================================================
class StackList : public IStack {
public:
    StackList();
    ~StackList() override;

    StackList(const StackList&)            = delete;
    StackList& operator=(const StackList&) = delete;

    void        push(const EditOp& op) override;
    bool        pop(EditOp& out)       override;
    bool        top(EditOp& out) const override;
    bool        isEmpty()        const override;
    std::size_t size()           const override;
    void        clear()                override;

    const char* nombreRepresentacion() const override {
        return "Pila / lista enlazada";
    }

    // Instrumentación: número de new ejecutados, para comparar contra las
    // copias del arreglo en la Sección 10.
    std::size_t nodosCreados() const { return nodosCreados_; }

private:
    struct Nodo {
        EditOp dato;
        Nodo*  sig;
        explicit Nodo(const EditOp& d, Nodo* s = nullptr) : dato(d), sig(s) {}
    };

    Nodo*       cabeza_;
    std::size_t n_;
    std::size_t nodosCreados_;
};

#endif // STACK_LIST_HPP
