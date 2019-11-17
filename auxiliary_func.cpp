#include "auxiliary_func.h"
#define PI                      3.141592654
#define EARTH_RADIUS            6378.137        //地球近似半径

auxiliary_func::auxiliary_func()
{

}
vector<string> auxiliary_func::split(const string &str, const string &delim)
{
    vector<string> res;
    if("" == str) return res;
    //先将要切割的字符串从string类型转换为char*类型
    char * strs = new char[str.length() + 1] ; //不要忘了
    strcpy(strs, str.c_str());

    char * d = new char[delim.length() + 1];
    strcpy(d, delim.c_str());

    char *p = strtok(strs, d);
    while(p) {
        string s = p; //分割得到的字符串转换为string类型
        res.push_back(s); //存入结果数组
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
            //这一块 还有点问题
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

string auxiliary_func::get_angle(OGRPoint point1, OGRPoint point2)
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
    string str1=to_string(pazi1);
    string str2=to_string(pazi2);
    string str3=str1+","+str2;
    return str3;
}

string auxiliary_func::get_collection_angle(int id1, int id2, OGRLayer *currentlyr)
{
    OGRPoint p[2];
    get_feature_byid(id1, &p[0], currentlyr, "centroid");
    get_feature_byid(id2, &p[1], currentlyr, "centroid");
    string angle;
    angle=get_angle(p[0], p[1]);
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
    tmp.name=od.name+"_flow";
    return tmp;
}
