#include "obstaculo.h"

Obstaculo::Obstaculo(const QRectF &rect, double resistance, QGraphicsItem* parent)
    : QGraphicsRectItem(rect, parent), m_resistance(resistance), m_initial(resistance)
{
    setBrush(QBrush(QColor(255, 220, 180)));
    setPen(QPen(Qt::darkGray));
    m_label = new QGraphicsSimpleTextItem(QString::number(int(m_resistance)), this);
    m_label->setPos(rect.width()/2 - 10, -20);
}

double Obstaculo::resistance() const {
    return m_resistance;
}

void Obstaculo::applyDamage(double d) {
    if (m_resistance <= 0) return;
    m_resistance -= d;
    if (m_resistance < 0) m_resistance = 0;
    updateVisual();
}

bool Obstaculo::destroyed() const {
    return m_resistance <= 0;
}

// CORREGIDO: Estaba mal tipografiado como 'Obstacle::updateVisual'
void Obstaculo::updateVisual() {
    if (m_resistance <= 0) {
        setBrush(QBrush(Qt::red));
        m_label->setText("0");
    } else {
        double ratio = m_resistance / m_initial;
        int gray = int(180 * ratio);
        setBrush(QBrush(QColor(gray, gray, gray)));
        m_label->setText(QString::number(int(m_resistance)));
    }
}
