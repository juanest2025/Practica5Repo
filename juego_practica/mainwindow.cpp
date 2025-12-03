#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPen>
#include <QBrush>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->Disparar, &QPushButton::clicked, this, &MainWindow::on_Disparar_clicked);
    connect(ui->AumentarAngulo, &QPushButton::clicked, this, &MainWindow::on_AumentarAngulo_clicked);
    connect(ui->DisminuirAngulo, &QPushButton::clicked, this, &MainWindow::on_DisminuirAngulo_clicked);


    // Crear escena
    scene = new QGraphicsScene(this);
    scene->setSceneRect(0, 0, 1000, 600);

    // ============================
    // CAJAS
    // ============================
    rect_1 = new QGraphicsRectItem(0, 500, 40, 100);
    rect_2 = new QGraphicsRectItem(0, 460, 180, 40);
    rect_3 = new QGraphicsRectItem(140, 500, 40, 100);


    int right_x = 1000 - 40;
    rect_4 = new QGraphicsRectItem(right_x, 500, 40, 100);
    rect_5 = new QGraphicsRectItem(right_x - 140, 460, 180, 40);
    rect_6 = new QGraphicsRectItem(right_x - 140, 500, 40, 100);


    QList<QGraphicsRectItem*> cajas = { rect_1, rect_2, rect_3, rect_4, rect_5, rect_6 };

    for (auto c : cajas)
    {
        c->setBrush(Qt::yellow);
        c->setData(0, "cajas");
        scene->addItem(c);
    }

    // ============================
    // CAÑÓN 1 (IZQUIERDA)
    // ============================
    cano1_plataforma = new QGraphicsRectItem(20, 420, 100, 12);
    cano1_plataforma->setBrush(Qt::blue);
    cano1_plataforma->setData(0, "estructura");
    scene->addItem(cano1_plataforma);

    cano1_cuerpo = new QGraphicsEllipseItem(20, 390, 100, 40);
    cano1_cuerpo->setBrush(Qt::white);
    cano1_cuerpo->setData(0, "estructura");
    scene->addItem(cano1_cuerpo);

    cano1_tubo = new QGraphicsRectItem(100, 405, 60, 15);
    cano1_tubo->setBrush(Qt::black);
    cano1_tubo->setData(0, "estructura");
    scene->addItem(cano1_tubo);

    // ============================
    // CAÑÓN 2 (DERECHA)
    // ============================
    int baseX = 1000 - 20 - 100;

    cano2_plataforma = new QGraphicsRectItem(baseX, 420, 100, 12);
    cano2_plataforma->setBrush(Qt::blue);
    cano2_plataforma->setData(0, "estructura");
    scene->addItem(cano2_plataforma);

    cano2_cuerpo = new QGraphicsEllipseItem(baseX, 390, 100, 40);
    cano2_cuerpo->setBrush(Qt::green);
    cano2_cuerpo->setData(0, "estructura");
    scene->addItem(cano2_cuerpo);

    cano2_tubo = new QGraphicsRectItem(baseX - 60, 405, 60, 15);
    cano2_tubo->setBrush(Qt::black);
    cano2_tubo->setData(0, "estructura");
    scene->addItem(cano2_tubo);

    // ============================
    // PISO INVISIBLE
    // ============================
    QGraphicsRectItem *piso = new QGraphicsRectItem(0, 580, 1000, 20);
    piso->setBrush(Qt::transparent);
    piso->setPen(Qt::NoPen);
    piso->setData(0, "suelo");
    scene->addItem(piso);

    // ============================
    // SPRITES
    // ============================
    QPixmap px1("./rival_1.png");
    QPixmap px2("./rival_2.png");

    sprite_1 = new QGraphicsPixmapItem(px1.scaled(70,70));
    sprite_2 = new QGraphicsPixmapItem(px2.scaled(70,70));

    sprite_1->setPos(60, 520);
    sprite_2->setPos(1000 - 60 - 70, 520);

    scene->addItem(sprite_1);
    scene->addItem(sprite_2);

    ui->graphicsView->setScene(scene);
}

MainWindow::~MainWindow()
{
    delete ui;
}




void MainWindow::on_AumentarAngulo_clicked()
{
    angulo += 5;
    if (angulo > 80) angulo = 80;   // límite para no apuntar raro
}


void MainWindow::on_DisminuirAngulo_clicked()
{
    angulo -= 5;
    if (angulo < 5) angulo = 5;
}



void MainWindow::on_Disparar_clicked()
{
    if (juegoTerminado) return;    // bloquear si juego terminó
    if (proyectil) return;         // ya hay uno en vuelo (si quieres permitir varios, adapta)

    double vx, vy;

    double rad = qDegreesToRadians(angulo);
    double velocidad = 18.0;

    int owner = 1; // 1 = izquierda, 2 = derecha

    if (turnoIzquierda) {
        owner = 1;
        vx = velocidad * cos(rad);
        vy = -velocidad * sin(rad);
    }
    else {
        owner = 2;
        vx = -velocidad * cos(rad);
        vy = -velocidad * sin(rad);
    }

    // crear proyectil
    proyectil = new Proyectil(vx, vy, 0.4, owner);

    // calcular salida y desplazar fuera del tubo para evitar colisión inmediata
    QPointF salida;
    if (turnoIzquierda) {
        salida = cano1_tubo->mapToScene(
            cano1_tubo->rect().right(),
            cano1_tubo->rect().center().y()
            );
    } else {
        salida = cano2_tubo->mapToScene(
            cano2_tubo->rect().left(),
            cano2_tubo->rect().center().y()
            );
    }

    // normalizar dirección y separar un poco
    double speed = qSqrt(vx*vx + vy*vy);
    QPointF dir(0,0);
    if (speed != 0.0) dir = QPointF(vx / speed, vy / speed);
    double separacion = 18.0; // ajusta si el proyectil queda dentro, aumentar
    QPointF posFinal = salida + dir * separacion;
    proyectil->setPos(posFinal);

    // conexiones: primero señales del proyectil
    connect(proyectil, &Proyectil::danioEstructura, this,
            [=](int jugador, int danio) {
                qDebug() << "MainWindow: recibir danio jugador" << jugador << "danio" << danio;
                aplicarDanio(jugador, danio);
            });

    // conectar destrucción (usa deleteLater por seguridad)
    connect(proyectil, &Proyectil::destruir, this, [=]() {
        if (!proyectil) return;
        scene->removeItem(proyectil);
        proyectil->deleteLater();
        proyectil = nullptr;
        qDebug() << "Proyectil destruido";
    });

    // añadir a la escena al final
    scene->addItem(proyectil);

    turnoIzquierda = !turnoIzquierda;

}


void MainWindow::aplicarDanio(int jugador, int danio)
{
    if (jugador == 1) {
        vidaEstructura1 -= danio;
        qDebug() << "Vida Jugador 1:" << vidaEstructura1;
        if (vidaEstructura1 <= 0) verificarGanador();
    }
    else {
        vidaEstructura2 -= danio;
        qDebug() << "Vida Jugador 2:" << vidaEstructura2;
        if (vidaEstructura2 <= 0) verificarGanador();
    }
}


void MainWindow::verificarGanador()
{
    if (juegoTerminado) return;  // evita ejecuciones repetidas

    if (vidaEstructura1 <= 0) {
        juegoTerminado = true;
        QMessageBox::information(this, "Resultado", "¡Jugador 2 gana!");
        return;
    }

    if (vidaEstructura2 <= 0) {
        juegoTerminado = true;
        QMessageBox::information(this, "Resultado", "¡Jugador 1 gana!");
        return;
    }
}


