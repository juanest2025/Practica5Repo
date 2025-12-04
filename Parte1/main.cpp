#include <iostream>
#include "Simulador.h"

int main() {
    Simulador sim(500, 400);

    sim.agregarParticula(Particula(100, 100, 30, 20, 2, 5));
    sim.agregarParticula(Particula(200, 150, -25, 15, 3, 5));
    sim.agregarParticula(Particula(300, 200, 20, -30, 1, 5));
    sim.agregarParticula(Particula(250, 300, -15, -10, 2, 5));

    sim.simular(10.0, 0.1);

    return 0;
}
