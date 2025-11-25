#ifndef OBSTACULO_H
#define OBSTACULO_H
#include <QObject>
#include <QGraphicsRectItem>
#include <QGraphicsSimpleTextItem>
#include <QBrush>
#include <QPen>
#include <QColor>
#include <QString>

class Obstaculo : public QObject, public QGraphicsRectItem {
    Q_OBJECT
public:
    Obstaculo(const QRectF &rect, double resistance, QGraphicsItem* parent = nullptr);

    double resistance() const;
    void applyDamage(double d);
    bool destroyed() const;

private:
    void updateVisual();

    double m_resistance;
    double m_initial;
    QGraphicsSimpleTextItem* m_label;
};


#endif // OBSTACULO_H
