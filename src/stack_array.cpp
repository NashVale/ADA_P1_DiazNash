#include "stack_array.hpp"

// ---- Gestión de memoria (ya resuelto) -------------------------------------

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

// ---- Consultas triviales (ya resueltas) -----------------------------------

bool        StackArray::isEmpty() const { return n_ == 0; }
std::size_t StackArray::size()    const { return n_; }

void StackArray::clear() {
    // Se libera el contenido pero se conserva la capacidad ya reservada.
    // DISCUTIR EN EL INFORME: clear() es O(n) porque cada EditOp contiene
    // std::string y hay que destruir n objetos; NO es O(1) como en una pila
    // de tipos triviales. Esto importa para la regla "un EDIT invalida el
    // Redo", que se ejecuta potencialmente en cada edición.
    for (std::size_t i = 0; i < n_; ++i) datos_[i] = EditOp{};
    n_ = 0;
}

// ---- Núcleo evaluable -----------------------------------------------------

void StackArray::push(const EditOp& op) {
    // TODO
    //   1. Si n_ == cap_  ->  grow()
    //   2. datos_[n_] = op;  ++n_;
    //
    //  Complejidad a justificar en la tabla 8.1:
    //    - sin redimensionar: Theta(1)   (escritura + incremento de índice)
    //    - con redimensionamiento: Theta(n) puntual
    //    - amortizado sobre n push consecutivos con duplicación: O(1)
    //      (la serie 1 + 2 + 4 + ... + n < 2n copias totales)
    (void)op;
}

bool StackArray::pop(EditOp& out) {
    // TODO
    //   Si isEmpty() -> return false   (NO-OP válido, ver Sección 4)
    //   Si no        -> --n_; out = datos_[n_]; limpiar datos_[n_]; return true
    //
    //  NOTA: si no "limpian" la celda liberada, la std::string sigue
    //  reteniendo memoria. No afecta la complejidad asintótica pero sí el
    //  consumo medido; vale la pena mencionarlo.
    (void)out;
    return false;
}

bool StackArray::top(EditOp& out) const {
    // TODO
    //   Si isEmpty() -> false; si no -> out = datos_[n_ - 1]; true
    (void)out;
    return false;
}

void StackArray::grow() {
    // TODO — ver la explicación en stack_array.hpp
    //   1. std::size_t nuevaCap = <política elegida>;
    //   2. EditOp* nuevo = new EditOp[nuevaCap];
    //   3. copiar los n_ elementos (o mover con std::move para no duplicar
    //      las cadenas: eso reduce la constante, no el orden asintótico)
    //   4. delete[] datos_;  datos_ = nuevo;  cap_ = nuevaCap;
    //   5. ++resizes_;  copias_ += n_;
}
