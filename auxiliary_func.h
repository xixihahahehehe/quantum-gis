#ifndef AUXILIARY_FUNC_H
#define AUXILIARY_FUNC_H
#include <vector>
#include<map>
#include<geodesic.h>
#include "ogrsf_frmts.h"
#include "collection/flowcollection.h"
#include "method/CalculateBtwId.h"
using namespace std;

class auxiliary_func
{
public:
    auxiliary_func();
    static vector<string> split(const string& str, const string& delim);
    static OGRLayer * generateGrid(OGRPoint bottomleft,OGRPoint topright,double gridsize);
    static OGRLayer * generateGrid(OGRLayer * polygons);
    static double get_distance(OGRPoint point1, OGRPoint point2);
    static string get_angle(OGRPoint point1, OGRPoint point2);
    //static double get_weight(double weight);
    static flowcollection generateFlowcollection(ODcollection,OGRLayer *);
    
    static double get_collection_distance(int id1, int id2, OGRLayer* currentlyr);
    static string get_collection_angle(int id1, int id2, OGRLayer* currentlyr);
};

#endif // AUXILIARY_FUNC_H
