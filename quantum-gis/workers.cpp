#include "workers.h"
#include <QMessageBox>
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
    fc->SetName(od.GetName()+"_flow");
    emit resultReady("result");
    /*tmp.setLayerConnection(layer);
    tmp.name=od.name+"_flow";
    return tmp;*/
}
void workers::paldoWork(ODcollection od,OGRLayer * layer,int start,int end\
                        ,std::map<tmpodcount,int> * newmap,volatile int * useable)
{
    flowcollection tmp=flowcollection();
    for(int i=start;i<end;i++)
    {
        OGRFeature *poFeature;
        int OID=-1;
        int DID=-1;
        layer->ResetReading();
        // optim here (OID==-1 || DID==-1)
        // half and ODdata aa=od.GetOD(i);
        ODdata aa=od.GetOD(i);
        layer->ResetReading();
        while( (poFeature = layer->GetNextFeature()) != NULL &&(OID==-1 || DID==-1))
        {
            bool containO=poFeature->GetGeometryRef()->Contains(aa.Origin);
            bool containD=poFeature->GetGeometryRef()->Contains(aa.Destin);
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
            //if readlock
            map<tmpodcount,int>::iterator iter1;
            tmpodcount aa;
            aa._DID=DID;
            aa._OID=OID;
            //mutex.lock();
            while(*useable==0)
            {

            }
            *useable=0;
            iter1=newmap->find(aa);
            if(iter1==newmap->end())
            {
                (*newmap)[aa]=1;
            }
            else
            {
                (*newmap)[aa]=(*newmap)[aa]+1;
            }
            *useable=1;
        }
    }
    emit palresultReady();

    /*map<tmpodcount, int>::iterator iter;
    for(iter = tmpmap.begin(); iter != tmpmap.end(); iter++)
    {
        flowdata tmpflow=flowdata(iter->first._OID,iter->first._DID,iter->second);
        //tmp.Addflow(tmpflow);
        fc->Addflow(tmpflow);
    }
    fc->setLayerConnection(layer);
    fc->name=od.name+"_flow";
    emit resultReady("result");*/


    /*tmp.setLayerConnection(layer);
    tmp.name=od.name+"_flow";
    return tmp;*/
}
