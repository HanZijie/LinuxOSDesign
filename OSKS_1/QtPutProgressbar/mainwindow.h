#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    long fileSize;
    long fileRate;
    ~MainWindow();

public slots:
    void updateProgressbar();

private:
    Ui::MainWindow *ui;
    QTimer *m_timer;
    int m_persent;

};

#endif // MAINWINDOW_H
