#pragma once
#include"flowcollection.h"
#include <igraph.h>
#include <stdarg.h>
#include <iostream>

class flowgraph
{
public:
	flowgraph();
	flowgraph(flowcollection* flowcol);
	~flowgraph();


	igraph_t mygraph;
	igraph_vector_t v_dict_to_seq; //VECTOR( v_dict_to_seq)[org_id] -> sequence id
	igraph_vector_t v_dict_to_org; //VECTOR(v_dict_to_org)[seq_id] -> original id
	igraph_matrix_t adja_mat;
	OGRLayer * layerConnection;

	//graph properties
	int vcount;// number of vertices 
	int ecount;//number of edges
	double average_shortest_path;
	double diameter;
	igraph_bool_t IsDirected; 
	

	int get_vcount();
	int get_ecount();
	void print();
	void print_dict_to_seq();
	void print_dict_to_org();
	void calculate_properties();
	void get_degree(vector<int>&);
	int get_degree_byid(int vid, bool org_or_not);//use original id, org_or_not = 1;sequencr id ,org_or_not = 0
	double get_flow_byid(int src_id, int tar_id, bool org_or_not);
	int convert_id(int vid, bool is_org_to_seq);	//transfrom orgin id -> sequence id :org_to_seq = 1; sequenct id -> origin id :org_to_seq = 0 

};

