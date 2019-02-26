#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QToolBar>
#include <QString>
#include <QTimer>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <QDebug>
#include <iostream>
#include <stdio.h>
#include <string>
#include <time.h>
#include <QLabel>
#include <QTime>
#include <QVector>
#include <QtCharts>
#include <dirent.h>
#include <stdlib.h>
#include <QMessageBox>

QT_CHARTS_USE_NAMESPACE
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void InitBasic();

    QTimer *m_timer,*m_timer_fast,*m_timer_shut;
    QLabel *lbltime,*lblcpu,*lblmem;
    QChart *m_chart,*m_chart1,*m_chart2;
    QLineSeries *series0,*series1,*series2;
    QChartView *chartView,*chartView1,*chartView2;
    QValueAxis *axisX,*axisY,*axisX1,*axisY1,*axisX2,*axisY2;

    long runtime;
    char kill[50];
    int maxSize;
    int yrange;
    bool tag;
    int all1,all2,idle1,idle2;
    int memTotal,memFree;
    int swapTotal,swapFree;
    int shutFlag;
    int shutTime;
    ~MainWindow();

public slots:
    void UpdMiddle();
    void UpdStatus();
    void shutdown();

private slots:
    void on_tabWidget_currentChanged(int index);

    void on_pushButton_query_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_shut_clicked();

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
