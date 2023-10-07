#pragma once
#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>

#include "HananVertex.h"
#include "HananEdge.h"
#include "HananCell.h"

#include "matplotlibcpp.h"

using namespace std;
namespace plot = matplotlibcpp;

bool HorizontalCompLess1(const HorizontalEdge* edge, const ComponentVertex* vertex);

bool HorizontalCompLess2(const ComponentVertex* vertex, const HorizontalEdge* edge);

bool VerticalCompLess1(const VerticalEdge* edge, const ComponentVertex* vertex);

bool VerticalCompLess2(const ComponentVertex* vertex, const VerticalEdge* edge);

class HananRoutingGraph
{
public:
	HananRoutingGraph(float board[2][2], vector<float(*)[2]>& component_list);
	
	vector<HananVertex*> VertexList;
	vector<ComponentVertex*> ComponentVertexList;
	vector<HorizontalEdge*> HorizontalEdgeList;
	vector<VerticalEdge*> VerticalEdgeList;

	map<float, vector<HorizontalEdge*>> HorizontalEdgeMap;
	map<float, vector<VerticalEdge*>> VerticalEdgeMap;

	vector<HananCell*> CellList;

	vector<vector<HananEdge*>> pathList;

	void updateRoutingGraph(vector<HananEdge*>& path, int bus_id);

	void printHananGrid();
	void printRoutingGraph();
	void printRoutingPath();

private:
	// Initial the Vertex and Edge of the Board and Component
	void initialBoard(float boundary_point_1[2], float boundary_point_2[2]);
	void initialComponent(float boundary_point_1[2], float boundary_point_2[2]);

	// Generate HananGrid from Component Vertex
	void generateHananGrid();

	// Generate Hanan Cell and Routing Graph
	void initialHananCellH(HorizontalEdge* edge);

	void initialAdjacentEdgeH(HorizontalEdge* edge);
	void initialAdjacentEdgeV(VerticalEdge* edge);

	void generateHananRoutingGraph();

	HananCell* findHananCell(HananEdge* edge1, HananEdge* edge2);
};

