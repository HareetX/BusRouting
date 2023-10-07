#include "HananCell.h"

RoutingArea* HananCell::current_routing_area(HananEdge* edge)
{
    for (auto& area : this->routing_area_list)
    {
        for (auto& edge_node : area->contain_edges)
        {
            if (edge == edge_node)
            {
                return area;
            }
        }
    }
    return nullptr;
}

bool RoutingArea::is_inner_edge(HananEdge* edge)
{
    for (auto& inner_edge : this->contain_edges)
    {
        if (edge == inner_edge)
        {
            return true;
        }
    }
    return false;
}
