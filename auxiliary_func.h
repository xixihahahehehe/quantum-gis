#ifndef AUXILIARY_FUNC_H
#define AUXILIARY_FUNC_H
#include <vector>
#include "ogrsf_frmts.h"
#include "collection/flowcollection.h"

#include <thread>
#include <mutex>

using namespace std;

class auxiliary_func
{
public:

    struct tmpodcount
    {
        int _OID;
        int _DID;
    };

    auxiliary_func();
    static vector<string> split(const string& str, const string& delim);
    static OGRLayer * generateGrid(OGRPoint bottomleft,OGRPoint topright,double gridsize);
    static OGRLayer * generateGrid(OGRLayer * polygons);
    static double get_distance(OGRPoint point1, OGRPoint point2);
    static int get_angle(OGRPoint point1, OGRPoint point2);
    static double get_weight(double weight);
    //static flowcollection generateFlowcollection(ODcollection,OGRLayer *);
    static flowcollection generateFlowcollection(ODcollection *,OGRLayer *);
    static void parallel_work(ODcollection * od,OGRLayer * layer,int,int);

};

#endif // AUXILIARY_FUNC_H
