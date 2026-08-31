#include "generator.hpp"

#include <fstream>
#include <random>
#include <string>

namespace {

std::string palabraAleatoria(std::mt19937& rng, int minLen, int maxLen) {
    static const char alfabeto[] = "abcdefghijklmnopqrstuvwxyz";
    std::uniform_int_distribution<int> distLen(minLen, maxLen);
    std::uniform_int_distribution<int> distChar(0, 25);
    int len = distLen(rng);
    std::string s;
    s.reserve(static_cast<std::size_t>(len));
    for (int i = 0; i < len; ++i) s += alfabeto[distChar(rng)];
    return s;
}

}

void generarEventosEditor(const std::string& rutaSalida,
                          std::size_t n,
                          unsigned semilla,
                          int pctUndo,
                          int pctRedo)
{
    std::ofstream out(rutaSalida);
    if (!out) return;

    std::mt19937 rng(semilla);
    std::uniform_int_distribution<int> dist100(1, 100);
    std::uniform_int_distribution<int> distTipo(0, 2);

    out << "# Instancia sintetica Problema 1 (Undo/Redo)\n";
    out << "# n=" << n << " semilla=" << semilla
        << " pctUndo=" << pctUndo << " pctRedo=" << pctRedo << "\n";
    out << "# Formato: EDIT <tipo> <pos> <contenido> <texto_previo>\n";

    std::size_t longEstimada = 0;

    for (std::size_t i = 0; i < n; ++i) {
        int r = dist100(rng);
        if (r <= pctUndo) {
            out << "UNDO\n";
        } else if (r <= pctUndo + pctRedo) {
            out << "REDO\n";
        } else {
            std::uniform_int_distribution<std::size_t> distPos(0, longEstimada);
            std::size_t pos = distPos(rng);
            int tipo = distTipo(rng);

            if (tipo == 0) {
                std::string txt = palabraAleatoria(rng, 1, 8);
                out << "EDIT INSERT " << pos << " " << txt << " -\n";
                longEstimada += txt.size();
            } else if (tipo == 1) {
                if (longEstimada == 0) { out << "EDIT INSERT 0 hola -\n"; longEstimada += 4; continue; }
                std::size_t maxBorrar = longEstimada - pos;
                if (maxBorrar == 0) { out << "EDIT INSERT " << pos << " x -\n"; ++longEstimada; continue; }
                std::string prev = palabraAleatoria(rng, 1,
                    static_cast<int>(maxBorrar > 6 ? 6 : maxBorrar));
                out << "EDIT DELETE " << pos << " - " << prev << "\n";
                longEstimada -= prev.size();
            } else {
                if (longEstimada == 0) { out << "EDIT INSERT 0 hola -\n"; longEstimada += 4; continue; }
                std::size_t maxRepl = longEstimada - pos;
                if (maxRepl == 0) { out << "EDIT INSERT " << pos << " y -\n"; ++longEstimada; continue; }
                std::string prev  = palabraAleatoria(rng, 1,
                    static_cast<int>(maxRepl > 5 ? 5 : maxRepl));
                std::string nuevo = palabraAleatoria(rng, 1, 5);
                out << "EDIT REPLACE " << pos << " " << nuevo << " " << prev << "\n";
                longEstimada = longEstimada - prev.size() + nuevo.size();
            }
        }
    }
}

void generarPaquetes(const std::string& rutaSalida,
                     std::size_t n,
                     unsigned semilla,
                     int gapMedioMs)
{
    std::ofstream out(rutaSalida);
    if (!out) return;

    std::mt19937 rng(semilla);
    std::uniform_int_distribution<int> distGap(0, gapMedioMs * 2);
    std::uniform_int_distribution<int> distTam(40, 1500);
    std::uniform_int_distribution<int> distRafaga(1, 100);

    out << "# Instancia sintetica Problema 2 (firewall)\n";
    out << "# n=" << n << " semilla=" << semilla
        << " gapMedioMs=" << gapMedioMs << "\n";
    out << "# Formato: <timestamp_ms> <tamano_bytes>\n";

    long long t = 0;
    std::size_t restanteRafaga = 0;

    for (std::size_t i = 0; i < n; ++i) {
        if (restanteRafaga == 0 && distRafaga(rng) <= 3) {
            std::uniform_int_distribution<int> distLenRafaga(20, 80);
            restanteRafaga = static_cast<std::size_t>(distLenRafaga(rng));
        }

        if (restanteRafaga > 0) {
            t += (distGap(rng) % 2);
            --restanteRafaga;
        } else {
            t += distGap(rng);
        }
        out << t << " " << distTam(rng) << "\n";
    }
}
