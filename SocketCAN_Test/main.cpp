#include "mainwindow.h"
#include "socketcan.h"
#include "canccontroller.h"
#include "QCoreApplication"
#include <QDebug>
#include "linux/can.h"

#include <QApplication>

/*  manage Threads */
#include <QThread>
#include <chrono>




int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();



   


    SocketCAN  can0 ;
    qDebug() << "Test CAN0 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!";


    SocketCAN  can1;;
    qDebug() << "Test CAN1 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!";

    /* _____________________________________________________________________________________________________ */


    /*  to display GUI */
    return a.exec();
}
