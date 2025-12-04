#ifndef PARTICULA_H
#define PARTICULA_H

class Particula {
private:
    double x, y;
    double vx, vy;
    double masa;
    double radio;

public:
    Particula(double x0, double y0, double vx0, double vy0, double m, double r);

    void actualizar(double dt);

    double getX() const;
    double getY() const;
    double getVx() const;
    double getVy() const;
    double getMasa() const;
    double getRadio() const;

    void setVx(double v);
    void setVy(double v);
};

#endif
