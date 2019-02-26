#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <unistd.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    m_timer = new QTimer(this);
    m_persent = 0;
    connect(m_timer, SIGNAL(timeout()), this, SLOT(updateProgressbar()));

    m_timer->start(2000);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateProgressbar()
{
    FILE *fp;
    char buf[128];
    char cpu[5];
    int user,nice,sys,idle,iowait,irq,softirq;
    int all1,all2,idle1,idle2;
    double usage;
    m_timer->start(2000);
    fp = fopen("/proc/stat","r");
    if(fp == NULL)
    {
        perror("fopen:");
        return;
    }
    fgets(buf,sizeof(buf),fp);
    sscanf(buf,"%s%d%d%d%d%d%d%d",cpu,&user,&nice,&sys,&idle,&iowait,&irq,&softirq);
    all1 = user+nice+sys+idle+iowait+irq+softirq;
    idle1 = idle;
    rewind(fp);
    /*第二次取数据*/
    usleep(80000);
    memset(buf,0,sizeof(buf));
    cpu[0] = '\0';
    user=nice=sys=idle=iowait=irq=softirq=0;
    fgets(buf,sizeof(buf),fp);
    sscanf(buf,"%s%d%d%d%d%d%d%d",cpu,&user,&nice,&sys,&idle,&iowait,&irq,&softirq);
    all2 = user+nice+sys+idle+iowait+irq+softirq;
    idle2 = idle;
    usage = (double)(all2-all1-(idle2-idle1)) / (all2-all1)*100;
    qDebug() << usage;
    fclose(fp);
    ui->bar3->setPersent(int(usage));
    repaint();
}
