#include "proyectil.h"
#include "constante.h"

// NOTA: Debes definir 'GRAVITY' en un archivo (ej. constants.h)
// const double GRAVITY = 9.8;

Proyectil::Proyectil(double masa, double radio, const QPointF& pos, const QPointF& vel, QGraphicsItem* parent)
    : QGraphicsEllipseItem(-radio, -radio, 2*radio, 2*radio, parent),
    m_masa(masa), m_radio(radio), m_velocidad(vel)
{
    setBrush(QBrush(Qt::red));
    setPos(pos);
}

QPointF Proyectil::velocidad() const {
    return m_velocidad;
}

void Proyectil::setVelocidad(const QPointF& v) {
    m_velocidad = v;
}

double Proyectil::masa() const {
    return m_masa;
}

double Proyectil::radio() const {
    return m_radio;
}

void Proyectil::movimiento(double dt) {
    m_velocidad.setY(m_velocidad.y() + GRAVITY * dt);
    moveBy(m_velocidad.x() * dt, m_velocidad.y() * dt);
}
