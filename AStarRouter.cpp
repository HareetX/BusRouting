#include "AStarRouter.h"

AStarNode::AStarNode(HananEdge* edge, float g)
{
	this->edge = edge;
	this->g = g;
	this->f = g;
	this->parent = NULL;
}

AStarNode::AStarNode(HananEdge* edge, float g, AStarNode* parent)
{
	this->edge = edge;
	this->g = g;
	this->f = g;
	this->parent = parent;
}

void AStarRouter::a_star_router(HananEdge* start_edge, HananEdge* end_edge, vector<HananEdge*>& path)
{
	AStarNode* start = new AStarNode(start_edge, 0);
	priority_queue<AStarNode*, vector<AStarNode*>, cmp1> open_set;
	queue<AStarNode*> closed_set;
	open_set.push(start);
	while (true)
	{
		if (open_set.empty())
		{
			// fail to find a routing path
			cout << "fail to find a path!" << endl;
			break;
		}
		AStarNode* current = open_set.top();
		open_set.pop();
		
		if (current->edge == end_edge)
		{
			// has found a path
			for (AStarNode* edge_node = current; edge_node != NULL; edge_node = edge_node->parent)
			{
				path.push_back(edge_node->edge);
			}
			// delete all the AStarNode
			while (!closed_set.empty())
			{
				AStarNode* node = closed_set.front();
				closed_set.pop();
				delete node;
			}
			while (!open_set.empty())
			{
				AStarNode* node = open_set.top();
				open_set.pop();
				delete node;
			}
			break;
		}
		for (auto& routing_edge : current->edge->adjacent_edges)
		{
			if (!closed_set.empty() and closed_set.back()->edge == routing_edge.adjvex)
			{
				continue;
			}
			AStarNode* neighbor = new AStarNode(routing_edge.adjvex, (current->g + routing_edge.weight), current);
			open_set.push(neighbor);
		}

		closed_set.push(current);
	}
}
