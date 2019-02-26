#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <QDebug>

static int shMemoryId;
static long *shDataRate;
static int sem_id;
void P(int semid,int index)
{
    struct sembuf sem;
    sem.sem_num = index;
    sem.sem_op = -1;
    sem.sem_flg = 0; //操作标记：0或IPC_NOWAIT等
    semop(semid,&sem,1);	//1:表示执行命令的个数
    return;
}

void V(int semid,int index)
{
    struct sembuf sem;
    sem.sem_num = index;
    sem.sem_op =  1;
    sem.sem_flg = 0;
    semop(semid,&sem,1);
    return;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    shMemoryId = shmget((key_t)3,0,NULL);
    shDataRate = (long *)shmat(shMemoryId,NULL,0);
    sem_id = semget((key_t)1,3,0666 | IPC_CREAT);

    long a = shDataRate[0];
    a = a/1024;

    ui->setupUi(this);
    ui->label_total->setText(QString::number(a));
    m_timer = new QTimer(this);
    m_persent = 0;
    connect(m_timer, SIGNAL(timeout()), this, SLOT(updateProgressbar()));

    m_timer->start(100);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateProgressbar()
{
    P(sem_id,2);
    fileSize = shDataRate[0];
    fileRate = shDataRate[2];
    float a = fileRate;
    ui->label_done->setText(QString::number(fileRate/1024));
    a = a/fileSize;
    m_persent = (int)(a*100);

    ui->bar3->setPersent(m_persent);
    repaint();
    V(sem_id,2);
}
