CXX      = g++
CXXFLAGS = -std=c++17 -O2 -Wall -Wextra
SRC      = $(wildcard src/*.cpp)
BIN      = ada_p1
SEMILLA  = 20260828

all: $(BIN)

$(BIN): $(SRC)
	$(CXX) $(CXXFLAGS) -o $@ $(SRC)

# Compilacion sin optimizacion, util para depurar con gdb/valgrind
debug: $(SRC)
	$(CXX) -std=c++17 -O0 -g -fsanitize=address,undefined -o $(BIN)_debug $(SRC)

tests: $(BIN)
	@bash run_tests.sh

bench: $(BIN)
	@mkdir -p data results
	@for n in 1000 10000 100000 1000000; do ./$(BIN) bench $$n $(SEMILLA) 5; done

clean:
	rm -f $(BIN) $(BIN)_debug data/bench_*.txt

.PHONY: all debug tests bench clean
