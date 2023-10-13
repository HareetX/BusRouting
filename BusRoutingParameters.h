#pragma once
#include "Bus.h"

class BusRoutingParameters
{
public:
	BusRoutingParameters(Position board[2])
	{
		this->board[0] = board[0];
		this->board[1] = board[1];
	};
	BusRoutingParameters(Position board[2], vector<Position*>& component_list)
	{
		this->board[0] = board[0];
		this->board[1] = board[1];

		this->component_list = component_list;
	};
	BusRoutingParameters(Position board[2], vector<Position*>& component_list, vector<Bus>& bus_list)
	{
		this->board[0] = board[0];
		this->board[1] = board[1];

		this->component_list = component_list;

		this->bus_list = bus_list;
	};
	
	Position board[2];
	vector<Position*> component_list;
	
	vector<Bus> bus_list;
	
	vector<vector<Position*>> path_list;

	void add_bus(Bus& bus);

	void add_path(vector<Position*>& path);

private:

};

