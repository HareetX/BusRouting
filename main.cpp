#pragma once

#include "HananRoutingGraph.h"
#include "AStarRouter.h"

int main()
{
	// boundary points of the board
	float board[2][2] = { {0, 0},{36, 27} };
	
	// boundary points of the components
	vector<float(*)[2]> component_list;
	
	float component1[2][2] = { {7, 5}, {12, 8} };
	component_list.push_back(component1);

	//float component2[2][2] = { {7, 12}, {21, 17} };
	//component_list.push_back(component2);
	//float component2[2][2] = { {15, 5}, {21, 17} };
	//component_list.push_back(component2);
	float component2[2][2] = { {17, 7}, {21, 12} };
	component_list.push_back(component2);

	float component3[2][2] = { {7, 15}, {21, 20} };
	component_list.push_back(component3);

	float component4[2][2] = { {25, 7}, {32, 16} };
	component_list.push_back(component4);

	// Initial the Routing Graph based on Hanan Grid
	HananRoutingGraph hanan_routing_graph(board, component_list);

	// Find the path by A* Searching
	AStarRouter router;
	vector<HananEdge*> path;
	router.a_star_router(hanan_routing_graph.VerticalEdgeMap.at(7)[5], hanan_routing_graph.HorizontalEdgeMap.at(16)[2], path);

	// update the path to the Routing Graph dynamically
	hanan_routing_graph.updateRoutingGraph(path);

	//Print Routing Graph with the path;
	hanan_routing_graph.printRoutingPath(path);

	return 0;
}