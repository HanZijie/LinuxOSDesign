#include "mainwindow.h"
#include "ui_mainwindow.h"

int table_Widget = 0;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    kill[0] = '\0';

    m_timer = new QTimer(this);
    m_timer_fast = new QTimer(this);
    m_timer_shut = new QTimer(this);
    connect(m_timer,SIGNAL(timeout()),this,SLOT(UpdMiddle()));
    connect(m_timer_fast,SIGNAL(timeout()),this,SLOT(UpdStatus()));
    connect(m_timer_shut,SIGNAL(timeout()),this,SLOT(shutdown()));
    m_timer_fast->start(500);
    m_timer->start(10000);

    lbltime = new QLabel();
    lblcpu = new QLabel();
    lblmem = new QLabel;
    ui->statusBar->addWidget(lbltime,1);
    ui->statusBar->addWidget(lblcpu,1);
    ui->statusBar->addWidget(lblmem,1);
    ui->label_shutdown->hide();

    //初始化图表
    maxSize = 120;
    m_chart = new QChart;
    chartView = new QChartView(m_chart);
    chartView->setRubberBand(QChartView::RectangleRubberBand);

    m_chart1 = new QChart;
    chartView1 = new QChartView(m_chart1);
    chartView1->setRubberBand(QChartView::RectangleRubberBand);

    m_chart2 = new QChart;
    chartView2 = new QChartView(m_chart2);
    chartView2->setRubberBand(QChartView::RectangleRubberBand);

    series0 = new QLineSeries;
    m_chart->addSeries(series0);

    series1 = new QLineSeries;
    m_chart1->addSeries(series1);

    series2 = new QLineSeries;
    m_chart2->addSeries(series2);

    for(int i=0;i<maxSize;++i)
    {
      series0->append(0,0);
      series1->append(0,0);
      series2->append(0,0);
    }
    series0->setUseOpenGL(true);//openGl 加速
    series1->setUseOpenGL(true);
    series2->setUseOpenGL(true);

    axisX = new QValueAxis;
    axisX->setRange(0,maxSize);
    axisX->setLabelFormat("%g");
    //axisX->setTitleText("时间/s");

    axisX1 = new QValueAxis;
    axisX1->setRange(0,maxSize);
    axisX1->setLabelFormat("%g");
    //axisX1->setTitleText("时间/s");

    axisX2 = new QValueAxis;
    axisX2->setRange(0,maxSize);
    axisX2->setLabelFormat("%g");
    //axisX2->setTitleText("时间/s");

    axisY = new QValueAxis;
    axisY->setRange(0,100);
    //axisY->setTitleText("CPU使用率");

    axisY1 = new QValueAxis;
    axisY1->setRange(0,100);
    //axisY1->setTitleText("内存使用率");

    axisY2 = new QValueAxis;
    axisY2->setRange(0,100);
    //axisY2->setTitleText("交换分区使用率");

    m_chart->setAxisX(axisX,series0);
    m_chart->setAxisY(axisY,series0);
    m_chart->legend()->hide();
    //m_chart->setTitle("2分钟内CPU使用率图像");
    ui->verticalLayout->addWidget(chartView);

    m_chart1->setAxisX(axisX1,series1);
    m_chart1->setAxisY(axisY1,series1);
    m_chart1->legend()->hide();
    //m_chart1->setTitle("2分钟内内存使用率图像");
    ui->verticalLayout_2->addWidget(chartView1);

    m_chart2->setAxisX(axisX2,series2);
    m_chart2->setAxisY(axisY2,series2);
    m_chart2->legend()->hide();
    //m_chart2->setTitle("2分钟内SWAP使用率图像");
    ui->verticalLayout_3->addWidget(chartView2);


    //ui->tableWidget->horizontalHeader()->setDefaultSectionSize(150);
    //设置表头内容
    QStringList header;
    header<<"进程号"<<"进程名"<<"状态"<<"父进程号"<<"优先级"<<"内存占用";
    ui->tableWidget->setColumnCount(6);
    ui->tableWidget->setHorizontalHeaderLabels(header);


    ui->tableWidget->verticalHeader()->setStretchLastSection(true);
    ui->tableWidget->verticalHeader()->setDefaultSectionSize(10); //设置行高
    ui->tableWidget->setFrameShape(QFrame::NoFrame); //设置无边框
    ui->tableWidget->setShowGrid(false); //设置不显示格子线

    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);  //设置选择行为时每次选择一行
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers); //设置不可编辑
    ui->tableWidget->horizontalHeader()->resizeSection(0,120); //设置表头第二列的宽度为150
    ui->tableWidget->horizontalHeader()->resizeSection(1,380); //设置表头第二列的宽度为150
    ui->tableWidget->horizontalHeader()->resizeSection(2,200); //设置表头第二列的宽度为150
    ui->tableWidget->horizontalHeader()->resizeSection(3,240); //设置表头第二列的宽度为150
    ui->tableWidget->horizontalHeader()->resizeSection(4,210); //设置表头第二列的宽度为150
    ui->tableWidget->horizontalHeader()->resizeSection(5,280); //设置表头第二列的宽度为150

    ui->tableWidget->setStyleSheet("selection-background-color:lightblue;"); //设置选中背景色
    ui->tableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;}"); //设置表头背景色
    UpdMiddle();
    InitBasic();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::InitBasic(){
    //zhujiming
    char hostname[50];
    int fd = open("/proc/sys/kernel/hostname",O_RDONLY);
    int len = read(fd,hostname,49);
    hostname[len] = '\0';
    ::close(fd);
    ui->line_host->setText("  "+ QString(hostname));
    //系统版本号
    char version[50],ostype[30];
    FILE *fp2 = fopen("/proc/sys/kernel/osrelease","r");
    FILE *fp3 = fopen("/proc/sys/kernel/ostype","r");
    fgets(version,49,fp2);
    fgets(ostype,29,fp3);
    fclose(fp2);
    fclose(fp3);
    ui->line_version->setText("  "+ QString(ostype) + QString(version));

    //系统启动时间
    char launch[50];
    FILE *fp4 = fopen("/proc/uptime","r");
    fgets(launch,49,fp4);
    fclose(fp4);
    long nowtime = 0,launtime = 0;
    runtime = 0;
    for(int i=0;launch[i] != '.';++i)
    {
        runtime = (launch[i] - '0') + runtime * 10;
    }

    struct tm cut;
    nowtime = time(NULL);
    launtime = nowtime - runtime;
    localtime_r(&launtime, &cut);
    char timef[100];
    sprintf(timef,"%04d/%02d/%02d %02d:%02d:%02d",cut.tm_year+1900,
            cut.tm_mon+1,cut.tm_mday,cut.tm_hour,
            cut.tm_min,cut.tm_sec);
    ui->line_launch->setText("  "+ QString::fromLatin1(timef));

    //cpu型号
    char cpuinfo[100];
    FILE *fp5 = fopen("/proc/cpuinfo","r");
    for(int i=0;i<5;++i)
        fgets(cpuinfo,99,fp5);
    int i=0;
    for(;cpuinfo[i]!=':';++i);
    ui->line_cpu->setText("  "+QString(cpuinfo+i+2));
    //主频
    for(int i=0;i<3;++i)
        fgets(cpuinfo,99,fp5);
    fclose(fp5);
    char bufhz[20];
    double mhz;
    sscanf(cpuinfo,"%s%s%s%lf",bufhz,bufhz,bufhz,&mhz);
    sprintf(bufhz,"  %.3lf MHz",mhz);
    ui->line_cpu_2->setText(QString(bufhz));
}

void MainWindow::UpdStatus(){
    //状态栏当前时间
    struct tm cut;
    char timef[100];
    long nowtime = time(NULL);
    localtime_r(&nowtime,&cut);
    sprintf(timef,"%04d/%02d/%02d %02d:%02d:%02d",cut.tm_year+1900,
            cut.tm_mon+1,cut.tm_mday,cut.tm_hour,
            cut.tm_min,cut.tm_sec);
    lbltime->setText(timef);
    //系统运行时间
    long hour,minu,sec;
    hour = runtime/3600;
    minu = (runtime%3600)/60;
    sec = (runtime%3600) % 60;
    runtime++;
    ui->line_run->setText("  "+ QString::number(hour)+"  时   "+QString::number(minu)
                          +"  分   "+QString::number(sec)+"  秒");
    FILE *fp;
    char buf[128];
    char cpu[5];
    int user,nice,sys,idle,iowait,irq,softirq;
    double usage;

    fp = fopen("/proc/stat","r");
    if(fp == NULL)
    {
       perror("fopen:");
       return;
    }
    fgets(buf,sizeof(buf),fp);
    sscanf(buf,"%s%d%d%d%d%d%d%d",cpu,&user,&nice,&sys,&idle,&iowait,&irq,&softirq);
    fclose(fp);
    if(tag)
    {
       tag = false;
       all1 = user+nice+sys+idle+iowait+irq+softirq;
       idle1 = idle;
    }
    else
    {
       tag = true;
       //cpu利用率
       /*第二次取数据*/
       all2 = user+nice+sys+idle+iowait+irq+softirq;
       idle2 = idle;
       usage = (double)(all2-all1-(idle2-idle1)) / (all2-all1)*100;

       //把数据前移一个单位，如果点的数量太多，这里消耗的时间也会很多
       QVector<QPointF> Points = series0->pointsVector();
       int i;
       for(i = 0;i< Points.size() - 1;i++)
       {
           Points[i] = QPoint(i+1,Points[i + 1].y());
       }
       Points[Points.size() - 1] = QPoint(Points.size(),int(usage)+1); //添加最新的点
       series0->replace(Points); //利用replace函数更新数据
       char text[30];
       sprintf(text,"CPU使用率：%.2lf%%",usage);
       lblcpu->setText(text);

       //内存利用率
       FILE *fp1;
       char buf1[60],buf2[60],tmp[25];
       double usagemem;

       fp1 = fopen("/proc/meminfo","r");
       if(fp1 == NULL)
       {
           perror("fopen:");
           return;
       }
       fgets(buf1,sizeof(buf1),fp1);
       fgets(buf2,sizeof(buf2),fp1);
       sscanf(buf1,"%s%d",tmp,&memTotal);
       sscanf(buf2,"%s%d",tmp,&memFree);
       usagemem = (double)(memTotal - memFree) / memTotal*100;

       //把数据前移一个单位，如果点的数量太多，这里消耗的时间也会很多
       Points = series1->pointsVector();
       for(i = 0;i< Points.size() - 1;i++)
       {
           Points[i] = QPoint(i+1,Points[i + 1].y());
       }
       Points[Points.size() - 1] = QPoint(Points.size(),int(usagemem)+1); //添加最新的点
       series1->replace(Points); //利用replace函数更新数据

       sprintf(text,"内存使用率：%d KB/%d KB %.2lf%%",memTotal-memFree,memTotal,usagemem);
       lblmem->setText(text);

       //swap利用率
       double usageswap;
       for(int i=0;i<13;++i)
           fgets(buf1,sizeof(buf1),fp1);
       fgets(buf2,sizeof(buf2),fp1);
       sscanf(buf1,"%s%d",tmp,&swapTotal);
       sscanf(buf2,"%s%d",tmp,&swapFree);
       fclose(fp1);
       usageswap = (double)(swapTotal - swapFree) / swapTotal*100;

       //把数据前移一个单位，如果点的数量太多，这里消耗的时间也会很多
       Points = series2->pointsVector();
       for(i = 0;i< Points.size() - 1;i++)
       {
           Points[i] = QPoint(i+1,Points[i + 1].y());
       }
       Points[Points.size() - 1] = QPoint(Points.size(),int(usageswap)+1); //添加最新的点
       series2->replace(Points); //利用replace函数更新数据
    }
}

void MainWindow::UpdMiddle()
{
    if(table_Widget){
        return;
    }
    qDebug()<<"begin"<<endl;
    DIR *dp;
    struct dirent *entry;
    struct stat statbuf;
    if( (dp = opendir("/proc") ) ==NULL)
    {
        printf("failing to opendir\n");
        return;
    }
    int j=0;
    ui->tableWidget->clearContents();
    while( entry=readdir(dp) )
    {
        lstat(entry->d_name,&statbuf);
        int len = strlen(entry->d_name);
        int i;
        for(i=0;i<len;++i)
        {
            if(entry->d_name[i]>'9' || entry->d_name[i]<'0')
                break;
        }
        if(i<len) continue;
        if(S_ISDIR(statbuf.st_mode))
        {
            int pid,ppid,dynice,i,memused;
            char proc[30],procinfo[200],state[3];
            char path[30];

            //打开/proc/pid/stat
            sprintf(path,"/proc/%s/stat",entry->d_name);
            FILE *fp = fopen(path,"r");
            fgets(procinfo,199,fp);
            fclose(fp);
            sscanf(procinfo,"%d%s%s%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d",
             &pid,proc,state,&ppid,&i,&i,&i,&i,&i,&i,&i,&i,&i,&i,&i,&i,&i,&dynice);

            //打开/proc/pid/statm
            sprintf(path,"/proc/%s/statm",entry->d_name);
            fp = fopen(path,"r");
            fgets(procinfo,50,fp);
            fclose(fp);
            sscanf(procinfo,"%d%d",&i,&memused);

            ui->tableWidget->setItem(j,0,new QTableWidgetItem(QString::number(pid)));
            QString Proc;
            for(int i=1;i<strlen(proc)-1;++i)
            {
                Proc += QString(proc[i]);
            }
            ui->tableWidget->setItem(j,1,new QTableWidgetItem(QString(Proc)));
            QString st;
            switch (state[0]) {
            case 'S':
                st = "睡眠";
                break;
            case 'R':
                st = "运行";
                break;
            case 'D':
                st = "Disk Sleep";
                break;
            case 'T':
                st = "Tracing Stop";
                break;
            case 'Z':
                st = "Zombie";
                break;
            case 'X':
                st = "Dead";
                break;
            default:
                st = "Stopped";
                break;
            }
            ui->tableWidget->setItem(j,2,new QTableWidgetItem(st));
            ui->tableWidget->setItem(j,3,new QTableWidgetItem(QString::number(ppid)));
            ui->tableWidget->setItem(j,4,new QTableWidgetItem(QString::number(dynice)));
            ui->tableWidget->setItem(j,5,new QTableWidgetItem(QString::number(memused)));
            ++j;
        }
    }
    ui->tableWidget->setRowCount(j);
    closedir(dp);
    repaint();
}

void MainWindow::on_tabWidget_currentChanged(int index)
{
    table_Widget = index;
}

void MainWindow::on_pushButton_query_clicked()
{
    QString qstrpid = ui->lineEdit_query->text();
    bool ok;
    qstrpid.toInt(&ok);
    if(ok){
        DIR *dp;
        bool tag = 0;
        struct dirent *entry;
        struct stat statbuf;
        if( (dp = opendir("/proc") ) ==NULL)
        {
           printf("failing to opendir\n");
           return;
        }
        int pid,ppid,dynice,i,memused;
        char proc[30],procinfo[200],state[3];
        char path[30];
        while( entry=readdir(dp) )
        {
           lstat(entry->d_name,&statbuf);
           if(qstrpid==QString(entry->d_name))
           {
               tag = 1;

               //打开/proc/pid/stat
               sprintf(path,"/proc/%s/stat",entry->d_name);
               FILE *fp = fopen(path,"r");
               fgets(procinfo,199,fp);
               fclose(fp);
               sscanf(procinfo,"%d%s%s%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d",
                &pid,proc,state,&ppid,&i,&i,&i,&i,&i,&i,&i,&i,&i,&i,&i,&i,&i,&dynice);

               //打开/proc/pid/statm
               sprintf(path,"/proc/%s/statm",entry->d_name);
               fp = fopen(path,"r");
               fgets(procinfo,50,fp);
               fclose(fp);
               sscanf(procinfo,"%d%d",&i,&memused);

               QString Proc;
               for(int i=1;i<strlen(proc)-1;++i)
               {
                   Proc += QString(proc[i]);
               }
               QString st;
               switch (state[0]) {
               case 'S':
                   st = "睡眠";
                   break;
               case 'R':
                   st = "运行";
                   break;
               case 'D':
                   st = "Disk Sleep";
                   break;
               case 'T':
                   st = "Tracing Stop";
                   break;
               case 'Z':
                   st = "Zombie";
                   break;
               case 'X':
                   st = "Dead";
                   break;
               default:
                   st = "Stopped";
                   break;
               }
               QString text;
               text = "进程名：    "+Proc+"         \n父进程：    "+QString::number(ppid)+"         \n状态：    "+st+"         \n优先级：    "+
                       QString::number(dynice)+"         \n内存占用：    "+QString::number(memused)+"";
               QMessageBox box;
               box.setText(text);
               QFont font;
               font.setPixelSize(35);
               box.setFont(font);
               box.setFixedSize(1200, 700);
               int bret = box.exec();
               ui->lineEdit_query->clear();
               break;
           }
        }
        if(tag==0)
           QMessageBox::about(this,"提示","未找到该进程。");
        closedir(dp);
    }else{
        QString qstrname = qstrpid;
        DIR *dp;
        bool tag = 0;
        struct dirent *entry;
        struct stat statbuf;
        if( (dp = opendir("/proc") ) ==NULL)
        {
            printf("failing to opendir\n");
            return;
        }
        int pid,ppid,dynice,i,memused;
        char proc[30],procinfo[200],state[3];
        char path[30];
        while( entry=readdir(dp) )
        {
            lstat(entry->d_name,&statbuf);

            int i,len=strlen(entry->d_name);
            for(i=0;i<len;++i)
            {
                if(entry->d_name[i]>'9' || entry->d_name[i]<'0')
                    break;
            }
            if(i<len || ! S_ISDIR(statbuf.st_mode)) continue;

            //打开/proc/pid/stat
            sprintf(path,"/proc/%s/stat",entry->d_name);
            FILE *fp = fopen(path,"r");
            fgets(procinfo,199,fp);
            fclose(fp);
            sscanf(procinfo,"%d%s%s%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d",
             &pid,proc,state,&ppid,&i,&i,&i,&i,&i,&i,&i,&i,&i,&i,&i,&i,&i,&dynice);

            QString Proc;
            for(int i=1;i<strlen(proc)-1;++i)
            {
                Proc += QString(proc[i]);
            }
            if(Proc==qstrname)
            {
                //打开/proc/pid/statm
                sprintf(path,"/proc/%s/statm",entry->d_name);
                fp = fopen(path,"r");
                fgets(procinfo,50,fp);
                fclose(fp);
                sscanf(procinfo,"%d%d",&i,&memused);

                QString st;
                switch (state[0]) {
                case 'S':
                    st = "睡眠";
                    break;
                case 'R':
                    st = "运行";
                    break;
                case 'D':
                    st = "Disk Sleep";
                    break;
                case 'T':
                    st = "Tracing Stop";
                    break;
                case 'Z':
                    st = "Zombie";
                    break;
                case 'X':
                    st = "Dead";
                    break;
                default:
                    st = "Stopped";
                    break;
                }
                QString text;
                text = "进程号：    "+QString::number(pid)+"         \n父进程：    "+QString::number(ppid)+"         \n状态：    "+st+"         \n优先级：    "+
                        QString::number(dynice)+"         \n内存占用：    "+QString::number(memused);
                QMessageBox box;
                box.setText(text);
                QFont font;
                font.setPixelSize(35);
                box.setFont(font);
                box.setFixedSize(1200, 700);
                int bret = box.exec();
                ui->lineEdit_query->clear();
                tag = 1;
                break;
            }
        }
        if(tag==0)
            QMessageBox::about(this,"提示","未找到该进程。");
        closedir(dp);
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    QString text = ui->lineEdit_create->text();
    int ret = system(text.toStdString().c_str());
        perror(text.toStdString().c_str());
}

void MainWindow::on_pushButton_shut_clicked()
{
    int row = ui->tableWidget->currentRow();
    QTableWidgetItem *item =  ui->tableWidget->item(row,0);
    QString strpid = item->text();
    std::string str = strpid.toStdString();
    int i;
    for(i=0;i<str.length();++i)
    {
        if(str[i]<'0'||str[i]>'9') break;
    }
    if(i==str.length())
    {
        sprintf(kill,"kill %s",str.c_str() );
    }
    else
    {
        kill[0] = '\0';
    }
    qDebug()<<kill;
    if(strlen(kill))
    {
        QMessageBox message(QMessageBox::Warning,"警告",
             "确定杀死进程？",QMessageBox::No|QMessageBox::Yes,NULL);
        message.setDefaultButton(QMessageBox::Yes);
        if (message.exec()==QMessageBox::Yes)
        {
            int ret = system(kill);
            if(ret!= -1)
                QMessageBox::about(this,"提示","进程已结束。");
            else
                QMessageBox::about(this,"提示","进程无法杀死。");
        }
        kill[0] = '\0';
    }
}

void MainWindow::on_pushButton_clicked()
{
    if(shutFlag != 1){
        system("shutdown -h");
        shutFlag = 1;
        shutTime = 60;
        m_timer_shut->start(1000);
        QString strTime = "系统将于60秒后关机";
        ui->label_shutdown->setText(strTime);
        ui->label_shutdown->show();
        ui->pushButton->setText("取消关机");
    }else{
        system("shutdown -c");
        m_timer_shut->stop();
        ui->label_shutdown->hide();
        ui->pushButton->setText("关机");
        shutFlag = 0;
    }
}

void MainWindow::shutdown(){
    shutTime-=1;
    QString strTime = "系统将于"+QString::number((int)shutTime)+"秒后关机";
    qDebug()<<strTime;
    ui->label_shutdown->setText(strTime);
    repaint();
}
