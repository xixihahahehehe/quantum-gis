#ifndef ODDATA_H
#define ODDATA_H
#include <ogr_feature.h>


class ODdata
{
public:

    //timestamp:Point.Z or Point.M
    OGRPoint * Origin;
    OGRPoint * Destin;
    ODdata();
    ODdata(OGRPoint *,OGRPoint *);
};

#endif // ODDATA_H
