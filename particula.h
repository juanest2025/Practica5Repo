#ifndef PARTICULA_H
#define PARTICULA_H

<<<<<<< HEAD
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
=======
#include <cmath>

class Particula
{

public:
    int id;
    double x, y;
    double vx, vy;
    double mass;
    double radius;

    Particula(int id_, double x_, double y_, double vx_, double vy_, double mass_);
    void move(double dt);
    void updateRadius();
};

#endif // PARTICULA_H
>>>>>>> 1dbb7aae41edf067df5169f09332ef0b8378459c
