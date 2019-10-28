#include "flowgraph.h"



flowgraph::flowgraph()
{
	vcount = 0;
	ecount = 0;
	layerConnection = NULL;
}
flowgraph::flowgraph(flowcollection* flowcol)
{
	IsDirected = IGRAPH_DIRECTED;
	layerConnection = flowcol->layerConnection;
	int max_org_id = flowcol->Countflowid();//set initially 
	igraph_vector_init(&v_dict_to_seq, max_org_id);
	igraph_vector_init(&v_dict_to_org, max_org_id);
	bool contain_zero_id = FALSE;
	flowcollection* flows = flowcol;
	int n_flow = flows->Countflow();
	//Initiate the dictionaries
	for (int i = 0; i < n_flow; i++)
	{
		flowdata flow = flows->Getflow(i);
		int vid = flow.OID;
		if (vid > max_org_id)
		{
			max_org_id = vid;
			igraph_vector_resize(&v_dict_to_seq, vid + 1);
		}
		VECTOR(v_dict_to_seq)[vid] = vid;

		if (vid == 0)
			contain_zero_id = 1;

		//the same to DID
		vid = flow.DID;
		if (vid > max_org_id)
		{
			max_org_id = vid;
			igraph_vector_resize(&v_dict_to_seq, vid + 1);
		}
		VECTOR(v_dict_to_seq)[vid] = vid;

		if (vid == 0)
			contain_zero_id = 1;

	}
	//Initiate dictionaries
	int k = 0;
	if (contain_zero_id)
	{
		VECTOR(v_dict_to_seq)[0] = k;
		VECTOR(v_dict_to_org)[k] = 0;
		k++;
	}
	for (int i = 1; i < igraph_vector_size(&v_dict_to_seq); i++)
	{
		if (VECTOR(v_dict_to_seq)[i] == 0) continue;
		VECTOR(v_dict_to_seq)[i] = k;
		VECTOR(v_dict_to_org)[k] = i;
		k++;
	}
	max_org_id = igraph_vector_max(&v_dict_to_org);
	
	//cout << "size of v_dict_to_seq:" << igraph_vector_size(&v_dict_to_seq) << endl
	//	<< "size of v_dict_to_org:" << igraph_vector_size(&v_dict_to_org) << endl;
	//
	igraph_vector_resize_min(&v_dict_to_seq);
	igraph_vector_resize(&v_dict_to_org, igraph_vector_max(&v_dict_to_seq) + 1);
	//cout << "size of v_dict_to_seq:" << igraph_vector_size(&v_dict_to_seq) << endl
	//	<< "size of v_dict_to_org:" << igraph_vector_size(&v_dict_to_org) << endl;

	igraph_matrix_init(&adja_mat, igraph_vector_size(&v_dict_to_org), igraph_vector_size(&v_dict_to_org));
	for (int i = 0; i < n_flow; i++)
	{
		flowdata flow = flows->Getflow(i);
		int row = VECTOR(v_dict_to_seq)[flow.OID];
		int col = VECTOR(v_dict_to_seq)[flow.DID];
		MATRIX(adja_mat,row,col ) = flow.weight;
	}
	igraph_i_set_attribute_table(&igraph_cattribute_table);
	igraph_weighted_adjacency(&mygraph, &adja_mat, IGRAPH_ADJ_DIRECTED, 0, /*loops=*/ 1);

	//print_dict_to_org();
	//print_dict_to_seq();
	//print();
	vcount = igraph_vcount(&mygraph);
	ecount = igraph_ecount(&mygraph);
}

flowgraph::~flowgraph()
{
	igraph_destroy(&mygraph);
	igraph_vector_destroy(&v_dict_to_org);
	igraph_vector_destroy(&v_dict_to_seq);
	cout << IGRAPH_FINALLY_STACK_SIZE() << endl;
}


void flowgraph::print() {
	igraph_t *g = &mygraph;
	igraph_vector_t el;
	long int i, j, n;
	char ch = igraph_is_directed(g) ? '>' : '-';

	igraph_vector_init(&el, 0);
	igraph_get_edgelist(g, &el, 0);
	n = igraph_ecount(g);

	for (i = 0, j = 0; i < n; i++, j += 2) {
		int a = (int)VECTOR(el)[j];
		a = (int)VECTOR(el)[j + 1];
		printf("%ld --%c %ld: %ld\n",
			(int)VECTOR(v_dict_to_org)[(int)VECTOR(el)[j]]
			, ch, (int)VECTOR(v_dict_to_org)[(int)VECTOR(el)[j + 1]], (long)EAN(g, "weight", i));
	}
	printf("\n");

	igraph_vector_destroy(&el);
}

void flowgraph::print_dict_to_seq() {
	cout << endl << "v_dict org to seq:" << endl;
	for (int i = 0; i < igraph_vector_size(&v_dict_to_seq); i++)
	{
		cout << i << "-" <<  VECTOR(v_dict_to_seq)[i] << " ";
	}
}
void flowgraph::print_dict_to_org() {
	cout << endl << "v_dict seq to org:" << endl;
	for (int i = 0; i < igraph_vector_size(&v_dict_to_org); i++)
	{
		cout << i<< "-"<< VECTOR(v_dict_to_org)[i]<<" ";
	}
}

int flowgraph::get_vcount() 
{
	return vcount;
}
int flowgraph::get_ecount()
{
	return ecount;
}

void flowgraph::calculate_properties()
{
	//average path length
	igraph_real_t tmp;
	igraph_average_path_length(&mygraph, &tmp,
		IsDirected, 1);
	average_shortest_path = (double)tmp;
	//diameter
	igraph_integer_t d;
	igraph_diameter(&mygraph, &d,
		/*pfrom*/NULL, /*pto*/NULL,
		/*igraph_vector_t *path */ NULL,
		IsDirected, 1);
	diameter = (int)d;
	//
	
	
}

void flowgraph::get_degree(vector<int>& v_degree)
{
	igraph_vector_t degree;
	igraph_vector_init(&degree, vcount);
	igraph_strength(&mygraph, &degree,
		igraph_vss_all(), /*IGRAPH_IN  IGRAPH_OUT*/ IGRAPH_ALL,
		IGRAPH_NO_LOOPS, /*const igraph_vector_t *weights*/ NULL);

	for (int i = 0; i < vcount; i++)
	{
		int deg = (int)VECTOR(degree)[i];
		//cout << i << ": " << VECTOR(degree)[i] << endl;
		v_degree.push_back(deg);
	}
	igraph_vector_destroy(&degree);
}

int flowgraph::get_degree_byid(int vid, bool org_or_not) //, igraph_neimode_t mode)
{

	//igraph_vector_init(degree, vcount);
	igraph_vector_t v;
	igraph_vector_init(&v, 1);
	igraph_integer_t seq_id;
	if (org_or_not)
	{
		seq_id = VECTOR(v_dict_to_seq)[vid];
	}
	else
	{
		seq_id = (igraph_integer_t)vid;
	}
	igraph_strength(&mygraph, &v, igraph_vss_1(seq_id), /*IGRAPH_IN  IGRAPH_OUT*/ IGRAPH_ALL,
		IGRAPH_NO_LOOPS/*IGRAPH_LOOPS*/, /*const igraph_vector_t *weights*/ NULL);

	int degree = (int)VECTOR(v)[0];
	igraph_vector_destroy(&v);
	return degree;
}

double flowgraph::get_flow_byid(int src_id, int tar_id, bool org_or_not)
{
	igraph_integer_t row;
	igraph_integer_t col;
	if (org_or_not)
	{
		row = VECTOR(v_dict_to_seq)[src_id];
		col = VECTOR(v_dict_to_seq)[tar_id];
	}
	else
	{
		row = (igraph_integer_t)src_id;
		col = (igraph_integer_t)tar_id;
	}
	
	double flow = (double)MATRIX(adja_mat, row, col);
	return flow;
}
int flowgraph::convert_id(int vid, bool is_org_to_seq)
{
	if (is_org_to_seq)
	{
		return (int)VECTOR(v_dict_to_seq)[vid];
	}
	else
	{
		return (int)VECTOR(v_dict_to_org)[vid];
	}

};

//
//int test()
//{
//	igraph_t g;
//	igraph_matrix_t mat;
//	int m[4][4] = { { 0, 1, 2, 0 }, { 2, 0, 0, 1 }, { 0, 0, 1, 0 }, { 0, 1, 0, 0 } };
//	long int i, j;
//
//	igraph_matrix_init(&mat, 4, 4);
//	for (i = 0; i < 4; i++) for (j = 0; j < 4; j++) MATRIX(mat, i, j) = m[i][j];
//	igraph_i_set_attribute_table(&igraph_cattribute_table);
//
//	/* [ 0 1 2 0 ]
//	   [ 2 0 0 1 ]
//	   [ 0 0 1 0 ]
//	   [ 0 1 0 0 ] */
//	igraph_weighted_adjacency(&g, &mat, IGRAPH_ADJ_DIRECTED, 0, /*loops=*/ 1);
//	print(&g);
//	igraph_destroy(&g);
//
//	/* [ 0 1 2 0 ]
//	   [ - 0 0 1 ]
//	   [ - - 1 0 ]
//	   [ - - - 0 ] */
//	igraph_weighted_adjacency(&g, &mat, IGRAPH_ADJ_UPPER, 0, /*loops=*/ 1);
//	print(&g);
//	igraph_destroy(&g);
//
//	/* [ 0 - - - ]
//	   [ 2 0 - - ]
//	   [ 0 0 1 - ]
//	   [ 0 1 0 0 ] */
//	igraph_weighted_adjacency(&g, &mat, IGRAPH_ADJ_LOWER, 0, /*loops=*/ 1);
//	print(&g);
//	igraph_destroy(&g);
//
//	/* [ 0 1 0 0 ]
//	   [ 1 0 0 1 ]
//	   [ 0 0 1 0 ]
//	   [ 0 1 0 0 ] */
//	igraph_weighted_adjacency(&g, &mat, IGRAPH_ADJ_MIN, 0, /*loops=*/ 1);
//	print(&g);
//	igraph_destroy(&g);
//
//	/* [ 0 2 2 0 ]
//	   [ 2 0 0 1 ]
//	   [ 2 0 1 0 ]
//	   [ 0 1 0 0 ] */
//	igraph_weighted_adjacency(&g, &mat, IGRAPH_ADJ_MAX, 0, /*loops=*/ 1);
//	print(&g);
//	igraph_destroy(&g);
//
//	/* [ 0 3 2 0 ]
//	   [ 3 0 0 2 ]
//	   [ 2 0 1 0 ]
//	   [ 0 2 0 0 ] */
//	igraph_weighted_adjacency(&g, &mat, IGRAPH_ADJ_PLUS, 0, /*loops=*/ 1);
//	print(&g);
//	igraph_destroy(&g);
//
//	igraph_matrix_destroy(&mat);
//
//	if (IGRAPH_FINALLY_STACK_SIZE() != 0) return 1;
//
//}