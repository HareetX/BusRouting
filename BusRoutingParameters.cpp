#include "BusRoutingParameters.h"

void BusRoutingParameters::add_bus(Bus& bus)
{
	this->bus_list.push_back(bus);
}

void BusRoutingParameters::add_path(vector<Position*>& path)
{
	this->path_list.push_back(path);
}

