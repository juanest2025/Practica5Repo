#ifndef PARTICULA_H
#define PARTICULA_H

#include <iostream>

class Particula {
public:
    double x, y;       // posición
    double vx, vy;     // velocidad
    double masa;       // masa
    double radio;      // tamaño aproximado

    Particula(double x0, double y0, double vx0, double vy0, double m, double r)
        : x(x0), y(y0), vx(vx0), vy(vy0), masa(m), radio(r) {}

    void mover(double dt) {
        x += vx * dt;
        y += vy * dt;
    }
};

#endif
