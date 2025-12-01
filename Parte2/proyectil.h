#ifndef PROYECTIL_H
#define PROYECTIL_H

#include <QObject>
#include <QGraphicsEllipseItem>
#include <QPointF>
#include <QBrush>

class Proyectil : public QObject, public QGraphicsEllipseItem {
    Q_OBJECT
public:
    Proyectil(double masa, double radio, const QPointF& pos, const QPointF& vel, QGraphicsItem* parent = nullptr);

    QPointF velocidad() const;
    void setVelocidad(const QPointF& v);
    double masa() const;
    double radio() const;
    void movimiento(double dt); // Función de avance de física (usada en MainWindow::onTick)

private:
    double m_masa;
    double m_radio;
    QPointF m_velocidad;
};

#endif // PROYECTIL_H
