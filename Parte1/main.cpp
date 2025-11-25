#include <iostream>
#include "simulador.h"
using namespace std;
int main() {
    Simulador sim(500, 400, 0.1);  // ancho, alto, paso de tiempo

    // Crear 4 partículas
    sim.agregarParticula(Particula(100, 100, 30, 20, 2, 5));
    sim.agregarParticula(Particula(200, 150, -25, 15, 3, 5));
    sim.agregarParticula(Particula(300, 200, 20, -30, 1, 5));
    sim.agregarParticula(Particula(250, 300, -15, -10, 2, 5));

    double tiempo_total = 10.0;
    double t = 0.0;

    while (t < tiempo_total) {
        sim.actualizar();
        sim.guardarEstado(t);
        t += sim.dt;
    }

    cout << "Simulación terminada. Resultados guardados en resultados.txt\n";
    return 0;
}
