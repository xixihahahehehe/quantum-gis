#include "odcollection.h"
#include "auxiliary_func.h"
ODcollection::ODcollection()
{

}

ODcollection::ODcollection(vector<ODdata> _ODs)
{
    ODs=_ODs;
}

ODcollection::ODcollection(string filename)
{
    string templine;
    ifstream ifs(filename);
    vector<string> tmpfname=auxiliary_func::split(filename,"//");
    string fullname=tmpfname.back();
    tmpfname.clear();
    tmpfname=auxiliary_func::split(fullname,".");
    name=tmpfname.front();
    vector<ODdata> tempOD;
    OGRSpatialReference * myespWGS84osr=new OGRSpatialReference("");
    myespWGS84osr->SetWellKnownGeogCS("WGS84");
    while (getline(ifs,templine))
    {
        try {
            vector<string> strVec=auxiliary_func::split(templine,",");
            if(strVec.size()==6)
            {
                OGRPoint * tmporigin=new OGRPoint(stod(strVec[0]),stod(strVec[1]),stod(strVec[2]));
                tmporigin->assignSpatialReference(myespWGS84osr);
                OGRPoint * tmpdestin=new OGRPoint(stod(strVec[3]),stod(strVec[4]),stod(strVec[5]));
                tmpdestin->assignSpatialReference(myespWGS84osr);
                tempOD.push_back(ODdata(tmporigin,tmpdestin));
            }
            else if(strVec.size()==4)
            {
                OGRPoint * tmporigin=new OGRPoint(stod(strVec[0]),stod(strVec[1]));
                tmporigin->assignSpatialReference(myespWGS84osr);
                OGRPoint * tmpdestin=new OGRPoint(stod(strVec[2]),stod(strVec[3]));
                tmpdestin->assignSpatialReference(myespWGS84osr);
                tempOD.push_back(ODdata(tmporigin,tmpdestin));
            }

        } catch (...) {
        }
    }
    ODs=tempOD;
}

int ODcollection::CountOD()
{
    return ODs.size();
}

ODdata ODcollection::GetOD(int index)
{
    return ODs[index];
}

void ODcollection::AddOD(ODdata newOD)
{
    ODs.push_back(newOD);
}

string ODcollection::GetName()
{
    return name;
}
