#include "mainwindow.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    setWindowTitle("Juego por turnos - Demo");
    resize(1100, 600);

    m_sceneRect = QRectF(0, 0, 1000, 480);
    m_scene = new QGraphicsScene(m_sceneRect, this);
    m_scene->setBackgroundBrush(QBrush(QColor(250, 250, 250)));
    m_view = new QGraphicsView(m_scene);
    m_view->setRenderHint(QPainter::Antialiasing);
    setCentralWidget(m_view);

    createControls();
    buildInterface();

    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &MainWindow::onTick);
    m_timer->start(int(DT * 1000));

    updateTurnLabel();
}

void MainWindow::onIncreaseAngle() {
    m_angle = std::min(175.0, m_angle + 5.0);
    updateAngleLabel();
}

void MainWindow::onFire() {
    if (m_projectile) return;

    QPointF muzzle = (m_currentPlayer == 1) ? m_cannonLeftMuzzle : m_cannonRightMuzzle;
    double angleDeg = (m_currentPlayer == 1) ? m_angle : (180.0 - m_angle);
    double angleRad = angleDeg * PI / 180.0;
    double vx = m_speed * std::cos(angleRad) * (m_currentPlayer == 2 ? -1 : 1);
    double vy = -m_speed * std::sin(angleRad);
    QPointF vel(vx, vy);

    m_projectile = new Proyectil(m_mass, m_radius, muzzle, vel);
    m_scene->addItem(m_projectile);

    qDebug() << "Disparo: Jugador" << m_currentPlayer << "angulo" << angleDeg << "vel" << m_speed;
}

void MainWindow::onTick() {
    if (!m_projectile) return;

    m_projectile->movimiento(DT);
    handleWallCollision(m_projectile);
    handleObstaclesCollision(m_projectile);

    QPointF ppos = m_projectile->pos();
    QPointF v = m_projectile->velocidad();
    double speed = std::hypot(v.x(), v.y());

    if (!m_sceneRect.contains(ppos) || speed < 5.0) {
        removeProjectile();
        nextTurn();
    }
}

void MainWindow::createControls() {
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

    m_speedLabel = new QLabel;
    vlay->addWidget(m_speedLabel);

    m_speedSpin = new QDoubleSpinBox;
    m_speedSpin->setRange(10, 2000);
    m_speedSpin->setValue(m_speed);
    connect(m_speedSpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), [this](double v){ m_speed = v; updateSpeedLabel(); });
    vlay->addWidget(m_speedSpin);

    m_massSpin = new QDoubleSpinBox;
    m_massSpin->setRange(0.1, 100);
    m_massSpin->setValue(m_mass);
    connect(m_massSpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), [this](double v){ m_mass = v; });
    vlay->addWidget(m_massSpin);

    m_radiusSpin = new QDoubleSpinBox;
    m_radiusSpin->setRange(1, 20);
    m_radiusSpin->setValue(m_radius);
    connect(m_radiusSpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), [this](double v){ m_radius = v; });
    vlay->addWidget(m_radiusSpin);

    m_restitutionSpin = new QDoubleSpinBox;
    m_restitutionSpin->setRange(0, 1);
    m_restitutionSpin->setValue(m_restitution);
    connect(m_restitutionSpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), [this](double v){ m_restitution = v; });
    vlay->addWidget(m_restitutionSpin);

    m_damageFactorSpin = new QDoubleSpinBox;
    m_damageFactorSpin->setRange(0, 1);
    m_damageFactorSpin->setValue(m_damageFactor);
    connect(m_damageFactorSpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), [this](double v){ m_damageFactor = v; });
    vlay->addWidget(m_damageFactorSpin);

    QPushButton* resetBtn = new QPushButton("Reiniciar");
    connect(resetBtn, &QPushButton::clicked, this, &MainWindow::resetGame);
    vlay->addWidget(resetBtn);

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
    m_angleLabel->setText(QString("Ángulo: %1°").arg(int(m_angle)));
}

void MainWindow::updateSpeedLabel() {
    m_speedLabel->setText(QString("Velocidad: %1").arg(int(m_speed)));
}

void MainWindow::buildInterface() {
    m_scene->clear();

    // Suelo
    QGraphicsRectItem* ground = new QGraphicsRectItem(0, m_sceneRect.height() - 20, m_sceneRect.width(), 20);
    ground->setBrush(QBrush(Qt::darkGreen));
    m_scene->addItem(ground);

    // Cañones (simples rectángulos)
    QGraphicsRectItem* cannonLeft = new QGraphicsRectItem(20, m_sceneRect.height() - 60, 60, 40);
    cannonLeft->setBrush(QBrush(Qt::blue));
    m_scene->addItem(cannonLeft);
    m_cannonLeftMuzzle = QPointF(80, m_sceneRect.height() - 60);

    QGraphicsRectItem* cannonRight = new QGraphicsRectItem(m_sceneRect.width() - 80, m_sceneRect.height() - 60, 60, 40);
    cannonRight->setBrush(QBrush(Qt::red));
    m_scene->addItem(cannonRight);
    m_cannonRightMuzzle = QPointF(m_sceneRect.width() - 20, m_sceneRect.height() - 60);

    // Stickmen
    drawStickman(50, m_sceneRect.height() - 80);
    drawStickman(m_sceneRect.width() - 50, m_sceneRect.height() - 80);

    // Obstáculos izquierda
    for (int i = 0; i < 5; ++i) {
        QRectF r(200 + i * 50, m_sceneRect.height() - 20 - (i + 1) * 30, 40, 30);
        Obstaculo* ob = new Obstaculo(r, 100.0 * (i + 1));
        m_scene->addItem(ob);
        m_obstaclesLeft.append(ob);
    }

    // Obstáculos derecha
    for (int i = 0; i < 5; ++i) {
        QRectF r(m_sceneRect.width() - 240 - i * 50, m_sceneRect.height() - 20 - (i + 1) * 30, 40, 30);
        Obstaculo* ob = new Obstaculo(r, 100.0 * (i + 1));
        m_scene->addItem(ob);
        m_obstaclesRight.append(ob);
    }
}

void MainWindow::drawStickman(double cx, double cy) {
    m_scene->addEllipse(cx - 10, cy - 30, 20, 20, QPen(Qt::black), QBrush(Qt::yellow)); // Cabeza
    m_scene->addLine(cx, cy - 10, cx, cy + 30, QPen(Qt::black)); // Cuerpo
    m_scene->addLine(cx - 15, cy, cx + 15, cy, QPen(Qt::black)); // Brazos
    m_scene->addLine(cx - 10, cy + 50, cx, cy + 30, QPen(Qt::black)); // Pierna izq
    m_scene->addLine(cx + 10, cy + 50, cx, cy + 30, QPen(Qt::black)); // Pierna der
}

void MainWindow::handleWallCollision(Proyectil* p) {
    QRectF prect = p->boundingRect().translated(p->pos());
    QPointF v = p->velocidad();

    if (prect.left() < 0 || prect.right() > m_sceneRect.width()) {
        v.setX(-m_restitution * v.x());
        p->setVelocidad(v);
    }
    if (prect.top() < 0) {
        v.setY(-m_restitution * v.y());
        p->setVelocidad(v);
    }
    if (prect.bottom() > m_sceneRect.height()) {
        removeProjectile();
        nextTurn();
    }
}

void MainWindow::handleObstaclesCollision(Proyectil* p) {
    checkCollisionList(p, m_obstaclesLeft);
    checkCollisionList(p, m_obstaclesRight);
}

void MainWindow::checkCollisionList(Proyectil* p, QVector<Obstaculo*>& list) {
    QRectF prect = p->boundingRect().translated(p->pos());
    QPointF v = p->velocidad();
    double speed = std::hypot(v.x(), v.y());
    double damage = m_damageFactor * p->masa() * speed;

    for (Obstaculo* ob : list) {
        if (ob->destroyed()) continue;
        QRectF obRect = ob->boundingRect().translated(ob->pos());

        if (prect.intersects(obRect)) {
            QPointF diff = p->pos() - ob->pos();
            ob->applyDamage(damage);
            qDebug() << "Impacto: daño=" << damage << "resta=" << ob->resistance();

            if (std::abs(diff.x()) > std::abs(diff.y())) {
                v.setX(-m_restitution * v.x());
            } else {
                v.setY(-m_restitution * v.y());
            }
            p->setVelocidad(v);

            if (std::abs(diff.x()) > std::abs(diff.y())) {
                p->setPos(diff.x() > 0 ? obRect.right() + p->radio() + 1 : obRect.left() - p->radio() - 1, p->pos().y());
            } else {
                p->setPos(p->pos().x(), diff.y() > 0 ? obRect.bottom() + p->radio() + 1 : obRect.top() - p->radio() - 1);
            }

            if (ob->destroyed()) {
                qDebug() << "Bloque destruido!";
                if (checkVictory()) showVictoryDialog();
            }
            break;
        }
    }
}

bool MainWindow::checkVictory() {
    bool leftAll = std::all_of(m_obstaclesLeft.begin(), m_obstaclesLeft.end(), [](Obstaculo* o){ return o->destroyed(); });
    bool rightAll = std::all_of(m_obstaclesRight.begin(), m_obstaclesRight.end(), [](Obstaculo* o){ return o->destroyed(); });
    return leftAll || rightAll;
}

void MainWindow::showVictoryDialog() {
    bool leftAll = std::all_of(m_obstaclesLeft.begin(), m_obstaclesLeft.end(), [](Obstaculo* o){ return o->destroyed(); });
    bool rightAll = std::all_of(m_obstaclesRight.begin(), m_obstaclesRight.end(), [](Obstaculo* o){ return o->destroyed(); });
    int winner = leftAll ? 2 : (rightAll ? 1 : -1);

    if (winner != -1) {
        QMessageBox::information(this, "Victoria", QString("¡Jugador %1 gana!").arg(winner));
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
    for (Obstaculo* o : m_obstaclesLeft) { m_scene->removeItem(o); delete o; }
    for (Obstaculo* o : m_obstaclesRight) { m_scene->removeItem(o); delete o; }
    m_obstaclesLeft.clear();
    m_obstaclesRight.clear();

    buildInterface();
    removeProjectile();
    m_currentPlayer = 1;
    m_angle = 45.0;
    updateAngleLabel();
    updateTurnLabel();
}
