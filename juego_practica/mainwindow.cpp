/*
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}
*/
#include "mainwindow.h"

// Constructor
MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    setWindowTitle("Juego por turnos - Demo");
    resize(1100, 600);

    // Scene & View
    m_sceneRect = QRectF(0,0,1000,480);
    m_scene = new QGraphicsScene(m_sceneRect, this);
    m_scene->setBackgroundBrush(QBrush(QColor(250,250,250)));
    m_view = new QGraphicsView(m_scene);
    m_view->setRenderHint(QPainter::Antialiasing);
    setCentralWidget(m_view);

    // UI controls
    createControls();

    // Build visual layout
    buildInterface();

    // Timer
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &MainWindow::onTick);
    m_timer->start(int(DT * 1000));

    // initial
    updateTurnLabel();
}

// ---------------------------- SLOTS ----------------------------

void MainWindow::onIncreaseAngle() {
    m_angle = std::min(175.0, m_angle + 5.0);
    updateAngleLabel();
}

void MainWindow::onFire() {
    if (m_projectile) return;

    QPointF muzzle = (m_currentPlayer == 1) ? m_cannonLeftMuzzle : m_cannonRightMuzzle;
    double angleDeg = (m_currentPlayer == 1) ? m_angle : (180.0 - m_angle);
    double angleRad = angleDeg * PI / 180.0;
    double vx = m_speed * std::cos(angleRad);
    double vy = -m_speed * std::sin(angleRad);
    if (m_currentPlayer == 2) vx = -vx;
    QPointF vel(vx, vy);

    // CORRECCIÓN 1: De Projectile a Proyectil
    m_projectile = new Proyectil(m_mass, m_radius, muzzle, vel);
    m_scene->addItem(m_projectile);

    qDebug() << "Disparo: Jugador" << m_currentPlayer << "angulo" << angleDeg << "vel" << m_speed;
}

void MainWindow::onTick() {
    if (!m_projectile) return;

    // CORRECCIÓN 2: De advancePhysics a movimiento
    m_projectile->movimiento(DT);

    handleWallCollision(m_projectile);
    handleObstaclesCollision(m_projectile);

    // CORRECCIÓN 3: De velocity() a velocidad()
    QPointF ppos = m_projectile->pos();
    QPointF v = m_projectile->velocidad();
    double speed = std::hypot(v.x(), v.y());

    if (!m_sceneRect.contains(ppos) || speed < 5.0) {
        removeProjectile();
        nextTurn();
    }
}

// ---------------------------- HELPERS (UI) ----------------------------

void MainWindow::createControls() {
    // ... (Código de createControls sin cambios funcionales) ...
    QDockWidget* dock = new QDockWidget("Controles", this);
    QWidget* w = new QWidget;
    QVBoxLayout* vlay = new QVBoxLayout;

    m_turnLabel = new QLabel;
    vlay->addWidget(m_turnLabel);

    QPushButton* incBtn = new QPushButton("Aumentar Ángulo +5°");
    connect(incBtn, &QPushButton::clicked, this, &MainWindow::onIncreaseAngle);
    vlay->addWidget(incBtn);

    QPushButton* fireBtn = new QPushButton("Disparar");
    connect(fireBtn, &QPushButton::clicked, this, &MainWindow::onFire);
    vlay->addWidget(fireBtn);

    m_angleLabel = new QLabel;
    vlay->addWidget(m_angleLabel);

    vlay->addSpacing(10);
    vlay->addWidget(new QLabel("Velocidad (px/s):"));
    m_speedSpin = new QDoubleSpinBox; m_speedSpin->setRange(50,1000); m_speedSpin->setValue(m_speed);
    connect(m_speedSpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, [&](double v){ m_speed = v; updateSpeedLabel(); });
    vlay->addWidget(m_speedSpin);

    m_speedLabel = new QLabel;
    vlay->addWidget(m_speedLabel);

    vlay->addWidget(new QLabel("Masa (u):"));
    m_massSpin = new QDoubleSpinBox; m_massSpin->setRange(0.1,10); m_massSpin->setValue(m_mass);
    connect(m_massSpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, [&](double v){ m_mass = v; });
    vlay->addWidget(m_massSpin);

    vlay->addWidget(new QLabel("Radio (px):"));
    m_radiusSpin = new QDoubleSpinBox; m_radiusSpin->setRange(3,20); m_radiusSpin->setValue(m_radius);
    connect(m_radiusSpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, [&](double v){ m_radius = v; });
    vlay->addWidget(m_radiusSpin);

    vlay->addWidget(new QLabel("Coef restitución (e):"));
    m_restitutionSpin = new QDoubleSpinBox; m_restitutionSpin->setRange(0.0,1.0); m_restitutionSpin->setDecimals(2);
    m_restitutionSpin->setValue(m_restitution);
    connect(m_restitutionSpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, [&](double v){ m_restitution = v; });
    vlay->addWidget(m_restitutionSpin);

    vlay->addWidget(new QLabel("Factor daño (k):"));
    m_damageFactorSpin = new QDoubleSpinBox; m_damageFactorSpin->setRange(0.01,5.0); m_damageFactorSpin->setDecimals(2);
    m_damageFactorSpin->setValue(m_damageFactor);
    connect(m_damageFactorSpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, [&](double v){ m_damageFactor = v; });
    vlay->addWidget(m_damageFactorSpin);

    vlay->addStretch();
    w->setLayout(vlay);
    dock->setWidget(w);
    addDockWidget(Qt::RightDockWidgetArea, dock);

    updateAngleLabel();
    updateSpeedLabel();
}

void MainWindow::updateTurnLabel() {
    m_turnLabel->setText(QString("Turno: Jugador %1").arg(m_currentPlayer));
}
void MainWindow::updateAngleLabel() {
    m_angleLabel->setText(QString("Ángulo actual: %1°").arg(m_angle));
}
void MainWindow::updateSpeedLabel() {
    m_speedLabel->setText(QString("Velocidad: %1 px/s").arg(m_speed));
}

void MainWindow::buildInterface() {
    // ... (Código de buildInterface sin cambios funcionales) ...
    // Left cannon platform
    m_scene->addRect(20, 320, 80, 10, QPen(), QBrush(Qt::lightGray));
    m_scene->addRect(900-100, 320, 80, 10, QPen(), QBrush(Qt::lightGray));

    // Cannons (simple rectangles with small wheel)
    m_scene->addRect(30, 260, 40, 40, QPen(Qt::black), QBrush(Qt::blue));
    m_scene->addEllipse(30, 300, 12, 12, QPen(), QBrush(Qt::black));
    m_scene->addRect(900-70, 260, 40, 40, QPen(Qt::black), QBrush(Qt::blue));
    m_scene->addEllipse(900-62, 300, 12, 12, QPen(), QBrush(Qt::black));

    // Place muzzle points (relative to shapes)
    m_cannonLeftMuzzle = QPointF(30 + 40 + 6, 260 + 20);
    m_cannonRightMuzzle = QPointF(900-70 - 6, 260 + 20);

    // Draw two rival structures: left and right
    double baseY = 360;
    // Left group - CORRECCIÓN 4: Usar Obstaculo
    Obstaculo* left_sideL = new Obstaculo(QRectF(120, baseY-150, 60, 150), 200.0);
    Obstaculo* left_sideR = new Obstaculo(QRectF(260, baseY-150, 60, 150), 200.0);
    Obstaculo* left_top   = new Obstaculo(QRectF(120+60, baseY-200, 140, 50), 100.0);
    m_scene->addItem(left_sideL);
    m_scene->addItem(left_sideR);
    m_scene->addItem(left_top);
    m_obstaclesLeft << left_sideL << left_sideR << left_top;
    drawStickman(200, baseY-60);

    // Right group - CORRECCIÓN 5: Usar Obstaculo
    Obstaculo* right_sideL = new Obstaculo(QRectF(600, baseY-150, 60, 150), 200.0);
    Obstaculo* right_sideR = new Obstaculo(QRectF(740, baseY-150, 60, 150), 200.0);
    Obstaculo* right_top   = new Obstaculo(QRectF(600+60, baseY-200, 140, 50), 100.0);
    m_scene->addItem(right_sideL);
    m_scene->addItem(right_sideR);
    m_scene->addItem(right_top);
    m_obstaclesRight << right_sideL << right_sideR << right_top;
    drawStickman(680, baseY-60);

    // ground rectangle border
    m_scene->addRect(m_sceneRect, QPen(Qt::black, 2));
}

void MainWindow::drawStickman(double cx, double cy) {
    // ... (Código de drawStickman sin cambios funcionales) ...
    // head
    m_scene->addEllipse(cx-8, cy-40, 16, 16, QPen(), QBrush(Qt::white));
    // torso
    m_scene->addLine(cx, cy-24, cx, cy-4, QPen());
    // arms
    m_scene->addLine(cx, cy-16, cx-14, cy-6, QPen());
    m_scene->addLine(cx, cy-16, cx+14, cy-6, QPen());
    // legs
    m_scene->addLine(cx, cy-4, cx-12, cy+16, QPen());
    m_scene->addLine(cx, cy-4, cx+12, cy+16, QPen());
    // label
    QGraphicsSimpleTextItem* t = m_scene->addSimpleText("Rival");
    t->setPos(cx-16, cy+18);
}

// ---------------------------- HELPERS (GAME LOGIC) ----------------------------

// CORRECCIÓN 6: Tipo de parámetro Proyectil
void MainWindow::handleWallCollision(Proyectil* p) {
    QPointF pos = p->pos();
    // CORRECCIÓN 7: De velocity() a velocidad()
    QPointF v = p->velocidad();
    // CORRECCIÓN 8: De radius() a radio()
    double r = p->radio();
    bool collided = false;

    // ... (lógica de colisión)
    if (pos.x() - r < m_sceneRect.left()) {
        pos.setX(m_sceneRect.left() + r);
        v.setX(-v.x());
        collided = true;
    }
    if (pos.x() + r > m_sceneRect.right()) {
        pos.setX(m_sceneRect.right() - r);
        v.setX(-v.x());
        collided = true;
    }
    if (pos.y() - r < m_sceneRect.top()) {
        pos.setY(m_sceneRect.top() + r);
        v.setY(-v.y());
        collided = true;
    }
    if (pos.y() + r > m_sceneRect.bottom()) {
        pos.setY(m_sceneRect.bottom() - r);
        v.setY(-v.y());
        collided = true;
    }

    if (collided) {
        p->setPos(pos);
        // CORRECCIÓN 9: De setVelocity() a setVelocidad()
        p->setVelocidad(v);
    }
}

// CORRECCIÓN 10: Tipo de parámetro Proyectil
void MainWindow::handleObstaclesCollision(Proyectil* p) {
    checkCollisionList(p, m_obstaclesLeft);
    checkCollisionList(p, m_obstaclesRight);
}

// CORRECCIÓN 11: Tipos de parámetros Proyectil y Obstaculo
void MainWindow::checkCollisionList(Proyectil* p, QVector<Obstaculo*> &list) {
    QRectF projRect = p->boundingRect().translated(p->pos());
    // CORRECCIÓN 12: Tipo de iterador Obstaculo
    for (Obstaculo* ob : list) {
        if (ob->destroyed()) continue;
        QRectF obRect = ob->rect().translated(ob->pos());
        if (projRect.intersects(obRect)) {

            QPointF projCenter = p->pos();
            QPointF obCenter = obRect.center();
            QPointF diff = projCenter - obCenter;
            // CORRECCIÓN 13: De velocity() a velocidad()
            QPointF v = p->velocidad();

            // damage calculation
            double speed = std::hypot(v.x(), v.y());
            // CORRECCIÓN 14: De mass() a masa()
            double damage = m_damageFactor * p->masa() * speed;
            ob->applyDamage(damage);
            qDebug() << "Impacto: daño=" << damage << "resta=" << ob->resistance();

            // collision response
            if (std::abs(diff.x()) > std::abs(diff.y())) {
                v.setX(-m_restitution * v.x());
            } else {
                v.setY(-m_restitution * v.y());
            }
            // CORRECCIÓN 15: De setVelocity() a setVelocidad()
            p->setVelocidad(v);

            // visual small displacement
            if (std::abs(diff.x()) > std::abs(diff.y())) {
                // CORRECCIÓN 16: De radius() a radio()
                if (diff.x() > 0)
                    p->setPos(obRect.right() + p->radio() + 1, p->pos().y());
                else
                    p->setPos(obRect.left() - p->radio() - 1, p->pos().y());
            } else {
                // CORRECCIÓN 17: De radius() a radio()
                if (diff.y() > 0)
                    p->setPos(p->pos().x(), obRect.bottom() + p->radio() + 1);
                else
                    p->setPos(p->pos().x(), obRect.top() - p->radio() - 1);
            }

            if (ob->destroyed()) {
                qDebug() << "Bloque destruido!";
                if (checkVictory()) {
                    showVictoryDialog();
                }
            }
            break;
        }
    }
}

bool MainWindow::checkVictory() {
    // CORRECCIÓN 18: Tipo de iterador Obstaculo
    bool leftAll = true, rightAll = true;
    for (Obstaculo* o : m_obstaclesLeft) if (!o->destroyed()) leftAll = false;
    for (Obstaculo* o : m_obstaclesRight) if (!o->destroyed()) rightAll = false;
    if (leftAll || rightAll) {
        return true;
    }
    return false;
}

void MainWindow::showVictoryDialog() {
    int winner = -1;
    // CORRECCIÓN 19: Tipo de iterador Obstaculo
    bool leftAll = true, rightAll = true;
    for (Obstaculo* o : m_obstaclesLeft) if (!o->destroyed()) leftAll = false;
    for (Obstaculo* o : m_obstaclesRight) if (!o->destroyed()) rightAll = false;
    if (leftAll) winner = 2;
    if (rightAll) winner = 1;

    if (winner != -1) {
        QString msg = QString("¡Jugador %1 gana!").arg(winner);
        QMessageBox::information(this, "Victoria", msg);
        resetGame();
    }
}

void MainWindow::removeProjectile() {
    if (!m_projectile) return;
    m_scene->removeItem(m_projectile);
    delete m_projectile;
    m_projectile = nullptr;
}

void MainWindow::nextTurn() {
    m_currentPlayer = (m_currentPlayer == 1) ? 2 : 1;
    updateTurnLabel();
}

void MainWindow::resetGame() {
    // CORRECCIÓN 20: Tipo de iterador Obstaculo
    for (Obstaculo* o : m_obstaclesLeft) { m_scene->removeItem(o); delete o; }
    for (Obstaculo* o : m_obstaclesRight) { m_scene->removeItem(o); delete o; }
    m_obstaclesLeft.clear(); m_obstaclesRight.clear();

    // La lista de items ya no es necesaria aquí, ya que solo eliminamos
    // los obstáculos de las listas. buildInterface() vuelve a dibujar todo lo fijo.

    buildInterface();
    removeProjectile();
    m_currentPlayer = 1;
    m_angle = 45.0;
    updateAngleLabel();
    updateTurnLabel();
}
