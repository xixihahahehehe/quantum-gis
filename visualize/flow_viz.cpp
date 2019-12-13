#include <iostream>
#include "flow_viz.h"
#include "ui_flow_viz.h"
#include "ogr_core.h"
#include "ogr_geometry.h"
#include <QPainter>
#include <QPainterPath>
#include <QLinearGradient>
#include <QWheelEvent>
#include <QDebug>

flow_viz::flow_viz(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::flow_viz)
{
    ui->setupUi(this);
    flag_press=false;
    //this->setWindowTitle("CSDN IT1995");
    _envelope=new OGREnvelope;
    m_originalWidth = this->width();
    m_originalHeight = this->height();
    //setMouseTracking(true);
}

flow_viz::~flow_viz()
{
    delete ui;
    delete _flowcollection;
    delete _envelope;
    //delete flowcollection
}

void flow_viz::set_flowcollection(flowcollection *_fcollection)
{
    if(_flowcollection)
    {
        _flowcollection=nullptr;
    }
    _flowcollection=_fcollection;
    _flowcollection->layerConnection->GetExtent(_envelope);
    max_weight=_flowcollection->maxstrength;
    min_weight=_flowcollection->minstrength;
    ViewBox.setRect(0,0,width(),height());
    InitialColor();
    flag_press=false;
    flag_move=false;
}

void flow_viz::AutoComputeTransPara()
{
   double _scalex=ViewBox.width()/(_envelope->MaxX-_envelope->MinX)*(1-2*_margin);
   double _scaley=ViewBox.height()/(_envelope->MaxY-_envelope->MinY)*(1-2*_margin);
   if (_scalex>_scaley)
   {
       _scale=_scaley;
   }
   else
   {
       _scale=_scalex;
   }
   double screenx=(ViewBox.width()-_scale*(_envelope->MaxX-_envelope->MinX))/2;
   double screeny=(ViewBox.height()-_scale*(_envelope->MaxY-_envelope->MinY))/2;
   _zoomx=ViewBox.left()+screenx;
   _zoomy=ViewBox.top()+screeny;
}

void flow_viz::InitialColor()
{
    base_line=QColor::fromRgb(100,100,100);
    od_color=QColor::fromRgb(47,79,79);
    //base_polygon=QColor::fromRgb(54,78,104);
    base_polygon=QColor::fromRgb(50,50,50);
    /*
    flow_color.push_back(QColor::fromRgb(255,222,173));
    flow_color.push_back(QColor::fromRgb(255,218,185));
    flow_color.push_back(QColor::fromRgb(255,160,122));
    flow_color.push_back(QColor::fromRgb(250,128,114));
    flow_color.push_back(QColor::fromRgb(240,128,128));
    flow_color.push_back(QColor::fromRgb(205,92,92));
    flow_color.push_back(QColor::fromRgb(178,34,34));
    flow_color.push_back(QColor::fromRgb(128,0,0));
    */
    for (int i=0;i<8;i++)
    {
        flow_color.push_back(QColor::fromRgb(86,149,246));
    }

}

double flow_viz::cal_dist(QPoint o_po, QPoint d_po)
{
    return sqrt(pow((o_po.x()-d_po.y()),2)+pow((o_po.y()-d_po.y()),2));
}

QColor flow_viz::cal_Flowcolor(double flow_weight)
{
    double interval=(max_weight-min_weight)/7;
    int index=floor((flow_weight-min_weight)/interval);
    return flow_color.at(index);
}

double flow_viz::cal_Flowwidth(double flow_weight)
{
    double width=(max_fwidth-min_fwidth)/(max_weight-min_weight)*(flow_weight-min_weight)+min_fwidth;
    return width;
}

QPoint flow_viz::Cal_MidPoint(QPoint o_point, QPoint d_point)
{
    double xmid=(o_point.x()*tan(PI*flow_angle/180)+d_point.x()*tan(PI*flow_angle/180)+(d_point.y()-o_point.y())*tan(PI*flow_angle/180)*tan(PI*flow_angle/180))/(tan(PI*flow_angle/180)+tan(PI*flow_angle/180));
    double ymid=(o_point.y()*tan(PI*flow_angle/180)+d_point.y()*tan(PI*flow_angle/180)+(o_point.x()-d_point.x())*tan(PI*flow_angle/180)*tan(PI*flow_angle/180))/(tan(PI*flow_angle/180)+tan(PI*flow_angle/180));
    return QPoint(floor(xmid),floor(ymid));
}

QPoint flow_viz::Cal_ArrowPoint(QPoint o_point,QPoint d_point,bool flag,double dist)
{
    double angle=0.0;
    if (o_point.x()==d_point.x())
    {
        if(o_point.y()>d_point.y())
        {
         angle=PI/2;
        }
        else
        {
         angle=PI/2*3;
        }
    }
    else
    {

        double tan_od=(o_point.y()-d_point.y())/(o_point.x()-d_point.x());
        angle=atan2((o_point.y()-d_point.y()),(o_point.x()-d_point.x()));
        /*if (angle<0)
        {
            angle=angle+PI;
        }
        */
    }
    if (flag)
    {
        angle=angle+flow_angle/180.0*PI;
    }
    else
    {
        angle=angle-flow_angle/180.0*PI;
    }
    double tmp=dist*0.05*cos(angle);
    QPoint o_arrow=QPoint(d_point.x()+dist*0.1*cos(angle),d_point.y()+dist*0.1*sin(angle));
    return o_arrow;
}

void flow_viz::draw_basemap(QPainter *painter)
{
    if (!_flowcollection->layerConnection)
    {
        return;
    }
    AutoComputeTransPara();
    QPainterPath _path;
    while(OGRFeature *feature=_flowcollection->layerConnection->GetNextFeature())
    {
        OGRGeometry *poGeometry;
        poGeometry=feature->GetGeometryRef();
        QVector<QPoint> _vertices;
       if (wkbFlatten(poGeometry->getGeometryType())==wkbLineString)
       {
            OGRLineString *_linestring=(OGRLineString*) poGeometry;
            int pt_num=_linestring->getNumPoints();
            painter->setPen(QPen(base_polygon,0.5));
            painter->setBrush(Qt::NoBrush);
            for (int i=0;i<pt_num;i++)
            {
               OGRPoint _ogrpoint;
               _linestring->getPoint(i,&_ogrpoint);
               _vertices.push_back(geo2screen(_ogrpoint.getX(),_ogrpoint.getY()));
            }
           painter->drawPolygon(QPolygon(_vertices));
       }
        else if(wkbFlatten(poGeometry->getGeometryType())==wkbPolygon)
       {
           OGRPolygon *_polygon=(OGRPolygon*) poGeometry;
           int NumberOfInnerRings = _polygon ->getNumInteriorRings();
           OGRLinearRing *poExteriorRing = _polygon ->getExteriorRing();
           int NumberOfExteriorRingVertices = poExteriorRing ->getNumPoints();
           QVector<QPoint> exterior_vertices;
           //color set
           painter->setPen(QPen(base_line,0.5));
           /*QLinearGradient lineargradient(ViewBox.left(),ViewBox.top(),ViewBox.left()+ViewBox.width(),ViewBox.right()+ViewBox.height());
           lineargradient.setColorAt(0,base_polygon);
           lineargradient.setColorAt(1,Qt::gray);*/
           painter->setBrush(base_polygon);
           for ( int k = 0; k < NumberOfExteriorRingVertices; k++ )
           {
              OGRPoint _ogrpoint;
              poExteriorRing ->getPoint(k,&_ogrpoint);
              exterior_vertices.push_back(geo2screen(_ogrpoint.getX(),_ogrpoint.getY()));
           }
           //double w=painter->pen().widthF();
           painter->drawPolygon(QPolygon(exterior_vertices));
           for ( int h = 0; h < NumberOfInnerRings; h++ )
           {
              painter->setBrush(Qt::NoBrush);
              OGRLinearRing *poInteriorRing = _polygon ->getInteriorRing(h);
              int NumberOfInteriorRingVertices = poInteriorRing ->getNumPoints();
              QVector<QPoint> interior_vertices;
              for ( int k = 0; k < NumberOfInteriorRingVertices; k++ )
              {
                OGRPoint _ogrpoint;
                poInteriorRing ->getPoint(k,&_ogrpoint);
                interior_vertices.push_back(geo2screen(_ogrpoint.getX(),_ogrpoint.getY()));
              }

              painter->drawPolygon(QPolygon(interior_vertices));
            }
       OGRFeature::DestroyFeature(feature);
       }
   }
}

void flow_viz::draw_OD(QPainter *painter)
{
    painter->setPen(QPen(od_color,0.5));
    painter->setBrush(Qt::NoBrush);
    AutoComputeTransPara();
    vector<ODdata>::iterator _itod;
    for(_itod=_flowcollection->ODconnection->ODs.begin();_itod!=_flowcollection->ODconnection->ODs.end();_itod++)
    {
      //OGRFeature *o_feature=_flowcollection->layerConnection->GetFeature(_iflow->OID);
      QPoint o_po=geo2screen(_itod->Origin->getX(),_itod->Origin->getY());
      //OGRFeature *d_feature=_flowcollection->layerConnection->GetFeature(_iflow->DID);
      QPoint d_po=geo2screen(_itod->Destin->getX(),_itod->Destin->getY());
      //painter->setPen(QPen(QColor::fromRgb(rand()%256,rand()%256,rand()%256)));
      painter->drawLine(o_po,d_po);
      //OGRFeature::DestroyFeature(o_feature);
      //OGRFeature::DestroyFeature(d_feature);
    }
}

void flow_viz::draw_flow(QPainter *painter)
{
    AutoComputeTransPara();
    vector<flowdata>::iterator _iflow;
    for(_iflow=_flowcollection->flows.begin();_iflow!=_flowcollection->flows.end();_iflow++)
    {
      QColor color=cal_Flowcolor(_iflow->weight);
      double width=cal_Flowwidth(_iflow->weight);
      QPainterPath tmpPath;
      painter->setPen(QPen(color,width));
      painter->setBrush(Qt::NoBrush);
      OGRFeature *o_feature=_flowcollection->layerConnection->GetFeature(_iflow->OID);
      QPoint o_po=ComputeFlowOriginDes(o_feature->GetGeometryRef());
      tmpPath.moveTo(o_po);
      OGRFeature *d_feature=_flowcollection->layerConnection->GetFeature(_iflow->DID);
      QPoint d_po=ComputeFlowOriginDes(d_feature->GetGeometryRef());
      QPoint mid_po=Cal_MidPoint(o_po,d_po);
      //painter->setPen(QPen(QColor::fromRgb(rand()%256,rand()%256,rand()%256)));
      tmpPath.quadTo(mid_po,d_po);
      painter->drawPath(tmpPath);
      OGRFeature::DestroyFeature(o_feature);
      OGRFeature::DestroyFeature(d_feature);
      double dist=cal_dist(o_po,d_po);
      QPoint arrow_o1=Cal_ArrowPoint(mid_po,d_po,true,dist);
      QPoint arrow_o2=Cal_ArrowPoint(mid_po,d_po,false,dist);
      QPainterPath arrow1_path;
      QPainterPath arrow2_path;
      //painter->setPen(QPen(QColor::fromRgb(255,0,0)));
      arrow1_path.moveTo(arrow_o1);
      arrow1_path.quadTo(Cal_MidPoint(arrow_o1,d_po),d_po);
      painter->drawPath(arrow1_path);
      arrow2_path.moveTo(arrow_o2);
      arrow2_path.quadTo(Cal_MidPoint(d_po,arrow_o2),d_po);
      painter->drawPath(arrow2_path);
    }
}


void flow_viz::paintEvent(QPaintEvent *)
{
    if(_flowcollection==nullptr)
    {
        return;
    }
    if(_flowcollection->layerConnection==nullptr)
    {
        return;
    }
/*
    int r=0,g=0,b=0;
    for (int i=0;i<3;i++)
    {
        r=rand()%256;
        g=rand()%256;
        b=rand()%256;
    }
    base_polygon=QColor::fromRgb(r,g,b);
*/
    QPainter *_painter=new QPainter(this);
    if (flag_move)
    {
       _painter->begin(this);
       _painter->setRenderHint(QPainter::SmoothPixmapTransform);
       _painter->drawPixmap(ViewBox,screen_map);
       _painter->end();
    }
    else
    {
    _flowcollection->layerConnection->ResetReading();
    _painter->begin(this);
    _painter->setRenderHint(QPainter::Antialiasing);
    draw_basemap(_painter);
    draw_flow(_painter);
    _painter->end();
    }
    //destroy(_painter);  //"destroys" data on a memory location which makes the object not usable,but the memory is still there for use(the object can be constructed again)
                          // call the destructor of the target function
    delete _painter;  //memory deallocated instead of pointer
}


void flow_viz::mousePressEvent(QMouseEvent *event)
{
    //event->accept();
    setCursor(Qt::ClosedHandCursor);
    flag_press=true;
    event->accept();
    pos_x=event->x();
    pos_y=event->y();
}

void flow_viz::mouseMoveEvent(QMouseEvent *event)
{
    if ((event->buttons() & Qt::LeftButton) == Qt::LeftButton)
    {
        //qDebug()<<event->buttons();
        screen_map=QPixmap(ViewBox.width(),ViewBox.height());
        screen_map=this->grab(ViewBox);
        //tmpPixmap.save("C:\\Users\\Geosoft\\Desktop\\hair.png");
        int moveX=event->x()-pos_x;
        int moveY=event->y()-pos_y;
        ViewBox.moveTopLeft(QPoint(moveX+ViewBox.x(),-moveY+ViewBox.y()));
        event->accept();
        pos_x=event->x();
        pos_y=event->y();
        flag_move=true;
        this->update();
        flag_move=false;
    }

}

void flow_viz::mouseReleaseEvent(QMouseEvent *event)
{
   setCursor(Qt::ArrowCursor);
   int moveX=event->x()-pos_x;
   int moveY=event->y()-pos_y;
   ViewBox.moveTopLeft(QPoint(moveX+ViewBox.x(),-moveY+ViewBox.y()));
   flag_press=false;
   flag_move=false;
   event->accept();
   this->update();
}

void flow_viz::wheelEvent(QWheelEvent *event)
{
    double numDegrees = -event->delta() / 8.0;
    double numSteps = numDegrees / 15.0;
    //double mouse_xd = event->x() / static_cast<double>(width());
    //double mouse_yd = event->y() / static_cast<double>(height());
    /*std::cout<<event->x()<<std::endl;
    std::cout<<event->y()<<std::endl;*/
    if (event->orientation() == Qt::Vertical) {
        double moveX = event->x()-(1+numSteps*0.05)*(event->x()-ViewBox.left());
        double moveY = event->y()-(1+numSteps*0.05)*(event->y()-ViewBox.top());
        ViewBox.moveTopLeft(QPoint(moveX,moveY));
        ViewBox.setWidth(ViewBox.width()*(1+numSteps * 0.05));
        ViewBox.setHeight(ViewBox.height()*(1+numSteps*0.05));
        event->accept();
        this->update();
    }

}

void flow_viz::mouseDoubleClickEvent(QMouseEvent *event)
{
   ViewBox.moveTopLeft(QPoint(0,0));
   ViewBox.setWidth(rect().width());
   ViewBox.setHeight(rect().height());
   update();
}

