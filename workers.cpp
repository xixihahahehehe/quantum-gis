#include "workers.h"

/*struct tmpodcount
{
    int _OID;
    int _DID;
};*/

bool operator<(workers::tmpodcount a, workers::tmpodcount b)
{
        return a._OID < b._OID;
}



workers::workers()
{

}
void workers::doWork(ODcollection od,OGRLayer * layer,flowcollection * fc)
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
        //tmp.Addflow(tmpflow);
        fc->Addflow(tmpflow);
    }
    fc->setLayerConnection(layer);
    fc->name=od.name+"_flow";
    emit resultReady("result");
    /*tmp.setLayerConnection(layer);
    tmp.name=od.name+"_flow";
    return tmp;*/
}
