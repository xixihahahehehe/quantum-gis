#include "flow_viz.h"
#include "ui_flow_viz.h"
#include "ogr_core.h"
#include "ogr_geometry.h"
#include <QPainter>
#include <QPainterPath>

flow_viz::flow_viz(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::flow_viz)
{
    ui->setupUi(this);
}

flow_viz::~flow_viz()
{
    delete ui;
    //delete flowcollection
}

void flow_viz::set_flowcollection(flowcollection *_fcollection)
{
    if(_flowcollection)
    {
        _flowcollection=nullptr;
    }
    _flowcollection=_fcollection;
}

void flow_viz::AutoComputeTransPara(const QRect& paintingRect)
{
   double _scalex=paintingRect.width()/(460959.095700001-431742.787399994)*(1-2*_margin);
   double _scaley=paintingRect.height()/(4430399.974300002-4400938.1632)*(1-2*_margin);
   if (_scalex>_scaley)
   {
       _scale=_scaley;
   }
   else
   {
       _scale=_scalex;
   }
   double screenx=(paintingRect.width()-_scale*(460959.095700001-431742.787399994))/2;
   double screeny=(paintingRect.height()-_scale*(4430399.974300002-4400938.1632))/2;
   _zoomx=paintingRect.left()+screenx;
   _zoomy=paintingRect.top()+screeny;

}

void flow_viz::draw_basemap(QPainter *painter, const QRect &paintingRect)
{
    if (!_flowcollection->layerConnection)
    {
        return;
    }
    AutoComputeTransPara(paintingRect);
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
            for (int i=0;i<pt_num;i++)
            {
               OGRPoint _ogrpoint;
               _linestring->getPoint(i,&_ogrpoint);
               _vertices.push_back(geo2screen(_ogrpoint.getX(),_ogrpoint.getY(),paintingRect));
            }
            _path.addPolygon(QPolygon(_vertices));
       }
        else if(wkbFlatten(poGeometry->getGeometryType())==wkbPolygon)
       {
           OGRPolygon *_polygon=(OGRPolygon*) poGeometry;
           int NumberOfInnerRings = _polygon ->getNumInteriorRings();
           OGRLinearRing *poExteriorRing = _polygon ->getExteriorRing();
           int NumberOfExteriorRingVertices = poExteriorRing ->getNumPoints();
           QVector<QPoint> exterior_vertices;
           for ( int k = 0; k < NumberOfExteriorRingVertices; k++ )
           {
              OGRPoint _ogrpoint;
              poExteriorRing ->getPoint(k,&_ogrpoint);
              exterior_vertices.push_back(geo2screen(_ogrpoint.getX(),_ogrpoint.getY(),paintingRect));
           }
           _path.addPolygon(QPolygon(exterior_vertices));
           for ( int h = 0; h < NumberOfInnerRings; h++ )
           {
              OGRLinearRing *poInteriorRing = _polygon ->getInteriorRing(h);
              int NumberOfInteriorRingVertices = poInteriorRing ->getNumPoints();
              QVector<QPoint> interior_vertices;
              for ( int k = 0; k < NumberOfInteriorRingVertices; k++ )
              {
                OGRPoint _ogrpoint;
                poInteriorRing ->getPoint(k,&_ogrpoint);
                interior_vertices.push_back(geo2screen(_ogrpoint.getX(),_ogrpoint.getY(),paintingRect));
              }
              _path.addPolygon(QPolygon(interior_vertices));
            }
       }
       OGRFeature::DestroyFeature(feature);
    }
    painter->drawPath(_path);
}

void flow_viz::draw_flow(QPainter *painter, const QRect &paintingRect)
{
    int r,g,b;
    for (int i=0;i<3;i++)
    {
        r=rand()%256;
        g=rand()%256;
        b=rand()%256;
    }
    QColor _qcolor=QColor::fromRgb(r,g,b);
    painter->setPen(QPen(_qcolor));
    AutoComputeTransPara(paintingRect);
    vector<flowdata>::iterator _iflow;
    for(_iflow=_flowcollection->flows.begin();_iflow!=_flowcollection->flows.end();_iflow++)
    {
      OGRFeature *o_feature=_flowcollection->layerConnection->GetFeature(_iflow->OID);
      QPoint o_po=ComputeFlowOriginDes(o_feature->GetGeometryRef(),rect());
      OGRFeature *d_feature=_flowcollection->layerConnection->GetFeature(_iflow->DID);
      QPoint d_po=ComputeFlowOriginDes(d_feature->GetGeometryRef(),rect());
      //painter->setPen(QPen(QColor::fromRgb(rand()%256,rand()%256,rand()%256)));
      painter->drawLine(o_po,d_po);
      OGRFeature::DestroyFeature(o_feature);
      OGRFeature::DestroyFeature(d_feature);
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
    _flowcollection->layerConnection->ResetReading();
    QPainter *_painter=new QPainter(this);
    int r,g,b;
    for (int i=0;i<3;i++)
    {
        r=rand()%256;
        g=rand()%256;
        b=rand()%256;
    }
    _painter->begin(this);
    _painter->setRenderHint(QPainter::Antialiasing);
    QColor _qcolor=QColor::fromRgb(r,g,b);
    _painter->setPen(QPen(_qcolor));
    _painter->begin(this);
    draw_basemap(_painter,rect());

    for (int i=0;i<3;i++)
    {
        r=rand()%256;
        g=rand()%256;
        b=rand()%256;
    }
    _qcolor=QColor::fromRgb(r,g,b);
    _painter->setPen(QPen(_qcolor));
    draw_flow(_painter,rect());

    _painter->end();
}
