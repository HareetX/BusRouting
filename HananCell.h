#pragma once
#include "ultis.h"
#include "HananEdge.h"

class RoutingArea
{
public:
	RoutingArea() {};
	~RoutingArea() {};

	vector<HananEdge*> contain_edges;
	//vector<RoutingEdge*> contain_routing_edges;

	bool is_inner_edge(HananEdge* edge);
};

class HananCell : public Cell
{
public:
	HananCell(HananEdge* edge_1, HananEdge* edge_2, HananEdge* edge_3, HananEdge* edge_4) : Cell(edge_1, edge_2, edge_3, edge_4) {};
	~HananCell() {};

	vector<RoutingArea*> routing_area_list;

	vector<pair<Line, int>> path_line;

	RoutingArea* current_routing_area(HananEdge* edge);
};

