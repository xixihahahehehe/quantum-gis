#include"DistanceDecayPara.h"

double distance_decay_parameter(flowgraph& od_graph, vector<vector<double>> &res)
{
	vector<int>degree;
	od_graph.get_degree(degree);

	cout << od_graph.get_degree_byid(522, 1) << endl;
	cout << od_graph.get_flow_byid(582, 581, 0) << endl;


	//vector<vector<double>> res;
	vector<double>x, y;
	double avg_x = 0, avg_y = 0, sum_xy = 0, sum_xx = 0;
	int vcount = od_graph.get_vcount();

	double Gij, dij, Aij;
	int Pi, Pj;
	int org_id1, org_id2;
	for (int i = 0; i < vcount; i++)
	{
		for (int j = i + 1; j < vcount; j++)
		{

			Gij = (od_graph.get_flow_byid(i, j, 0) + od_graph.get_flow_byid(j, i, 0));
			if (Gij > -0.000001 && Gij < 0.000001) continue;
			Pi = od_graph.get_degree_byid(i, 0);
			Pj = od_graph.get_degree_byid(j, 0);
			org_id1 = od_graph.convert_id(i, 0);// sequence id to orginal id
			org_id2 = od_graph.convert_id(j, 0);// orginal id to sequence id
			dij = get_poly_distance(org_id1, org_id2, od_graph.layerConnection);
			Aij = Gij / (Pi*Pj);
			y.push_back(Aij);//original x
			x.push_back(dij);//original y

			avg_y += log(Aij);
			avg_x += log(dij);

			sum_xy += log(Aij)*log(dij);
			sum_xx += log(dij)*log(dij);
		}
	}
	res.push_back(x);
	res.push_back(y);
	avg_x /= x.size();
	avg_y /= y.size();
	vector<double> stat;
	//系数b最小二乘估计量
	double b_stat = -(sum_xy - x.size()*avg_x*avg_y) / (sum_xx - x.size()*avg_x*avg_x);
	double a_stat = avg_y + b_stat * avg_x;
	double SSE = 0,SSM=0,SST;
	for (int i = 0; i < x.size(); i++)
	{
		double y_ = -b_stat * log(x[i]) + a_stat; //y_stat
		SSM += (y_ - avg_y)*(y_ - avg_y);
		SSE += (y_ - log(y[i]))*(y_ - log(y[i]));
	}
	SST = SSM + SSE;
	double R2 = SSM / SST;//regression 
	stat.push_back(b_stat);
	stat.push_back(a_stat);
	stat.push_back(SSE);
	stat.push_back(SSM);
	stat.push_back(SST);
	stat.push_back(R2);
	res.push_back(stat);
	return b_stat;
}