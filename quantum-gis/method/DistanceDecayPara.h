#pragma once
#include<iostream>
#include <vector>
#include<list>

#include"../collection/flowcollection.h"
#include "../collection/flowgraph.h"
#include"CalculateBtwId.h"
using namespace std;

double distance_decay_parameter(flowgraph& od_graph, vector<vector<double>> &res);//res[0] -> x; res[1] -> y; res[2] -> {b_stat,a_stat,SSE,SSM,SST,R2}