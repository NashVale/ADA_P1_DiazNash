#include "tad.hpp"
#include "stack_array.hpp"
#include "stack_list.hpp"
#include "queue_circular.hpp"
#include "queue_list.hpp"
#include "undoredo.hpp"
#include "ratelimiter.hpp"
#include "generator.hpp"

#include <chrono>
#include <streambuf>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

using Reloj = std::chrono::high_resolution_clock;

namespace {

class BufNulo : public std::streambuf {
protected:
    int overflow(int c) override { return c; }
};

double msDesde(Reloj::time_point t0) {
    auto t1 = Reloj::now();
    return std::chrono::duration<double, std::milli>(t1 - t0).count();
}

void mediaYDesv(const double* v, int k, double& media, double& desv) {
    media = 0.0;
    for (int i = 0; i < k; ++i) media += v[i];
    media /= k;
    double acc = 0.0;
    for (int i = 0; i < k; ++i) { double d = v[i] - media; acc += d * d; }
    desv = (k > 1) ? std::sqrt(acc / (k - 1)) : 0.0;
}

void uso() {
    std::cout <<
        "Uso:\n"
        "  ada_p1 gen1  <n> <semilla> <salida>\n"
        "  ada_p1 gen2  <n> <semilla> <salida>\n"
        "  ada_p1 p1    <entrada> <array|lista> [archivo_log]\n"
        "  ada_p1 p2    <entrada> <C> <T_ms> <L> [archivo_log]\n"
        "  ada_p1 bench <n> <semilla> <repeticiones>\n";
}

int ejecutarP1(const std::string& entrada, const std::string& rep,
               const std::string& rutaLog)
{
    IStack* undo = nullptr;
    IStack* redo = nullptr;
    if (rep == "array") { undo = new StackArray(8); redo = new StackArray(8); }
    else if (rep == "lista") { undo = new StackList(); redo = new StackList(); }
    else { std::cerr << "Representacion invalida: " << rep << "\n"; return 1; }

    std::ofstream flog;
    std::ostream* log = &std::cout;
    if (!rutaLog.empty()) { flog.open(rutaLog); log = &flog; }

    UndoRedoEngine motor(undo, redo);

    auto t0 = Reloj::now();
    std::size_t lineas = motor.procesarArchivo(entrada, *log);
    double ms = msDesde(t0);

    const auto& s = motor.estadisticas();
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "\n=== PROBLEMA 1 (" << undo->nombreRepresentacion() << ") ===\n"
              << "Lineas procesadas   : " << lineas << "\n"
              << "Ediciones aplicadas : " << s.edicionesAplicadas << "\n"
              << "UNDO efectivos      : " << s.undosEfectivos
              << "   no-ops: " << s.undosNoOp << "\n"
              << "REDO efectivos      : " << s.redosEfectivos
              << "   no-ops: " << s.redosNoOp << "\n"
              << "Redos invalidados   : " << s.redosInvalidados << "\n"
              << "Tam. pila Undo final: " << motor.tamPilaUndo() << "\n"
              << "Tam. pila Redo final: " << motor.tamPilaRedo() << "\n"
              << "Long. documento     : " << motor.documento().size() << "\n"
              << "Tiempo total        : " << ms << " ms\n";

    if (rep == "array") {
        auto* sa = static_cast<StackArray*>(undo);
        std::cout << "Redimensionamientos : " << sa->numRedimensionamientos()
                  << "   copias acumuladas: " << sa->copiasAcumuladas()
                  << "   capacidad final: " << sa->capacidad() << "\n";
    }

    delete undo;
    delete redo;
    return 0;
}

int ejecutarP2(const std::string& entrada, std::size_t C, long long T,
               std::size_t L, const std::string& rutaLog)
{
    QueueCircular buffer(C);
    QueueList     ventana;

    std::ofstream flog;
    std::ostream* log = &std::cout;
    if (!rutaLog.empty()) { flog.open(rutaLog); log = &flog; }

    RateLimiter fw(&buffer, &ventana, T, L);

    auto t0 = Reloj::now();
    std::size_t n = fw.procesarArchivo(entrada, *log);
    double ms = msDesde(t0);

    const auto& s = fw.estadisticas();
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "\n=== PROBLEMA 2 (C=" << C << " T=" << T << "ms L=" << L << ") ===\n"
              << "Paquetes procesados      : " << n << "\n"
              << "Aceptados                : " << s.aceptados << "\n"
              << "Rechazados (buffer lleno): " << s.rechazadosBuffer << "\n"
              << "Rechazados (limite tasa) : " << s.rechazadosTasa << "\n"
              << "Ocupacion maxima buffer  : " << s.ocupacionMaxima << " / " << C << "\n"
              << "Marcas purgadas (total)  : " << s.marcasPurgadasTot
              << "   <- comparar con n para el argumento amortizado\n"
              << "Purga maxima puntual     : " << s.purgaMaximaPuntual
              << "   <- peor caso puntual observado\n"
              << "Tam. final buffer        : " << fw.tamBuffer() << "\n"
              << "Tam. final ventana       : " << fw.tamVentana() << "\n"
              << "Tiempo total             : " << ms << " ms\n";
    return 0;
}

int ejecutarBench(std::size_t n, unsigned semilla, int reps) {
    if (reps < 1) reps = 5;

    const std::string f1 = "data/bench_p1.txt";
    const std::string f2 = "data/bench_p2.txt";
    generarEventosEditor(f1, n, semilla);
    generarPaquetes(f2, n, semilla);

    BufNulo bufNulo;
    std::ostream sumidero(&bufNulo);

    double t[64];
    double media = 0.0, desv = 0.0;

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "n = " << n << ", repeticiones = " << reps
              << ", semilla = " << semilla << "\n";

    for (int i = 0; i < reps; ++i) {
        StackArray u(8), r(8);
        UndoRedoEngine m(&u, &r);
        auto t0 = Reloj::now();
        m.procesarArchivo(f1, sumidero);
        t[i] = msDesde(t0);
    }
    mediaYDesv(t, reps, media, desv);
    std::cout << "P1 pila arreglo : " << media << " ms  (sd " << desv << ")\n";

    for (int i = 0; i < reps; ++i) {
        StackList u, r;
        UndoRedoEngine m(&u, &r);
        auto t0 = Reloj::now();
        m.procesarArchivo(f1, sumidero);
        t[i] = msDesde(t0);
    }
    mediaYDesv(t, reps, media, desv);
    std::cout << "P1 pila lista   : " << media << " ms  (sd " << desv << ")\n";

    for (int i = 0; i < reps; ++i) {
        QueueCircular b(1024);
        QueueList     v;
        RateLimiter fw(&b, &v, 1000, 200);
        auto t0 = Reloj::now();
        fw.procesarArchivo(f2, sumidero);
        t[i] = msDesde(t0);
    }
    mediaYDesv(t, reps, media, desv);
    std::cout << "P2 firewall     : " << media << " ms  (sd " << desv << ")\n";

    std::cout << "\nNOTA: el tiempo incluye la lectura y el parseo del archivo, que es\n"
                 "O(n) y domina las constantes. Para aislar el costo de las estructuras,\n"
                 "carguen los eventos a memoria primero y midan solo el bucle de\n"
                 "operaciones. Discutan esa decision metodologica en la Seccion 10.\n";
    return 0;
}

}

int main(int argc, char** argv) {
    if (argc < 2) { uso(); return 1; }
    std::string cmd = argv[1];

    if (cmd == "gen1" && argc >= 5) {
        generarEventosEditor(argv[4], std::strtoul(argv[2], nullptr, 10),
                             static_cast<unsigned>(std::strtoul(argv[3], nullptr, 10)));
        std::cout << "Generado: " << argv[4] << "\n";
        return 0;
    }
    if (cmd == "gen2" && argc >= 5) {
        generarPaquetes(argv[4], std::strtoul(argv[2], nullptr, 10),
                        static_cast<unsigned>(std::strtoul(argv[3], nullptr, 10)));
        std::cout << "Generado: " << argv[4] << "\n";
        return 0;
    }
    if (cmd == "p1" && argc >= 4) {
        return ejecutarP1(argv[2], argv[3], argc >= 5 ? argv[4] : "");
    }
    if (cmd == "p2" && argc >= 6) {
        return ejecutarP2(argv[2],
                          std::strtoul(argv[3], nullptr, 10),
                          std::strtoll(argv[4], nullptr, 10),
                          std::strtoul(argv[5], nullptr, 10),
                          argc >= 7 ? argv[6] : "");
    }
    if (cmd == "bench" && argc >= 5) {
        return ejecutarBench(std::strtoul(argv[2], nullptr, 10),
                             static_cast<unsigned>(std::strtoul(argv[3], nullptr, 10)),
                             std::atoi(argv[4]));
    }

    uso();
    return 1;
}
