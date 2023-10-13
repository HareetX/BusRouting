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
	HananRoutingGraph(Position board[2], vector<Position*>& component_list);
	
	vector<HananVertex*> VertexList;
	vector<ComponentVertex*> ComponentVertexList;
	vector<HorizontalEdge*> HorizontalEdgeList;
	vector<VerticalEdge*> VerticalEdgeList;

	map<float, vector<HorizontalEdge*>> HorizontalEdgeMap;
	map<float, vector<VerticalEdge*>> VerticalEdgeMap;

	vector<HananCell*> CellList;

	vector<vector<HananEdge*>> pathList;

	void updateRoutingGraph(vector<HananEdge*>& path, Position start, Position end, float width, int bus_id);

	HananEdge* findBusTerminalEdge(Position bus_terminal);

	void printHananGrid();
	void printRoutingGraph();
	void printRoutingPath();

private:
	// Initial the Vertex and Edge of the Board and Component
	void initialBoard(Position boundary_point_1, Position boundary_point_2);
	void initialComponent(Position boundary_point_1, Position boundary_point_2);

	// Generate HananGrid from Component Vertex
	void generateHananGrid();

	// Generate Hanan Cell and Routing Graph
	void initialHananCellH(HorizontalEdge* edge);

	void initialAdjacentEdgeH(HorizontalEdge* edge);
	void initialAdjacentEdgeV(VerticalEdge* edge);

	void generateHananRoutingGraph();

	HananCell* findHananCell(HananEdge* edge1, HananEdge* edge2);
};

