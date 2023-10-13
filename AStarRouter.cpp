#include "AStarRouter.h"

AStarNode::AStarNode(HananEdge* edge, float g, float h)
{
	this->edge = edge;
	this->g = g;
	this->h = h;
	this->f = g + h;
	this->parent = NULL;
}

AStarNode::AStarNode(HananEdge* edge, float g, float h, AStarNode* parent)
{
	this->edge = edge;
	this->g = g;
	this->h = h;
	this->f = g + h;
	this->parent = parent;
}

void AStarRouter::a_star_router(HananEdge* start_edge, HananEdge* end_edge, float width, vector<HananEdge*>& path)
{
	// DEBUG: path file
	ofstream fout;
	this->create_path_file(fout);

	AStarNode* start = new AStarNode(start_edge, 0, 
		diagonal_distance(start_edge->root_node_edge()->center_position(), end_edge->root_node_edge()->center_position()));
	priority_queue<AStarNode*, vector<AStarNode*>, cmp1> open_set;
	unordered_map<HananEdge*, AStarNode*> closed_set;
	//queue<AStarNode*> closed_set;
	path.clear();
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
		
		// DEBUG: path file
		this->write_path_file(fout, current);

		open_set.pop();
		
		if (current->edge == end_edge)
		{
			// has found a path
			for (AStarNode* edge_node = current; edge_node != NULL; edge_node = edge_node->parent)
			{
				path.push_back(edge_node->edge);
			}
			// delete all the AStarNode
			unordered_map<HananEdge*, AStarNode*>::iterator it = closed_set.begin();
			while (it != closed_set.end()/*!closed_set.empty()*/)
			{
				AStarNode* node = (*it).second;
				/*AStarNode* node = closed_set.front();
				closed_set.pop();*/
				delete node;
				it++;
			}
			closed_set.clear();

			while (!open_set.empty())
			{
				AStarNode* node = open_set.top();
				open_set.pop();
				delete node;
			}

			// DEBUG: path file
			this->close_path_file(fout);
			
			break;
		}

		for (auto& routing_edge : current->edge->adjacent_edges[0])
		{
			if (closed_set.find(routing_edge->adjvex) != closed_set.end())
			{
				continue;
			}
			/*if (!closed_set.empty() and closed_set.back()->edge == routing_edge->adjvex)
			{
				continue;
			}*/
			float routing_weight = (routing_edge->conflict_id.empty()) ? routing_edge->weight : 1000;
			float capacity_weight = (width < routing_edge->adjvex->root_node_edge()->capacity) ? 0 : 1000;
			AStarNode* neighbor = new AStarNode(routing_edge->adjvex, (current->g + routing_weight + capacity_weight), 
				diagonal_distance(routing_edge->adjvex->root_node_edge()->center_position(), end_edge->root_node_edge()->center_position()), 
				current);
			open_set.push(neighbor);
		}
		for (auto& routing_edge : current->edge->adjacent_edges[1])
		{
			if (closed_set.find(routing_edge->adjvex) != closed_set.end())
			{
				continue;
			}
			/*if (!closed_set.empty() and closed_set.back()->edge == routing_edge->adjvex)
			{
				continue;
			}*/
			
			float routing_weight = (routing_edge->conflict_id.empty()) ? routing_edge->weight : 1000;
			float capacity_weight = (width < routing_edge->adjvex->root_node_edge()->capacity) ? 0 : 1000;
			AStarNode* neighbor = new AStarNode(routing_edge->adjvex, (current->g + routing_weight + capacity_weight), 
				diagonal_distance(routing_edge->adjvex->root_node_edge()->center_position(), end_edge->root_node_edge()->center_position()), 
				current);
			open_set.push(neighbor);
		}

		closed_set[current->edge] = current;
		/*closed_set.push(current);*/
	}
}

void AStarRouter::create_path_file(ofstream& fout, string filename)
{
	fout.open(filename, ios::trunc);
}

void AStarRouter::write_path_file(ofstream& fout, AStarNode* current)
{
	vector<Position> path_pos;
	for (AStarNode* edge_node = current; edge_node != NULL; edge_node = edge_node->parent)
	{
		path_pos.push_back(edge_node->edge->center_position());
	}
	int pos_size = path_pos.size();
	for (int i = pos_size - 1; i >= 0; i--)
	{
		fout << "(" << path_pos[i].x << ", " << path_pos[i].y << ")" << "->";
	}
	fout << endl;
}

void AStarRouter::close_path_file(ofstream& fout)
{
	if (fout.is_open())
	{
		fout.close();
	}
}
