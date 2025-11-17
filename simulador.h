#ifndef SIMULADOR_H
#define SIMULADOR_H

#include <vector>
#include <fstream>
#include "particula.h"

class Simulador {
public:
    double ancho, alto;          // Tamaño de la caja
    double dt;                   // Paso de tiempo
    std::vector<Particula> particulas;
    std::ofstream archivo;

    Simulador(double w, double h, double paso)
        : ancho(w), alto(h), dt(paso) {
        archivo.open("resultados.txt");
    }

    ~Simulador() {
        archivo.close();
    }

    void agregarParticula(Particula p) {
        particulas.push_back(p);
    }

    void actualizar() {
        for (auto &p : particulas) {
            p.mover(dt);
            verificarColisionPared(p);
        }
    }

    void verificarColisionPared(Particula &p) {
        // Rebote con las paredes
        if (p.x - p.radio < 0 || p.x + p.radio > ancho) {
            p.vx *= -1; // invierte la velocidad en x
        }
        if (p.y - p.radio < 0 || p.y + p.radio > alto) {
            p.vy *= -1; // invierte la velocidad en y
        }
    }

    void guardarEstado(double tiempo) {
        archivo << "Tiempo: " << tiempo << "\n";
        for (size_t i = 0; i < particulas.size(); ++i) {
            archivo << "Particula " << i
                    << " x=" << particulas[i].x
                    << " y=" << particulas[i].y << "\n";
        }
        archivo << "\n";
    }
};

#endif
