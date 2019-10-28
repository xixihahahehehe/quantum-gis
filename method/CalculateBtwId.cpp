#include"CalculateBtwId.h"



void get_feature_byid(int id, void*res, OGRLayer* currentlyr, const char* type, const char* fieldname)
{
	currentlyr->ResetReading();
	OGRFeature *poFeature;
	
	while ((poFeature = currentlyr->GetNextFeature()) != NULL)
	{
		if (poFeature->GetFID() == id) // find the feature
		{

			if (strcmp(type, "centroid") == 0)
			{
				//OGRPoint* pt=(OGRPoint*)res;
				//res = (OGRPoint*)res;
				poFeature->GetGeometryRef()->Centroid((OGRPoint*)res);
				((OGRPoint*)res)->assignSpatialReference(poFeature->GetGeometryRef()->getSpatialReference()->Clone());
				//cout << pt.getSpatialReference()->IsProjected() << endl << poFeature->GetGeometryRef()->getSpatialReference()->IsProjected() << endl;
				//OGRFeature::DestroyFeature(poFeature);
				//res = pt;
				//return &pt;
			}
			if (strcmp(type, "attribute") == 0)
			{
				int fidx = poFeature->GetFieldIndex(fieldname);
				OGRFieldDefn* pFieldDef = poFeature->GetFieldDefnRef(fidx);
				//cout << pFieldDef->GetFieldTypeName(pFieldDef->GetType()) << endl;
	
				if (pFieldDef->GetType()==OFTInteger)
				{
					int attr = poFeature->GetFieldAsInteger(fieldname);
					*(int *)res = attr;
					//return &attr;
				}
				else if (pFieldDef->GetType() == OFTInteger64)
				{
					int64 attr = poFeature->GetFieldAsInteger(fieldname);
					*(int *)res = attr;
					//return &attr;
				}
				else if (pFieldDef->GetType() == OFTString)
				{
					const char * attr = poFeature->GetFieldAsString(fieldname);
					strcpy((char *)res, attr);
					//return (void*)attr;
				}
				else if (pFieldDef->GetType() == OFTReal)
				{
					double attr;
					attr = poFeature->GetFieldAsDouble(fieldname);
					*(double *)res = attr;
					//return &attr;
				}
				
			}
			OGRFeature::DestroyFeature(poFeature);
			return;

		}
		OGRFeature::DestroyFeature(poFeature);
		
		//cout << endl;
	}
	//OGRFeature::DestroyFeature(poFeature);
	res = NULL;
	return;
	//return NULL;
}

double get_poly_distance(int id1, int id2, OGRLayer* currentlyr)
{
	//currentlyr->ResetReading();
	//OGRFeature *poFeature;
	OGRPoint p[2], pr_p[2];
	//p[0] = *(OGRPoint*)get_feature_byid(id1, currentlyr, "centroid");
	get_feature_byid(id1, &p[0], currentlyr, "centroid");
	get_feature_byid(id2, &p[1], currentlyr, "centroid");
	double dis;
	dis = p[0].Distance(&p[1]);
	if (!p[0].getSpatialReference()->IsProjected())
	{
		// get projected points
		pr_p[0] = get_proj_pt(&p[0]);
		pr_p[1] = get_proj_pt(&p[1]);
		// calculate projected distances
		dis = pr_p[0].Distance(&pr_p[1]);
	}

	return dis;

}

OGRPoint get_proj_pt(OGRPoint *p)
{
	OGRPoint *pt = p;

	//定义地理坐标系 WGS84
	char    *WGS84_WKT = NULL;
	OGRSpatialReference oTargetSRS, *oSourceSRS = pt->getSpatialReference();
	OGRCoordinateTransformation *poTransform;

	oTargetSRS.SetProjCS("UTM");//Transverse_Mercator//EPSG:3857
	oTargetSRS.SetWellKnownGeogCS("WGS84");
	//oTargetSRS.importFromEPSG(3857);
	//oTargetSRS.SetProjection(SRS_PT_MERCATOR_AUXILIARY_SPHERE);////SRS_PT_TRANSVERSE_MERCATOR
	//oTargetSRS.set
	oTargetSRS.SetUTM(50);

	oTargetSRS.exportToWkt(&WGS84_WKT);
	printf("%s\n", WGS84_WKT);
	CPLFree(WGS84_WKT);//使用完后释放
	oSourceSRS->exportToWkt(&WGS84_WKT);
	printf("%s\n", WGS84_WKT);
	CPLFree(WGS84_WKT);
	//OGRSpatialReference  wgs84;
	//assert(wgs84.SetWellKnownGeogCS("WGS84") == OGRERR_NONE);
	//p[0].assignSpatialReference(&wgs84);
	//p[1].assignSpatialReference(&wgs84);
	//pt = &p[0];
	//cout << pt->getSpatialReference() << endl;
	poTransform = OGRCreateCoordinateTransformation(oSourceSRS, &oTargetSRS);


	if (poTransform == NULL)
		printf("Transformation failed.\n");

	double x = p->getX(), y = p->getY();
	int n = 1;
	poTransform->Transform(1, &x, &y);
	OGRPoint p_pt;
	p_pt.setX(x);
	p_pt.setY(y);
	p_pt.assignSpatialReference(oTargetSRS.Clone());
	
	OCTDestroyCoordinateTransformation(poTransform);
	return p_pt;
}

