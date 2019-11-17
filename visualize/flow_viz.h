#ifndef FLOW_VIZ_H
#define FLOW_VIZ_H

#include <QWidget>
#include "collection/flowcollection.h"
#include "geometries/flowdata.h"
#include "ogrsf_frmts.h"
#include <vector>
#include <fstream>
#include <cstdlib>
#include <math.h>

//UI related
#include <QPainter>
#include <QPoint>
#include <QPolygon>
#include <QVector>
#include <QPen>
#include <QColor>

namespace Ui {
class flow_viz;
}

class flow_viz : public QWidget
{
    Q_OBJECT

public:
    explicit flow_viz(QWidget *parent = nullptr);
    ~flow_viz();
    void set_flowcollection(flowcollection *_fcollection);
    void draw_basemap(QPainter *painter, const QRect& paintingRect);
private:
    Ui::flow_viz *ui;
    flowcollection *_flowcollection=nullptr;

    double _zoomx;
    double _zoomy;
    double _scale;
    double max_weight;
    double min_weight;
    double _margin=0.05;
    OGREnvelope *_envelope;
    QPoint geo2screen(double _geox,double _geoy,const QRect& paintingRect)
    {
        double x=(_geox-431742.787399994)*_scale+_zoomx;
        double y=(_geoy-4400938.1632)*_scale+_zoomy;
        y=paintingRect.height()-y;
        return QPoint(floor(x),floor(y));
    }
    void AutoComputeTransPara(const QRect& paintingRect);
    QPoint ComputeFlowOriginDes(OGRGeometry *_ogrgeometry,const QRect& paintingRect)
    {
        OGRPoint _ogrpoint;
        _ogrgeometry->Centroid(&_ogrpoint);
        return geo2screen(_ogrpoint.getX(),_ogrpoint.getY(),paintingRect);
    }


    void draw_flow(QPainter *painter,const QRect& paintingRect);
    virtual void paintEvent( QPaintEvent * );

    void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;

    void extendWindowsSize();
    void shrinkWindowsSize();

    int m_originalWidth;
    int m_originalHeight;

};

#endif // FLOW_VIZ_H
