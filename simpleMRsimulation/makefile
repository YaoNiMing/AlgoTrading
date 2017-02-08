bin/trading.so: bin/trading.o bin/sim.o bin/grids.o bin/strategy.o 
	g++-mp-4.7 -std=c++11 -I inc -shared -o bin/trading.so bin/trading.o bin/sim.o bin/grids.o bin/strategy.o -pthread

bin/trading.o: src/trading.cpp
	g++-mp-4.7 -c -std=c++11 -fPIC src/trading.cpp -o bin/trading.o

bin/grids.o: src/grids.cpp
	g++-mp-4.7 -c -std=c++11 -fPIC src/grids.cpp -o bin/grids.o


bin/strategy.o: src/strategy.cpp
	g++-mp-4.7 -c -std=c++11 -fPIC src/strategy.cpp -o bin/strategy.o

bin/sim.o: src/sim.cpp
	g++-mp-4.7 -c -std=c++11 -fPIC src/sim.cpp -o bin/sim.o
