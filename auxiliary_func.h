#ifndef AUXILIARY_FUNC_H
#define AUXILIARY_FUNC_H
#include <vector>
#include "ogrsf_frmts.h"
#include "collection/flowcollection.h"
using namespace std;

class auxiliary_func
{
public:
    auxiliary_func();
    static vector<string> split(const string& str, const string& delim);
    static OGRLayer * generateGrid(OGRPoint bottomleft,OGRPoint topright,double gridsize);
    static OGRLayer * generateGrid(OGRLayer * polygons);
    static double get_distance(OGRPoint point1, OGRPoint point2);
    static int get_angle(OGRPoint point1, OGRPoint point2);
    static double get_weight(double weight);
    static flowcollection generateFlowcollection(ODcollection,OGRLayer *);
};

#endif // AUXILIARY_FUNC_H
