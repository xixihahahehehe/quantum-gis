#ifndef FLOWCOLLECTION_H
#define FLOWCOLLECTION_H
#include "geometries/flowdata.h"
#include "ogrsf_frmts.h"
#include <vector>
#include <fstream>
#include <cstdlib>
#include <math.h>
#include "odcollection.h"
//UI related
#include <QPainter>
#include <QPoint>
#include <QPolygon>
#include <QVector>
#include <QPen>
#include <QColor>

using namespace std;
class flowcollection
{
private:
    double _zoomx;
    double _zoomy;
    double _scalex;
    double _scaley;
    double max_weight;
    double min_weight;
    double _margin=0.05;
    QColor _qcolor;
    OGREnvelope *_envelope;
    QPoint geo2screen(double _geox,double _geoy,const QRect& paintingRect)
    {
        double x=(_geox+_zoomx)*_scalex;
        double y=(_geoy+_zoomy)*_scaley;
        y=paintingRect.height()-y;
        return QPoint(floor(x),floor(y));
    }
    void AutoComputeTransPara(const QRect& paintingRect);
    QPoint ComputeFlowOriginDes(OGRGeometry *_ogrgeometry)
    {
        OGRPoint *_ogrpoint;
        _ogrgeometry->Centroid(_ogrpoint);
        return QPoint(_ogrpoint->getX(),_ogrpoint->getY());
    }

public:

    vector<flowdata> flows;
    OGRLayer * layerConnection;
    ODcollection * ODconnection;
    flowcollection();
    flowcollection(string filename, bool with_headers, string delimeter);
    flowcollection(vector<flowdata>);
    flowcollection(string filename);
    void Addflow(flowdata);
    void Deleteflow(int index);
    flowdata Getflow(int index);
    int Countflow();
    void setLayerConnection(OGRLayer *);

    void draw_basemap(QPainter *painter,const QRect& paintingRect);
    void draw_flow(QPainter *painter,const QRect& paintingRect);


    //new added


    flowcollection(ODcollection ods,OGRLayer * _layer);//通用接口
    flowcollection(string filename,OGRLayer * _layerConnection);

    flowcollection(string odfilename,string layerfilename);

    int Countflowid();
};

#endif // FLOWCOLLECTION_H
