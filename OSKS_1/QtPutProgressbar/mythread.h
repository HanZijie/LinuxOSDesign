#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QObject>
#include <QThread>

class MyThread : public QThread
{
public:
    MyThread();
    void closeThread();

protected:

};

#endif // MYTHREAD_H
