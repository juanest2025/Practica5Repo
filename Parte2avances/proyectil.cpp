#include "proyectil.h"
#include <QGraphicsScene>
#include <QDebug>

Proyectil::Proyectil(double vx0, double vy0, double g, int owner0)
{
    vx = vx0;
    vy = vy0;
    gravedad = g;
    owner = owner0;

    // Centrado localmente
    setRect(-7.5, -7.5, 15, 15);
    setBrush(Qt::red);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Proyectil::mover);
    timer->start(16);

    // si no choca en 5s, destruir
    QTimer::singleShot(5000, this, [=](){
        if (timer->isActive()) timer->stop();
        emit destruir();
    });
}

void Proyectil::mover()
{
    // Movimiento básico
    moveBy(vx, vy);
    vy += gravedad;

    // Revisar colisiones
    QList<QGraphicsItem*> colisiones = collidingItems();
    for (auto *item : colisiones)
    {
        QString tipo = item->data(0).toString();

        if (tipo == "cajas")
        {
            if (timer->isActive()) timer->stop();

            // daño ligero por caer en caja
            if (owner == 1)
                emit danioEstructura(2, 10);
            else
                emit danioEstructura(1, 10);

            emit destruir();   // ← OBLIGATORIO
            return;
        }

        if (tipo == "estructura")
        {
            if (timer->isActive()) timer->stop();

            if (owner == 1)
                emit danioEstructura(2, 20);
            else
                emit danioEstructura(1, 20);

            emit destruir();
            return;
        }

        if (tipo == "suelo")
        {
            rebotes++;
            vy = -0.65 * vy;
            qDebug() << "Proyectil: suelo rebote" << rebotes;

            if (rebotes > 4)
            {
                if (timer->isActive()) timer->stop();
                scene()->removeItem(this);
                delete this;
                return;
            }
        }
    }
}

