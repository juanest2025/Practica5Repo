#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsPixmapItem>
#include <QTimer>
#include "proyectil.h"
#include <QtMath>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_Disparar_clicked();

    void on_AumentarAngulo_clicked();

    void on_DisminuirAngulo_clicked();

private:
    bool turnoIzquierda = true; // true = cañón 1, false = cañón 2
    Ui::MainWindow *ui;
    QGraphicsScene *scene;

    // Cajas
    QGraphicsRectItem *rect_1;
    QGraphicsRectItem *rect_2;
    QGraphicsRectItem *rect_3;
    QGraphicsRectItem *rect_4;
    QGraphicsRectItem *rect_5;
    QGraphicsRectItem *rect_6;

    // Cañón 1
    QGraphicsRectItem *cano1_plataforma;
    QGraphicsEllipseItem *cano1_cuerpo;
    QGraphicsRectItem *cano1_tubo;

    // Cañón 2
    QGraphicsRectItem *cano2_plataforma;
    QGraphicsEllipseItem *cano2_cuerpo;
    QGraphicsRectItem *cano2_tubo;

    // Sprites
    QGraphicsPixmapItem *sprite_1;
    QGraphicsPixmapItem *sprite_2;

    // Proyectil
    Proyectil *proyectil = nullptr;

    //Ángulo
    double angulo = 45.0;

    // Sistema de vida
    int vidaEstructura1 = 100;
    int vidaEstructura2 = 100;
    bool juegoTerminado = false;

    void aplicarDanio(int jugador, int danio);
    void verificarGanador();
    void reiniciarJuego();


};

#endif // MAINWINDOW_H

