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
#include <QRect>
#include <QCursor>
#include <QWheelEvent>
#include <QMouseEvent>

#define PI 3.14159265

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

private:
    Ui::flow_viz *ui;
    flowcollection *_flowcollection=nullptr;

    bool flag_press;
    int pos_x;
    int pos_y;
    double scroll_size=0.05;
    double _zoomx;
    double _zoomy;
    double _scale;
    double max_weight;
    double min_weight;
    double _margin=0.05;
    double min_fwidth=0.5;
    double max_fwidth=2;
    int flow_angle=20;
    QColor od_color;
    vector<QColor> flow_color;
    QColor base_line;
    QColor base_polygon;//fill polygon or draw polyline
    QRect ViewBox;
    OGREnvelope *_envelope;

    QPoint geo2screen(double _geox,double _geoy)
    {
        double x=(_geox-_envelope->MinX)*_scale+_zoomx;
        double y=(_geoy-_envelope->MinY)*_scale+_zoomy;
        y=ViewBox.height()-y;
        return QPoint(floor(x),floor(y));
    }
    QPoint Cal_MidPoint(QPoint o_point,QPoint d_point);
    QPoint Cal_ArrowPoint(QPoint o_point,QPoint d_point);
    void AutoComputeTransPara();
    QPoint ComputeFlowOriginDes(OGRGeometry *_ogrgeometry)
    {
        OGRPoint _ogrpoint;
        _ogrgeometry->Centroid(&_ogrpoint);
        return geo2screen(_ogrpoint.getX(),_ogrpoint.getY());
    }

    void InitialColor();
    QColor cal_Flowcolor(double flow_weight);
    double cal_Flowwidth(double flow_weight);
    void draw_OD(QPainter *painter);
    void draw_flow(QPainter *painter);
    void draw_basemap(QPainter *painter);
    virtual void paintEvent( QPaintEvent * );
    void extendWindowsSize();
    void shrinkWindowsSize();

    int m_originalWidth;
    int m_originalHeight;

protected:
    //virtual void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;


    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mouseDoubleClickEvent(QMouseEvent *event);
    virtual void wheelEvent(QWheelEvent *event);

};

#endif // FLOW_VIZ_H
