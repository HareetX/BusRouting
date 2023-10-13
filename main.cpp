#pragma once
#include "BusRoutingParameters.h"
#include "HananRoutingGraph.h"
#include "AStarRouter.h"

int main()
{
	// boundary points of the board
	Position board[2] = { Position{0, 0},Position{36, 27} };
	
	// boundary points of the components
	vector<Position*> component_list;

	Position component1[2] = { Position{7, 5}, Position{12, 8} };
	component_list.push_back(component1);

	//Position component2[2] = { Position{7, 12}, Position{21, 17} };
	//component_list.push_back(component2);
	//Position component2[2] = { Position{15, 5}, Position{21, 17} };
	//component_list.push_back(component2);
	Position component2[2] = { Position{17, 7}, Position{21, 12} };
	component_list.push_back(component2);

	Position component3[2] = { Position{7, 15}, Position{21, 20} };
	component_list.push_back(component3);

	Position component4[2] = { Position{25, 7}, Position{32, 16} };
	component_list.push_back(component4);

	BusRoutingParameters parameters(board, component_list);
	Bus bus1 = Bus(Position{ 7,6 }, Position{ 28,16 }, 2.5, 0);
	Bus bus2 = Bus(Position{ 21,18 }, Position{ 30,16 }, 1.5, 1);
	Bus bus3 = Bus(Position{ 17,10 }, Position{ 7,18 }, 3, 2);
	parameters.add_bus(bus1);
	parameters.add_bus(bus2);
	parameters.add_bus(bus3);

	// Initial the Routing Graph based on Hanan Grid
	HananRoutingGraph hanan_routing_graph(parameters.board, parameters.component_list);

	// Global Routing based on A* Searching
	AStarRouter router;
	vector<HananEdge*> path;
	
	for (auto& bus : parameters.bus_list)
	{
		// Find the path by A* Searching
		router.a_star_router(hanan_routing_graph.findBusTerminalEdge(bus.start), hanan_routing_graph.findBusTerminalEdge(bus.end), bus.width, path);

		// update the path to the Routing Graph dynamically
		hanan_routing_graph.updateRoutingGraph(path, bus.start, bus.end, bus.width, 0);

		//// Print Routing Graph with the path;
		//hanan_routing_graph.printRoutingPath();
	}
	// Print Routing Graph with the path;
	hanan_routing_graph.printRoutingPath();
	
	// Store the bus path
	//parameters.add_path

	return 0;
}