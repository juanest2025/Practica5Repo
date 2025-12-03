/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QGraphicsView *graphicsView;
    QPushButton *Disparar;
    QPushButton *AumentarAngulo;
    QPushButton *DisminuirAngulo;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1305, 703);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        graphicsView = new QGraphicsView(centralwidget);
        graphicsView->setObjectName("graphicsView");
        graphicsView->setGeometry(QRect(15, 21, 1021, 641));
        Disparar = new QPushButton(centralwidget);
        Disparar->setObjectName("Disparar");
        Disparar->setGeometry(QRect(1110, 130, 90, 29));
        AumentarAngulo = new QPushButton(centralwidget);
        AumentarAngulo->setObjectName("AumentarAngulo");
        AumentarAngulo->setGeometry(QRect(1100, 210, 121, 29));
        DisminuirAngulo = new QPushButton(centralwidget);
        DisminuirAngulo->setObjectName("DisminuirAngulo");
        DisminuirAngulo->setGeometry(QRect(1100, 280, 131, 29));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1305, 25));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        Disparar->setText(QCoreApplication::translate("MainWindow", "Disparar", nullptr));
        AumentarAngulo->setText(QCoreApplication::translate("MainWindow", "AumentarAngulo", nullptr));
        DisminuirAngulo->setText(QCoreApplication::translate("MainWindow", "DisminuirAngulo", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
