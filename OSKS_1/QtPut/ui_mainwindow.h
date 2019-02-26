/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QWidget>
#include "ringsprogressbar.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    RingsProgressbar *bar3;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_total;
    QLabel *label_done;
    QLabel *label_4;
    QLabel *label_6;
    QLabel *label_7;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(742, 573);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        bar3 = new RingsProgressbar(centralWidget);
        bar3->setObjectName(QStringLiteral("bar3"));
        bar3->setGeometry(QRect(350, 280, 200, 200));
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(80, 270, 231, 61));
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(80, 110, 129, 34));
        label_3 = new QLabel(centralWidget);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(80, 190, 129, 34));
        label_total = new QLabel(centralWidget);
        label_total->setObjectName(QStringLiteral("label_total"));
        label_total->setGeometry(QRect(280, 110, 129, 34));
        label_done = new QLabel(centralWidget);
        label_done->setObjectName(QStringLiteral("label_done"));
        label_done->setGeometry(QRect(280, 190, 129, 34));
        label_4 = new QLabel(centralWidget);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(500, 110, 129, 34));
        label_6 = new QLabel(centralWidget);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(500, 180, 129, 34));
        label_7 = new QLabel(centralWidget);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(80, 20, 129, 34));
        MainWindow->setCentralWidget(centralWidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
        label->setText(QApplication::translate("MainWindow", "Progress :", nullptr));
        label_2->setText(QApplication::translate("MainWindow", "Total :", nullptr));
        label_3->setText(QApplication::translate("MainWindow", "Done:", nullptr));
        label_total->setText(QApplication::translate("MainWindow", "TextLabel", nullptr));
        label_done->setText(QApplication::translate("MainWindow", "TextLabel", nullptr));
        label_4->setText(QApplication::translate("MainWindow", "KiB", nullptr));
        label_6->setText(QApplication::translate("MainWindow", "KiB", nullptr));
        label_7->setText(QApplication::translate("MainWindow", "Put:", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
