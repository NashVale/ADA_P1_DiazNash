#include "ratelimiter.hpp"

#include <fstream>
#include <sstream>
#include <ostream>

const char* nombreVeredicto(Veredicto v) {
    switch (v) {
        case Veredicto::ACEPTADO:             return "ACEPTADO";
        case Veredicto::RECHAZO_BUFFER_LLENO: return "RECHAZADO_BUFFER_LLENO";
        case Veredicto::RECHAZO_TASA:         return "RECHAZADO_LIMITE_TASA";
    }
    return "DESCONOCIDO";
}

RateLimiter::RateLimiter(IQueue* buffer, IQueue* ventana,
                         long long T_ms, std::size_t L)
    : buffer_(buffer), ventana_(ventana), T_ms_(T_ms), L_(L), stats_() {}

std::size_t RateLimiter::tamBuffer()  const { return buffer_->size();  }
std::size_t RateLimiter::tamVentana() const { return ventana_->size(); }

bool RateLimiter::consumir(Packet& out) {
    return buffer_->dequeue(out);
}

std::size_t RateLimiter::purgarExpirados(long long t_actual) {
    std::size_t purgadas = 0;
    Packet f;

    while (ventana_->front(f)) {
        if (f.ts_ms > t_actual - T_ms_) break;

        Packet tmp;
        ventana_->dequeue(tmp);
        ++purgadas;
    }
    return purgadas;
}

Veredicto RateLimiter::procesar(const Packet& p) {
    ++stats_.total;

    std::size_t purgadas = purgarExpirados(p.ts_ms);
    stats_.marcasPurgadasTot += purgadas;
    if (purgadas > stats_.purgaMaximaPuntual) stats_.purgaMaximaPuntual = purgadas;

    if (ventana_->size() >= L_) {
        ++stats_.rechazadosTasa;
        return Veredicto::RECHAZO_TASA;
    }

    if (buffer_->isFull()) {
        ++stats_.rechazadosBuffer;
        return Veredicto::RECHAZO_BUFFER_LLENO;
    }

    buffer_->enqueue(p);
    ventana_->enqueue(p);

    ++stats_.aceptados;
    if (buffer_->size() > stats_.ocupacionMaxima)
        stats_.ocupacionMaxima = buffer_->size();

    return Veredicto::ACEPTADO;
}

std::size_t RateLimiter::procesarArchivo(const std::string& ruta,
                                         std::ostream& log)
{
    std::ifstream in(ruta);
    if (!in) {
        log << "ERROR: no se pudo abrir el archivo " << ruta << "\n";
        return 0;
    }

    std::string linea;
    std::size_t n = 0;

    while (std::getline(in, linea)) {
        if (linea.empty() || linea[0] == '#') continue;

        std::istringstream ss(linea);
        Packet p;
        if (!(ss >> p.ts_ms >> p.size_bytes)) {
            log << "ERROR: linea mal formada: " << linea << "\n";
            continue;
        }
        ++n;
        Veredicto v = procesar(p);
        log << n << "\t" << p.ts_ms << "ms\t" << p.size_bytes << "B\t"
            << nombreVeredicto(v)
            << "\tbuf=" << buffer_->size()
            << "\tven=" << ventana_->size() << "\n";
    }
    return n;
}
