#include "particula.h"

Particle::Particle(int id_, double x_, double y_, double vx_, double vy_, double mass_)
    : id(id_), x(x_), y(y_), vx(vx_), vy(vy_), mass(mass_) {
    radius = std::sqrt(mass / M_PI);
}

void Particle::move(double dt) {
    x += vx * dt;
    y += vy * dt;
}

void Particle::updateRadius() {
    radius = std::sqrt(mass / M_PI);
}

Particula::Particula() {}
