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
    string name;
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
    double minstrength=2147483647;
    double maxstrength=-1;
    //new added


    flowcollection(ODcollection ods,OGRLayer * _layer);//通用接口
    flowcollection(string filename,OGRLayer * _layerConnection);

    flowcollection(string odfilename,string layerfilename);

    int Countflowid();
    string GetName();
    void SetName(string fname);


};

#endif // FLOWCOLLECTION_H
