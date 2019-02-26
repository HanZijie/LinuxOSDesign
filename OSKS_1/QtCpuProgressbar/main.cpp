#include "mainwindow.h"
#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    qDebug() << "start!\n";
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("CPURate");
    w.show();

    return a.exec();
}
