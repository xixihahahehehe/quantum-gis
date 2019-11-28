#ifndef WORKERS_H
#define WORKERS_H

#include <QObject>
#include "collection/odcollection.h"
#include "collection/flowcollection.h"
class workers:public QObject
{
    Q_OBJECT
    public:
        //thread data
        ODcollection OD;
        OGRLayer * layer;
        workers();
        workers(ODcollection,OGRLayer *);
        struct tmpodcount
        {
            int _OID;
            int _DID;
        };
    public slots:
        //void doWork(const QString &parameter) {
        //    QString result;
        //    /* ... here is the expensive or blocking operation ... */
        //    emit resultReady(result);
        //}
        void doWork(ODcollection,OGRLayer *,flowcollection *);
    signals:
        void resultReady(const QString &result);

};

#endif // WORKERS_H
