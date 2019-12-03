#include "oddata.h"

ODdata::ODdata()
{

}
ODdata::ODdata(OGRPoint * _O,OGRPoint * _D)
{
    Origin=_O;
    Destin=_D;
}

