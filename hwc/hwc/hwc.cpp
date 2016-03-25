
#include "stdafx.h"
#include <vector>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <map>
#include <iterator>
using namespace std;
struct vertex
{
	int vertex_id;
	vector<int> connected_vertex_id;
};
struct edge
{
	int edge_id;
	int a; int b;
	int value;
};
struct MGraph
{
	vector<int> vertex_id;
	vector<edge> edges;// the edge and its weight
	int vertex_num, edge_num;
};
void import_edge_info(string file_name, vector<edge>& edges, vector<int>& topo_vertice, map<int, vector<int>>& vertice)
{
	ifstream topo_file(file_name);
	int edge_id = 0;//edge id initialize to 0
	string line;
	while (getline(topo_file, line))
	{
		int temp_count = 1; int temp_value = 0; int temp_in_id = 0; int temp_out_id = 0;
		stringstream linestream(line);	string topo_info;
		while (getline(linestream, topo_info, ','))
		{
			int int_number = atoi(topo_info.c_str());
			if (temp_count == 1)
			{
				temp_count++;
				continue;
			}
			else if (temp_count == 2)
			{
				topo_vertice.push_back(int_number);
				temp_in_id = int_number;
			}
			else if (temp_count == 3)
			{
				topo_vertice.push_back(int_number);
				temp_out_id = int_number;
			}
			else if (temp_count == 4)
			{
				temp_value = int_number;
			}
			temp_count++;
		}
		vertice[temp_in_id].push_back(temp_out_id);
		edge temp_edge = { edge_id, temp_in_id, temp_out_id, temp_value };
		edges.push_back(temp_edge);
		edge_id++;
	}
	std::sort(topo_vertice.begin(), topo_vertice.end());
	topo_vertice.erase(unique(topo_vertice.begin(), topo_vertice.end()), topo_vertice.end());
	for (size_t i = 0; i < topo_vertice.size(); i++)
	{
		int temp_vertex_id = topo_vertice[i];
		map<int, vector<int>>::iterator iter = vertice.find(temp_vertex_id);
		if (iter == vertice.end())
		{
			vertice[temp_vertex_id].push_back(-1);
		}
	}

}

void import_demand_info(string file_name, int& start_id, int& end_id, vector<int>& demand_vertice)
{
	ifstream demand_file(file_name);
	string demand_info;
	int temp_count = 1;
	while (getline(demand_file, demand_info, ','))
	{
		if (temp_count == 1)
		{
			int index = atoi(demand_info.c_str());
			start_id = index;
			temp_count++;
		}
		else if (temp_count == 2)
		{
			int index = atoi(demand_info.c_str());
			end_id = index;
			temp_count++;
		}
		else if (temp_count == 3)
		{
			stringstream demand_points(demand_info);
			string pt;
			while (getline(demand_points, pt, '|'))
			{
				demand_vertice.push_back(atoi(pt.c_str()));
			}
		}
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	/* first we import the data and prepare for the later handling*/

	MGraph topo_graph;
	//int inf = 99999999;// infinite 
	vector<int> topo_vertice, demand_vertice;
	vector<edge> edges;
	map<int, vector<int>> vertice;
	import_edge_info("topo.csv", edges, topo_vertice, vertice);
	//for (size_t i = 0; i < edges.size(); i++)
	//{
	//	edge temp_edge = edges[i];
	//	cout << temp_edge.edge_id << " " << temp_edge.a << " " << temp_edge.b << " " << temp_edge.value << endl;
	//}
	//for (size_t i = 0; i < topo_vertice.size(); i++)
	//{
	//	cout << topo_vertice[i] << " ";
	//}
	//for (map<int, vector<int>>::iterator iter = vertice.begin(); iter != vertice.end(); iter++)
	//{
	//	cout << iter->first << " ";
	//	for (size_t i = 0; i < (iter->second).size(); i++)
	//	{
	//		cout << (iter->second)[i] << " ";
	//	}
	//	cout << endl;
	//}
	int start_id, end_id;
	import_demand_info("demand.csv", start_id, end_id, demand_vertice);
	//cout << start_id << " " << end_id << endl;
	//for (size_t j = 0; j < demand_vertice.size(); j++)
	//{
	//	cout << demand_vertice[j] << " ";
	//}
	
	// distance matrix representation
	vector<int> dist; int inf = 99999999;
	int vertice_num = topo_vertice.size();
	for (int i = 0; i < vertice_num; i++)
	{
		for (int j = 0; j < vertice_num; j++)
		{
			if (i == j)
				dist.push_back(0);
			else
				dist.push_back(inf);
		}
	}
	for (size_t i = 0; i < edges.size(); i++)
	{
		int temp_a = edges[i].a; int temp_b = edges[i].b;
		dist[temp_a*vertice_num + temp_b] = edges[i].value;
	}
	for (int i = 0; i < vertice_num*vertice_num; i++)
	{
		cout << dist[i] << " ";
	}
	cout << endl;
	for (int k = 0; k < vertice_num; k++)
	{
		for (int i = 0; i < vertice_num; i++)
		{
			for (int j = 0; j < vertice_num; j++)
			{
				if (dist[i*vertice_num + j] > dist[i*vertice_num + k] + dist[k*vertice_num + j])
				{
					dist[i*vertice_num + j] = dist[i*vertice_num + k] + dist[k*vertice_num + j];
				}
			}
		}
	}
	for (int i = 0; i < vertice_num*vertice_num; i++)
	{
		cout << dist[i] << " ";
	}

	getchar();
	return 0;

}

