#ifndef SIMULADOR_H
#define SIMULADOR_H

#include <vector>
#include "Particula.h"

class Simulador {
private:
    double ancho, alto;
    std::vector<Particula> particulas;

public:
    Simulador(double width, double height);

    void agregarParticula(const Particula& p);
    void verificarColisionPared(Particula& p);
    void simular(double tiempoTotal, double dt);
};

#endif
