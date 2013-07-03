#include "MyThread.h"
#include <QDebug>


void MyThread::run()
{
    int i;
    i = 1;
    qDebug() << "hello, from a thread!";
}
