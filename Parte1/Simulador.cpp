#include "Simulador.h"
#include <fstream>
#include <iostream>

Simulador::Simulador(double width, double height)
    : ancho(width), alto(height) {}

void Simulador::agregarParticula(const Particula& p) {
    particulas.push_back(p);
}

void Simulador::verificarColisionPared(Particula& p) {
    // Rebote horizontal
    if (p.getX() - p.getRadio() < 0 || p.getX() + p.getRadio() > ancho) {
        double newVx = -p.getVx();
        p.setVx(newVx);
    }

    // Rebote vertical
    if (p.getY() - p.getRadio() < 0 || p.getY() + p.getRadio() > alto) {
        double newVy = -p.getVy();
        p.setVy(newVy);
    }
}

void Simulador::simular(double tiempoTotal, double dt) {
    std::ofstream archivo("resultados.txt");

    if (!archivo.is_open()) {
        std::cerr << "Error al abrir resultados.txt\n";
        return;
    }

    for (double t = 0; t <= tiempoTotal; t += dt) {

        archivo << "Tiempo: " << t << "\n";
        std::cout << "Tiempo: " << t << "\n";

        for (size_t i = 0; i < particulas.size(); i++) {

            particulas[i].actualizar(dt);
            verificarColisionPared(particulas[i]);

            archivo << "Particula " << i
                    << " x=" << particulas[i].getX()
                    << " y=" << particulas[i].getY() << "\n";

            std::cout << "Particula " << i
                      << " x=" << particulas[i].getX()
                      << " y=" << particulas[i].getY() << "\n";
        }

        archivo << "\n";
    }

    archivo.close();
    std::cout << "\nResultados guardados en resultados.txt\n";
}
