#include "flowcollection.h"
#include "auxiliary_func.h"


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
    vector<string> tmpfname=auxiliary_func::split(filename,"//");
    string fullname=tmpfname.back();
    tmpfname.clear();
    tmpfname=auxiliary_func::split(fullname,".");
    name=tmpfname.front();
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
