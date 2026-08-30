#ifndef TAD_HPP
#define TAD_HPP

#include <cstddef>
#include <string>

// ===========================================================================
//  TAD — Tipos Abstractos de Datos (Sección 6 del enunciado)
//
//  Este archivo declara ÚNICAMENTE el CONTRATO de Pila y Cola: qué operaciones
//  existen y qué promete cada una. NO dice nada sobre la representación interna.
//
//  Las clases concretas (StackArray/StackList, QueueCircular/QueueList)
//  implementan este mismo contrato con representaciones distintas. Eso es lo
//  que hay que mostrar explícitamente en el informe: qué cambia (el costo, el
//  uso de memoria, la localidad) y qué permanece igual (la interfaz, la
//  semántica de cada operación, los casos límite).
//
//  DECISIÓN DE DISEÑO A JUSTIFICAR EN EL INFORME:
//  Las operaciones que pueden fallar (pop sobre pila vacía, enqueue sobre cola
//  llena) NO lanzan excepción ni abortan: devuelven bool y escriben el
//  resultado en un parámetro de salida. Esto cumple el requisito de la Sección
//  7 ("mensajes o códigos de retorno claros, no se aceptan fallos silenciosos
//  ni caídas del programa"). La alternativa (excepciones) también es válida
//  pero cambia el costo constante de la operación; discútanlo.
//
//  NOTA SOBRE EL DESPACHO VIRTUAL:
//  Usar herencia con métodos virtuales es la forma más directa de demostrar
//  "una interfaz, dos implementaciones". Tiene un costo: cada llamada pasa por
//  la vtable y el compilador no puede hacer inline. En la Sección 10 del
//  informe pueden (y conviene) medir el mismo experimento con llamada virtual
//  y con la clase concreta directa, y discutir la diferencia. Es un excelente
//  ejemplo de "constantes ocultas por la notación asintótica": ambos son O(1),
//  pero no cuestan lo mismo.
// ===========================================================================


// ---------------------------------------------------------------------------
//  Tipos de elemento
// ---------------------------------------------------------------------------

enum class EditType { INSERT, DELETE, REPLACE };

// Operación de edición registrada en el historial (Problema 1).
//
// OJO — decisión de diseño que deben justificar: para poder DESHACER un DELETE
// o un REPLACE hace falta saber qué texto había ANTES. Por eso el registro
// guarda tanto el texto nuevo como el texto previo. Si solo guardaran el texto
// nuevo, el undo sería imposible sin re-simular el documento desde el inicio.
// Esto tiene consecuencias en la complejidad ESPACIAL: cada entrada de la pila
// ocupa O(|content| + |previous|), no O(1). Menciónenlo en la tabla 8.1.
struct EditOp {
    EditType    type = EditType::INSERT;
    std::size_t pos  = 0;   // posición del documento donde se aplica
    std::string content;    // texto insertado / texto nuevo del reemplazo
    std::string previous;   // texto que ocupaba esa posición antes de la operación
};

// Paquete de red sintético (Problema 2).
struct Packet {
    long long ts_ms      = 0;  // marca de tiempo de llegada, en milisegundos
    int       size_bytes = 0;
};


// ---------------------------------------------------------------------------
//  TAD Pila (LIFO)
// ---------------------------------------------------------------------------
//
//  Invariante del TAD: si la secuencia de push fue e1, e2, ..., ek (sin pops
//  intermedios), los pop sucesivos devuelven ek, ek-1, ..., e1.
//
class IStack {
public:
    virtual ~IStack() = default;

    // Inserta un elemento en el tope.
    // Post: size() aumenta en 1; top() devuelve el elemento insertado.
    virtual void push(const EditOp& op) = 0;

    // Extrae el elemento del tope y lo copia en 'out'.
    // Devuelve false si la pila estaba vacía (NO-OP válido, no es error fatal).
    virtual bool pop(EditOp& out) = 0;

    // Consulta el tope sin extraerlo. false si está vacía.
    virtual bool top(EditOp& out) const = 0;

    virtual bool        isEmpty() const = 0;
    virtual std::size_t size()    const = 0;

    // Vacía la pila. Necesario para la regla "un EDIT nuevo invalida el Redo".
    virtual void clear() = 0;

    virtual const char* nombreRepresentacion() const = 0;
};


// ---------------------------------------------------------------------------
//  TAD Cola (FIFO)
// ---------------------------------------------------------------------------
//
//  Invariante del TAD: si la secuencia de enqueue aceptados fue p1, ..., pk,
//  los dequeue sucesivos devuelven p1, ..., pk (mismo orden de llegada).
//
class IQueue {
public:
    virtual ~IQueue() = default;

    // Inserta por el final. Devuelve false si la cola está llena (descarte).
    // En representaciones sin capacidad fija, isFull() es siempre false.
    virtual bool enqueue(const Packet& p) = 0;

    // Extrae por el frente. Devuelve false si la cola está vacía.
    virtual bool dequeue(Packet& out) = 0;

    // Consulta el frente sin extraerlo. false si está vacía.
    virtual bool front(Packet& out) const = 0;

    virtual bool        isEmpty() const = 0;
    virtual bool        isFull()  const = 0;
    virtual std::size_t size()    const = 0;

    virtual const char* nombreRepresentacion() const = 0;
};

#endif // TAD_HPP
