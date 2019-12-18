#include "auxiliary_func.h"
#define PI                      3.141592654
#define EARTH_RADIUS            6378.137

auxiliary_func::auxiliary_func()
{

}
vector<string> auxiliary_func::split(const string &str, const string &delim)
{
    vector<string> res;
    if("" == str) return res;
    char * strs = new char[str.length() + 1] ;
    strcpy(strs, str.c_str());

    char * d = new char[delim.length() + 1];
    strcpy(d, delim.c_str());

    char *p = strtok(strs, d);
    while(p) {
        string s = p;
        res.push_back(s);
        p = strtok(NULL, d);
    }

    return res;
}

OGRLayer * auxiliary_func::generateGrid(OGRPoint bottomleft, OGRPoint topright, double gridsize)
{
    OGRLayer * templayer;
    for(double x=bottomleft.getX();x<topright.getX();x=x+gridsize)
    {
        for(double y=bottomleft.getY();y<topright.getY();y=y+gridsize)
        {
            OGRGeometry * bb;
            string wkt="POLYGON(("+to_string(x)+" "+to_string(y)+","+\
                    to_string(x+gridsize)+" "+to_string(y)+","+\
                    to_string(x+gridsize)+" "+to_string(y+gridsize)+","+\
                    to_string(x)+" "+to_string(y+gridsize)+"))";

            //char * cc[2];
            //strcpy(cc[0],wkt.c_str());
            /*bb->importFromWkt(cc);
            OGRFeature * aa;
            aa=OGRFeature::CreateFeature(templayer->GetLayerDefn());
            aa->SetGeometry(bb);
            templayer->CreateFeature(aa);*/
        }
    }
    return templayer;
}

double auxiliary_func::get_distance(OGRPoint point1, OGRPoint point2)
{
    struct geod_geodesic g;
    double s12;
    geod_init(&g, 6378137, 1/298.257223563);
    geod_inverse(&g, point1.getX(), point1.getY(), point2.getX(), point2.getY(), &s12, 0, 0);
    //printf("%.3f\n", s12);
    return s12;
}

double auxiliary_func::get_collection_distance(int id1, int id2, OGRLayer *currentlyr)
{
    OGRPoint p[2];
    get_feature_byid(id1, &p[0], currentlyr, "centroid");
    get_feature_byid(id2, &p[1], currentlyr, "centroid");
    double dis;
    dis=get_distance(p[0], p[1]);
    return dis;
}

double auxiliary_func::get_angle(OGRPoint point1, OGRPoint point2)
{
    //vector <string> angle;
    struct geod_geodesic g;
    double s12;
    double pazi1;
    double pazi2;
    geod_init(&g, 6378137, 1/298.257223563);
    geod_inverse(&g, point1.getX(), point1.getY(), point2.getX(), point2.getY(), &s12, &pazi1, &pazi2);
    //printf("%.3f\n", pazi1);
    //printf("%.3f\n", pazi2);
//    string str1=to_string(pazi1);
//    string str2=to_string(pazi2);
//    string str3=str1+","+str2;
//    return str3;
    return pazi1;
}

double auxiliary_func::get_collection_angle(int id1, int id2, OGRLayer *currentlyr)
{
    OGRPoint p[2];
    get_feature_byid(id1, &p[0], currentlyr, "centroid");
    get_feature_byid(id2, &p[1], currentlyr, "centroid");
    double angle;
    angle=get_angle(p[0], p[1]);
    return angle;
}

double auxiliary_func::get_distance_proj(OGRPoint point1, OGRPoint point2)
{
//     double radLat1 = point1.getX() * PI / 180.0;   //1˚ = π / 180
//     double radLat2 = point2.getX() * PI / 180.0;   //1˚ = π / 180
//     double a = radLat1 - radLat2;//
//     double b = point1.getY() * PI / 180.0 - point2.getY() * PI / 180.0;  //
//     double dst = 2 * asin((sqrt(pow(sin(a / 2), 2) + cos(radLat1) * cos(radLat2) * pow(sin(b / 2), 2))));
//     dst = dst * EARTH_RADIUS;
//     dst = round(dst * 10000) / 10000;
//     return dst;
    double x1=point1.getX();
    double x2=point2.getX();
    double y1=point1.getY();
    double y2=point2.getY();
    double dst=sqrt(((x1-x2)*(x1-x2))+((y1-y2)*(y1-y2)));
    return dst;
}

double auxiliary_func::get_collection_distance_proj(int id1, int id2, OGRLayer *currentlyr)
{
     OGRPoint p[2];
     get_feature_byid(id1, &p[0], currentlyr, "centroid");
     get_feature_byid(id2, &p[1], currentlyr, "centroid");
     double dis;
     dis=get_distance_proj(p[0], p[1]);
     return dis;
}
double auxiliary_func::get_angle_proj(OGRPoint point1, OGRPoint point2)
{
    double x = point1.getX() - point2.getX();//t d
    double y = point1.getY() - point2.getY();//z y
    double angle = -1;
    if (y == 0 && x > 0) angle = 0;
    if (y == 0 && x < 0) angle = 180;
    if (x == 0 && y > 0) angle = 90;
    if (x == 0 && y < 0) angle = 270;
    if (angle == -1)
    {
        OGRPoint p1(point1.getX(),point2.getY());
        OGRPoint p2(point2.getX(),point1.getY());
        double dislat = get_distance_proj(p1, point2);
        double dislng = get_distance_proj(p2, point2);
        if (x > 0 && y > 0) angle = atan2(dislng, dislat) / PI * 180;
        if (x < 0 && y > 0) angle = atan2(dislat, dislng) / PI * 180 + 90;
        if (x < 0 && y < 0) angle = atan2(dislng, dislat) / PI * 180 + 180;
        if (x > 0 && y < 0) angle = atan2(dislat, dislng) / PI * 180 + 270;
    }
//    string str1=to_string(angle);
    return angle;
}

double auxiliary_func::get_collection_angle_proj(int id1, int id2, OGRLayer *currentlyr)
{
    OGRPoint p[2];
    get_feature_byid(id1, &p[0], currentlyr, "centroid");
    get_feature_byid(id2, &p[1], currentlyr, "centroid");
    double angle;
    angle=get_angle_proj(p[0], p[1]);
    return angle;
}

struct tmpodcount
{
    int _OID;
    int _DID;
};

bool operator<(tmpodcount a, tmpodcount b)
{
        return a._OID < b._OID;
}

flowcollection auxiliary_func::generateFlowcollection(ODcollection od, OGRLayer * layer)
{
    flowcollection tmp=flowcollection();
    std::map<tmpodcount,int> tmpmap;
    for(int i=0;i<od.CountOD();i++)
    {
        OGRFeature *poFeature;
        int OID=-1;
        int DID=-1;
        layer->ResetReading();
        while( (poFeature = layer->GetNextFeature()) != NULL)
        {
            bool containO=poFeature->GetGeometryRef()->Contains(od.GetOD(i).Origin);
            bool containD=poFeature->GetGeometryRef()->Contains(od.GetOD(i).Destin);
            if(containO)
            {
                OID=poFeature->GetFID();
            }
            if(containD)
            {
                DID=poFeature->GetFID();
            }
        }
        if(OID!=-1&&DID!=-1)
        {
            map<tmpodcount,int>::iterator iter1;
            tmpodcount aa;
            aa._DID=DID;
            aa._OID=OID;
            iter1=tmpmap.find(aa);
            if(iter1==tmpmap.end())
            {
                tmpmap[aa]=1;
            }
            else
            {
                tmpmap[aa]=tmpmap[aa]+1;
            }
        }
    }
    map<tmpodcount, int>::iterator iter;
    for(iter = tmpmap.begin(); iter != tmpmap.end(); iter++)
    {
        flowdata tmpflow=flowdata(iter->first._OID,iter->first._DID,iter->second);
        tmp.Addflow(tmpflow);
    }
    tmp.setLayerConnection(layer);
    tmp.SetName(od.GetName()+"_flow");
    return tmp;
}
