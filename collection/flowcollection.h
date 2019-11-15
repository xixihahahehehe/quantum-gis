#ifndef FLOWCOLLECTION_H
#define FLOWCOLLECTION_H
#include "geometries/flowdata.h"
#include "ogrsf_frmts.h"
#include <vector>
#include <fstream>
#include <cstdlib>
#include <math.h>
#include "odcollection.h"


using namespace std;
class flowcollection
{
private:

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

    //new added


    flowcollection(ODcollection ods,OGRLayer * _layer);//通用接口
    flowcollection(string filename,OGRLayer * _layerConnection);

    flowcollection(string odfilename,string layerfilename);

    int Countflowid();

    string name;
};

#endif // FLOWCOLLECTION_H
