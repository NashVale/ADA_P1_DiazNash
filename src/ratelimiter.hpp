#ifndef RATELIMITER_HPP
#define RATELIMITER_HPP

#include "tad.hpp"
#include <string>
#include <iosfwd>

// ===========================================================================
//  PROBLEMA 2 — Búfer de recepción + limitador de tasa por ventana deslizante.
//
//  Parámetros (entrada, NO deben quedar codificados fijos — Sección 5):
//     C = capacidad del búfer de recepción
//     T = amplitud de la ventana deslizante, en milisegundos
//     L = número máximo de paquetes admitidos dentro de una ventana
//
//  Algoritmo por paquete p que llega en t = p.ts_ms:
//     1. Purgar de la cola de ventana toda marca anterior a t - T.
//     2. Si el tamaño resultante de la ventana es >= L  -> RECHAZO_TASA.
//     3. Si el búfer está lleno                          -> RECHAZO_BUFFER.
//     4. En otro caso: encolar en el búfer, registrar t en la ventana,
//        ACEPTADO.
//
//  DECISIÓN DE DISEÑO OBLIGATORIA (Sección 5, casos límite):
//  ¿el borde t = t0 + T se incluye o se excluye? Es decir, ¿la condición de
//  purga es  marca < t - T  o  marca <= t - T ? Ambas son defendibles; deben
//  elegir una, justificarla y cubrirla con el caso de prueba 7.
//
//  SEGUNDA DECISIÓN: ¿se comprueba primero la tasa o primero el búfer? El
//  orden cambia las estadísticas de salida (un paquete puede violar ambas
//  condiciones a la vez y solo se cuenta en una). Fíjenlo y documéntenlo.
// ===========================================================================

enum class Veredicto { ACEPTADO, RECHAZO_BUFFER_LLENO, RECHAZO_TASA };

const char* nombreVeredicto(Veredicto v);

struct EstadisticasFirewall {
    std::size_t total              = 0;
    std::size_t aceptados          = 0;
    std::size_t rechazadosBuffer   = 0;
    std::size_t rechazadosTasa     = 0;
    std::size_t ocupacionMaxima    = 0;
    std::size_t marcasPurgadasTot  = 0;  // suma de purgas: clave para el
                                         // argumento amortizado de la Sec. 8.3
    std::size_t purgaMaximaPuntual = 0;  // peor caso puntual observado
};

class RateLimiter {
public:
    // No toma posesión de las colas.
    RateLimiter(IQueue* buffer, IQueue* ventana, long long T_ms, std::size_t L);

    Veredicto procesar(const Packet& p);

    // Extrae un paquete del búfer (el firewall lo "consume"). false si vacío.
    bool consumir(Packet& out);

    const EstadisticasFirewall& estadisticas() const { return stats_; }
    std::size_t tamBuffer()  const;
    std::size_t tamVentana() const;

    // Lee <ts_ms> <size_bytes> por línea y escribe el reporte por paquete.
    std::size_t procesarArchivo(const std::string& ruta, std::ostream& log);

private:
    // -----------------------------------------------------------------------
    //  TODO (núcleo evaluable — este método ES el análisis amortizado que la
    //  Sección 8.3 declara OBLIGATORIO para el Problema 2)
    //
    //  Elimina del frente de ventana_ todas las marcas que ya salieron de la
    //  ventana [t - T, t]. Devuelve cuántas eliminó.
    //
    //    while (!ventana_->isEmpty()) {
    //        mirar el frente sin extraerlo (front)
    //        si su ts_ms sigue dentro de la ventana -> break
    //        si no -> dequeue y contar
    //    }
    //
    //  EL ARGUMENTO A ESCRIBIR EN EL INFORME:
    //  una llamada puede purgar hasta O(k) marcas, luego el PEOR CASO PUNTUAL
    //  es O(n). Pero cada marca se encola exactamente una vez y se desencola
    //  a lo sumo una vez en toda la ejecución, así que el total de purgas a lo
    //  largo de n paquetes está acotado por n. El costo AMORTIZADO por paquete
    //  es por tanto O(1). Los campos marcasPurgadasTot y purgaMaximaPuntual
    //  existen justamente para que puedan EVIDENCIARLO con datos medidos:
    //  comprueben que marcasPurgadasTot <= n mientras purgaMaximaPuntual >> 1.
    // -----------------------------------------------------------------------
    std::size_t purgarExpirados(long long t_actual);

    IQueue*              buffer_;
    IQueue*              ventana_;
    long long            T_ms_;
    std::size_t          L_;
    EstadisticasFirewall stats_;
};

#endif // RATELIMITER_HPP
