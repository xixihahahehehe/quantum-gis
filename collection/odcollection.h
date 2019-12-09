#ifndef ODCOLLECTION_H
#define ODCOLLECTION_H
#include "../geometries/oddata.h"
#include <vector>
#include <fstream>
using namespace std;
class ODcollection
{
private:
    string name;
public:
    vector<ODdata> ODs;

    ODcollection();
    ODcollection(vector<ODdata>);
    ODcollection(string filename);
    void AddOD(ODdata);
    void DeleteOD(int index);
    ODdata GetOD(int index);
    int CountOD();
    string GetName();
};

#endif // ODCOLLECTION_H
