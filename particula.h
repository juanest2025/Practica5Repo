#ifndef PARTICULA_H
#define PARTICULA_H

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
