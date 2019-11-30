#ifndef WORKCONTROL_H
#define WORKCONTROL_H
#include <QObject>
#include <QThread>
#include "workers.h"
#include <QMainWindow>
class workcontrol:public QObject
{

    Q_OBJECT
    public:
        QThread workerThread;
        workcontrol() {
            /*workers *worker = new workers;
            worker->moveToThread(&workerThread);
            connect(&workerThread, &QThread::finished, worker, &QObject::deleteLater);
            connect(this, &workcontrol::operate, worker, &workers::doWork);
            connect(worker, &workers::resultReady, this, &workcontrol::handleResults);
            workerThread.start();*/
        }


        ~workcontrol() {
            workerThread.quit();
            workerThread.wait();
        }
    public slots:
        void handleResults(const QString &);
    signals:
        //void operate(const QString &);
        void operate(ODcollection od,OGRLayer * layer,flowcollection * fc);


//public:
//    workcontrol();
};

#endif // WORKCONTROL_H
