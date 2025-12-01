#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTimer>
#include <QLabel>
#include <QPushButton>
#include <QDoubleSpinBox>
#include <QDockWidget>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QDebug>
#include <cmath>
#include <algorithm>
#include <QPainter> // Necesario para QGraphicsView::setRenderHint
#include "constante.h"

// Asumiendo que estas constantes y clases están definidas en alguna parte
// NOTA: Debes definir 'DT', 'PI', 'DEFAULT_RESTITUTION', 'DEFAULT_DAMAGE_FACTOR' en un archivo (ej. constants.h)
#include "proyectil.h" // Clase Proyectil
#include "obstaculo.h"  // Clase Obstaculo

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget* parent = nullptr);

private slots:
    void onIncreaseAngle();
    void onFire();
    void onTick();

private:
    // Scene & UI
    QGraphicsScene* m_scene = nullptr;
    QGraphicsView* m_view = nullptr;
    QRectF m_sceneRect;
    QTimer* m_timer = nullptr;

    // Controls
    QWidget* m_controlWidget = nullptr;
    QLabel* m_turnLabel = nullptr;
    QLabel* m_angleLabel = nullptr;
    QLabel* m_speedLabel = nullptr;
    QDoubleSpinBox* m_speedSpin = nullptr;
    QDoubleSpinBox* m_massSpin = nullptr;
    QDoubleSpinBox* m_radiusSpin = nullptr;
    QDoubleSpinBox* m_restitutionSpin = nullptr;
    QDoubleSpinBox* m_damageFactorSpin = nullptr;

    // Game objects
    Proyectil* m_projectile = nullptr; // Tipo corregido
    QVector<Obstaculo*> m_obstaclesLeft; // Tipo corregido
    QVector<Obstaculo*> m_obstaclesRight; // Tipo corregido
    QPointF m_cannonLeftMuzzle, m_cannonRightMuzzle;

    // Game state
    int m_currentPlayer = 1;
    double m_angle = 45.0;
    double m_speed = 400.0;
    double m_mass = 1.0;
    double m_radius = 8.0;
    double m_restitution = DEFAULT_RESTITUTION;
    double m_damageFactor = DEFAULT_DAMAGE_FACTOR;

    // Helpers
    void createControls();
    void updateTurnLabel();
    void updateAngleLabel();
    void updateSpeedLabel();
    void buildInterface();
    void drawStickman(double cx, double cy);

    // Physics/Game Logic (Firmas corregidas)
    void handleWallCollision(Proyectil* p);
    void handleObstaclesCollision(Proyectil* p);
    void checkCollisionList(Proyectil* p, QVector<Obstaculo*> &list);
    bool checkVictory();
    void showVictoryDialog();
    void removeProjectile();
    void nextTurn();
    void resetGame();
};

#endif // MAINWINDOW_H
