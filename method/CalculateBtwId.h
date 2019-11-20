#ifndef CALCULATEBTWID_H
#define CALCULATEBTWID_H
#include<iostream>
#include<string.h>
#include <vector>
#include <cstdlib> // exit()
#include<list>
#include<vector>

//#include"geos.h"
//#include "gdal_priv.h"
#include "ogrsf_frmts.h"
#define ACCEPT_USE_OF_DEPRECATED_PROJ_API_H 1
#include "proj_api.h"

#include "igraph.h"

using namespace std;





double get_poly_distance(int id1, int id2, OGRLayer* currentlyr);
// centroid distance of polygon id1 and id2 
// if not projected, currently using UTM50

//void* get_feature_byid(int id, OGRLayer* currentlyr, const char* type, const char* fieldname = NULL);
void get_feature_byid(int id, void*res, OGRLayer* currentlyr, const char* type, const char* fieldname = NULL);
// get feature by id()
// type: "centroid" return (OGRPoint*)   /*get feature centroid*/
// type: "attribute" fieldname:""(specified by layer)

OGRPoint get_proj_pt(OGRPoint *p);
// get projected pt from WGS84
// set projected coordinate system UTM 50 


#endif
