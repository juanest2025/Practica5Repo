#ifndef PROYECTIL_H
#define PROYECTIL_H

#include <QObject>
#include <QGraphicsEllipseItem>
#include <QTimer>

class Proyectil : public QObject, public QGraphicsEllipseItem
{
    Q_OBJECT
public:
    // owner: 1 => disparó jugador 1 (izquierda), 2 => disparó jugador 2 (derecha)
    explicit Proyectil(double vx0, double vy0, double g, int owner0);

signals:
    void destruir();
    void danioEstructura(int jugador, int danio);

private slots:
    void mover();

private:
    double vx;
    double vy;
    double gravedad;
    int rebotes = 0;
    int owner;

    QTimer *timer;
};

#endif // PROYECTIL_H

