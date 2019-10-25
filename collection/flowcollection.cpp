#include "flowcollection.h"
#include "auxiliary_func.h"
#include "ogr_core.h"
#include "ogr_geometry.h"

flowcollection::flowcollection()
{

}

flowcollection::flowcollection(vector<flowdata> _flows)
{
    flows=_flows;
}

int flowcollection::Countflow()
{
    return flows.size();
}

flowdata flowcollection::Getflow(int index)
{
    return flows[index];
}

void flowcollection::Addflow(flowdata newflow)
{
    flows.push_back(newflow);
}

void flowcollection::setLayerConnection(OGRLayer * layer)
{
    GDALAllRegister();
    layerConnection=layer;
    //layerConnection->GetExtent(_envelope);
}

flowcollection::flowcollection(string odfilename,string layerfilename)
{

}

flowcollection::flowcollection(string filename)
{
    string templine;
    ifstream ifs(filename);
    vector<flowdata> tempflows;
    while (getline(ifs,templine))
    {
        try {
            vector<string> strVec=auxiliary_func::split(templine,",");
            tempflows.push_back(flowdata(stoi(strVec[0]),stoi(strVec[1]),stod(strVec[2])));
        } catch (...) {

        }
    }
    flows=tempflows;
}

void flowcollection::AutoComputeTransPara(const QRect& paintingRect)
{
   _scalex=paintingRect.width()/(460959.095700001-431742.787399994)*(1-2*_margin);
   _scaley=paintingRect.height()/(4430399.974300002-4400938.1632)*(1-2*_margin);
   _zoomx=paintingRect.left()+_margin*paintingRect.width()-431742.787399994;
   _zoomy=paintingRect.top()+_margin*paintingRect.height()-4400938.1632;

}

void flowcollection::draw_basemap(QPainter *painter, const QRect &paintingRect)
{
    if (!layerConnection)
    {
        return;
    }
    AutoComputeTransPara(paintingRect);
    int r,g,b;
    for (int i=0;i<3;i++)
    {
        r=rand()%256;
        g=rand()%256;
        b=rand()%256;
    }
    _qcolor=QColor::fromRgb(r,g,b);
    painter->setPen(QPen(_qcolor));
    OGRFeature *feature;
    while(feature=layerConnection->GetNextFeature())
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
               OGRPoint *_ogrpoint;
               _linestring->getPoint(i,_ogrpoint);
               _vertices.push_back(geo2screen(_ogrpoint->getX(),_ogrpoint->getY(),paintingRect));
            }
            painter->drawPolyline(QPolygon(_vertices));
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
              OGRPoint *_ogrpoint;
              poExteriorRing ->getPoint(k,_ogrpoint);
              exterior_vertices.push_back(geo2screen(_ogrpoint->getX(),_ogrpoint->getY(),paintingRect));
           }
           painter->drawPolyline(QPolygon(exterior_vertices));
           painter->setPen(QPen(QColor::fromRgb(rand()%256,rand()%256,rand()%256)));
           for ( int h = 0; h < NumberOfInnerRings; h++ )
           {
              OGRLinearRing *poInteriorRing = _polygon ->getInteriorRing(h);
              int NumberOfInteriorRingVertices = poInteriorRing ->getNumPoints();
              QVector<QPoint> interior_vertices;
              for ( int k = 0; k < NumberOfInteriorRingVertices; k++ )
              {
                OGRPoint *_ogrpoint;
                poInteriorRing ->getPoint(k,_ogrpoint);
                interior_vertices.push_back(geo2screen(_ogrpoint->getX(),_ogrpoint->getY(),paintingRect));
              }
              painter->drawPolyline(QPolygon(exterior_vertices));
       }
        OGRFeature::DestroyFeature(feature);
       }
    }
}

void flowcollection::draw_flow(QPainter *painter, const QRect &paintingRect)
{
    vector<flowdata>::iterator _iflow;
    for (_iflow=flows.begin();_iflow!=flows.end();_iflow++)
    {
      OGRFeature *o_feature=layerConnection->GetFeature(_iflow->OID);
      QPoint o_po=ComputeFlowOriginDes(o_feature->GetGeometryRef());
      OGRFeature *d_feature=layerConnection->GetFeature(_iflow->DID);
      QPoint d_po=ComputeFlowOriginDes(d_feature->GetGeometryRef());
      painter->setPen(QPen(QColor::fromRgb(rand()%256,rand()%256,rand()%256)));
      painter->drawLine(o_po,d_po);
      OGRFeature::DestroyFeature(o_feature);
      OGRFeature::DestroyFeature(d_feature);
    }
}


//jzj added

flowcollection::flowcollection(string filename, bool with_headers, string delimeter)
{
    string templine;
    ifstream ifs(filename);
    vector<flowdata> tempflows;
    while (getline(ifs, templine))
    {
        if (with_headers)
        {
            with_headers = 0;
            continue;
        }
        try {
            //vector<string> strVec=auxiliary_func::split(templine,",");
            vector<string> strVec = auxiliary_func::split(templine, delimeter);
            if (strVec.size() != 3)
            {
                delimeter = "\t";
                strVec = auxiliary_func::split(templine, delimeter);
            }
            if (strVec.size() != 3)
            {
                delimeter = " ";
                strVec = auxiliary_func::split(templine, delimeter);
            }
            if (strVec.size() == 3)
            {
                tempflows.push_back(flowdata(stoi(strVec[0]), stoi(strVec[1]), stod(strVec[2])));
            }
        }
        catch (...) {

        }
    }
    flows = tempflows;
}
int flowcollection::Countflowid()
{
    return layerConnection->GetFeatureCount();
}
