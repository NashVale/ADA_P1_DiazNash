#ifndef STACK_ARRAY_HPP
#define STACK_ARRAY_HPP

#include "tad.hpp"

// ===========================================================================
//  Pila sobre arreglo dinámico gestionado manualmente.
//
//  Representación:  un bloque crudo  EditOp* datos_  de tamaño cap_,
//                   con n_ elementos válidos en las posiciones [0, n_).
//                   El TOPE es datos_[n_ - 1].
//
//  Invariantes:     0 <= n_ <= cap_        y      cap_ >= 1
//                   isEmpty()  <=>  n_ == 0
//
//  Prohibido usar std::vector como contenedor de la pila (Sección 7).
//  Aquí se usa new EditOp[] / delete[] y se gestiona la capacidad a mano.
// ===========================================================================
class StackArray : public IStack {
public:
    explicit StackArray(std::size_t capacidadInicial = 8);
    ~StackArray() override;

    // Se prohíbe copiar: la clase posee memoria cruda y una copia superficial
    // provocaría doble liberación. Si necesitan copiar, implementen la regla
    // de los tres/cinco y justifíquenlo.
    StackArray(const StackArray&)            = delete;
    StackArray& operator=(const StackArray&) = delete;

    void        push(const EditOp& op) override;
    bool        pop(EditOp& out)       override;
    bool        top(EditOp& out) const override;
    bool        isEmpty()        const override;
    std::size_t size()           const override;
    void        clear()                override;

    const char* nombreRepresentacion() const override {
        return "Pila / arreglo dinamico";
    }

    // --- Instrumentación para la Sección 9 (experimentación) ---------------
    // Sirven para evidenciar el comportamiento amortizado: cuántas veces se
    // redimensionó y cuántas copias de elementos costó en total.
    std::size_t capacidad()             const { return cap_; }
    std::size_t numRedimensionamientos() const { return resizes_; }
    std::size_t copiasAcumuladas()       const { return copias_; }

private:
    // ---------------------------------------------------------------------
    //  TODO (núcleo evaluable — análisis amortizado, Secciones 8.1 y 8.3)
    //
    //  Reserva un bloque nuevo de mayor capacidad, traslada los n_ elementos
    //  y libera el bloque anterior.
    //
    //  Deben ELEGIR y JUSTIFICAR la política de crecimiento:
    //    - cap_ * 2      -> O(1) amortizado, desperdicio de hasta el 50%
    //    - cap_ * 3/2    -> menos desperdicio, más redimensionamientos
    //    - cap_ + k      -> ¡ojo! esto NO es O(1) amortizado, es O(n) por push
    //                       en promedio. Es el contraejemplo perfecto para el
    //                       informe: pruébenlo y midan la diferencia.
    //
    //  Al implementarlo, incrementen resizes_ y sumen n_ a copias_ para poder
    //  graficar el costo amortizado después.
    // ---------------------------------------------------------------------
    void grow();

    EditOp*     datos_;
    std::size_t cap_;
    std::size_t n_;
    std::size_t resizes_;
    std::size_t copias_;
};

#endif // STACK_ARRAY_HPP
