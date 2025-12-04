#include "Particula.h"

Particula::Particula(double x0, double y0, double vx0, double vy0, double m, double r)
    : x(x0), y(y0), vx(vx0), vy(vy0), masa(m), radio(r) {}

void Particula::actualizar(double dt) {
    x += vx * dt;
    y += vy * dt;
}

double Particula::getX() const { return x; }
double Particula::getY() const { return y; }
double Particula::getVx() const { return vx; }
double Particula::getVy() const { return vy; }
double Particula::getMasa() const { return masa; }
double Particula::getRadio() const { return radio; }

void Particula::setVx(double v) { vx = v; }
void Particula::setVy(double v) { vy = v; }
