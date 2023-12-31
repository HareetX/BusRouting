#include "HananEdge.h"

bool HorizontalCompLess(const HorizontalEdge* a, const HorizontalEdge* b)
{
	return a->horizontal_coord < b->horizontal_coord;
}

bool VerticalCompLess(const VerticalEdge* a, const VerticalEdge* b)
{
	return a->vertical_coord < b->vertical_coord;
}

bool HorizontalVertexCompLess(const Vertex* a, const Vertex* b)
{
	return a->position.x < b->position.x;
}

bool VerticalVertexCompLess(const Vertex* a, const Vertex* b)
{
	return a->position.y < b->position.y;
}

bool HorizontalEdge::is_inner_vertex(Position vertex_pos)
{
	return this->vertexs[0]->position.x < vertex_pos.x && vertex_pos.x < this->vertexs[1]->position.x;
}

void HorizontalEdge::add_inner_vertex(Vertex* vertex)
{
	this->inner_vertexs.insert(lower_bound(this->inner_vertexs.begin(), this->inner_vertexs.end(), vertex, HorizontalVertexCompLess), vertex);
}

void HorizontalEdge::disassemble_edge(map<float, vector<HorizontalEdge*>>& horizontal_edges_map)
{
	// if the key is not existed, create the item
	if (horizontal_edges_map.find(this->horizontal_coord) == horizontal_edges_map.end())
	{
		vector<HorizontalEdge*> horizontal_edges;
		horizontal_edges_map[this->horizontal_coord] = horizontal_edges;
	}
	// diasassemble the edge by inner vertexs
	Vertex* vertex1 = this->vertexs[0];
	Vertex* vertex2;
	for (auto vertex : this->inner_vertexs)
	{
		vertex2 = vertex;
		HorizontalEdge* edge = new HorizontalEdge(vertex1, vertex2, vertex2->position.x - vertex1->position.x, this->edge_type);
		// associate vertex and edge
		vertex1->edges[1] = edge;  // right edge of vertex1
		vertex2->edges[3] = edge;  // left  edge of vertex2
		// add edge to horizontal edges list
		horizontal_edges_map[this->horizontal_coord].push_back(edge);
		vertex1 = vertex;
	}
	vertex2 = this->vertexs[1];
	HorizontalEdge* edge = new HorizontalEdge(vertex1, vertex2, vertex2->position.x - vertex1->position.x, this->edge_type);
	// associate vertex and edge
	vertex1->edges[1] = edge;  // right edge of vertex1
	vertex2->edges[3] = edge;  // left  edge of vertex2
	// add edge to horizontal edges list
	horizontal_edges_map[this->horizontal_coord].push_back(edge);
}

//void HorizontalEdge::initial_adjacent_edges()
//{
//	if ((this->vertexs[0]->edges[0] and this->vertexs[1]->edges[0]) and (this->edge_type != ComponentBoundary or ((HananEdge*)(this->vertexs[0]->edges[0]))->edge_type != ComponentBoundary))
//	{
//		if (!(this->is_extended_adjvex((HananEdge*)(this->vertexs[0]->edges[0]))))
//		{
//			RoutingEdge* adjacent_edge = new RoutingEdge((HananEdge*)(this->vertexs[0]->edges[0]), 1);
//			this->adjacent_edges[0].push_back(adjacent_edge);
//
//			adjacent_edge = new RoutingEdge(this, 1);
//			((HananEdge*)(this->vertexs[0]->edges[0]))->adjacent_edges[1].push_back(adjacent_edge);
//
//			this->adjacent_edges[0].back()->inverse_edge = ((HananEdge*)(this->vertexs[0]->edges[0]))->adjacent_edges[1].back();
//			((HananEdge*)(this->vertexs[0]->edges[0]))->adjacent_edges[1].back()->inverse_edge = this->adjacent_edges[0].back();
//		}
//		
//		if (!(this->is_extended_adjvex((HananEdge*)(this->vertexs[1]->edges[0]))))
//		{
//			RoutingEdge* adjacent_edge = new RoutingEdge((HananEdge*)(this->vertexs[1]->edges[0]), 1);
//			this->adjacent_edges[0].push_back(adjacent_edge);
//
//			adjacent_edge = new RoutingEdge(this, 1);
//			((HananEdge*)(this->vertexs[1]->edges[0]))->adjacent_edges[0].push_back(adjacent_edge);
//
//			this->adjacent_edges[0].back()->inverse_edge = ((HananEdge*)(this->vertexs[1]->edges[0]))->adjacent_edges[0].back();
//			((HananEdge*)(this->vertexs[1]->edges[0]))->adjacent_edges[0].back()->inverse_edge = this->adjacent_edges[0].back();
//		}
//		
//		if (!(this->is_extended_adjvex((HananEdge*)(this->vertexs[0]->edges[0]->vertexs[0]->edges[1]))))
//		{
//			RoutingEdge* adjacent_edge = new RoutingEdge((HananEdge*)(this->vertexs[0]->edges[0]->vertexs[0]->edges[1]), 1);
//			this->adjacent_edges[0].push_back(adjacent_edge);
//
//			adjacent_edge = new RoutingEdge(this, 1);
//			((HananEdge*)(this->vertexs[0]->edges[0]->vertexs[0]->edges[1]))->adjacent_edges[1].push_back(adjacent_edge);
//
//			this->adjacent_edges[0].back()->inverse_edge = ((HananEdge*)(this->vertexs[0]->edges[0]->vertexs[0]->edges[1]))->adjacent_edges[1].back();
//			((HananEdge*)(this->vertexs[0]->edges[0]->vertexs[0]->edges[1]))->adjacent_edges[1].back()->inverse_edge = this->adjacent_edges[0].back();
//		}
//	}
//	if ((this->vertexs[0]->edges[2] and this->vertexs[1]->edges[2]) and (this->edge_type != ComponentBoundary or ((HananEdge*)(this->vertexs[0]->edges[2]))->edge_type != ComponentBoundary))
//	{
//		if (!(this->is_extended_adjvex((HananEdge*)(this->vertexs[0]->edges[2]))))
//		{
//			RoutingEdge* adjacent_edge = new RoutingEdge((HananEdge*)(this->vertexs[0]->edges[2]), 1);
//			this->adjacent_edges[1].push_back(adjacent_edge);
//
//			adjacent_edge = new RoutingEdge(this, 1);
//			((HananEdge*)(this->vertexs[0]->edges[2]))->adjacent_edges[1].push_back(adjacent_edge);
//
//			this->adjacent_edges[1].back()->inverse_edge = ((HananEdge*)(this->vertexs[0]->edges[2]))->adjacent_edges[1].back();
//			((HananEdge*)(this->vertexs[0]->edges[2]))->adjacent_edges[1].back()->inverse_edge = this->adjacent_edges[1].back();
//		}
//
//		if (!(this->is_extended_adjvex((HananEdge*)(this->vertexs[1]->edges[2]))))
//		{
//			RoutingEdge* adjacent_edge = new RoutingEdge((HananEdge*)(this->vertexs[1]->edges[2]), 1);
//			this->adjacent_edges[1].push_back(adjacent_edge);
//
//			adjacent_edge = new RoutingEdge(this, 1);
//			((HananEdge*)(this->vertexs[1]->edges[2]))->adjacent_edges[0].push_back(adjacent_edge);
//
//			this->adjacent_edges[1].back()->inverse_edge = ((HananEdge*)(this->vertexs[1]->edges[2]))->adjacent_edges[0].back();
//			((HananEdge*)(this->vertexs[1]->edges[2]))->adjacent_edges[0].back()->inverse_edge = this->adjacent_edges[1].back();
//		}
//
//		if (!(this->is_extended_adjvex((HananEdge*)(this->vertexs[0]->edges[2]->vertexs[1]->edges[1]))))
//		{
//			RoutingEdge* adjacent_edge = new RoutingEdge((HananEdge*)(this->vertexs[0]->edges[2]->vertexs[1]->edges[1]), 1);
//			this->adjacent_edges[1].push_back(adjacent_edge);
//
//			adjacent_edge = new RoutingEdge(this, 1);
//			((HananEdge*)(this->vertexs[0]->edges[2]->vertexs[1]->edges[1]))->adjacent_edges[0].push_back(adjacent_edge);
//
//			this->adjacent_edges[1].back()->inverse_edge = ((HananEdge*)(this->vertexs[0]->edges[2]->vertexs[1]->edges[1]))->adjacent_edges[0].back();
//			((HananEdge*)(this->vertexs[0]->edges[2]->vertexs[1]->edges[1]))->adjacent_edges[0].back()->inverse_edge = this->adjacent_edges[1].back();
//		}
//	}
//}

bool VerticalEdge::is_inner_vertex(Position vertex_pos)
{
	return this->vertexs[0]->position.y < vertex_pos.y && vertex_pos.y < this->vertexs[1]->position.y;
}

void VerticalEdge::add_inner_vertex(Vertex* vertex)
{
	this->inner_vertexs.insert(lower_bound(this->inner_vertexs.begin(), this->inner_vertexs.end(), vertex, VerticalVertexCompLess), vertex);
}

void VerticalEdge::disassemble_edge(map<float, vector<VerticalEdge*>>& vertical_edges_map)
{
	// if the key is not existed, create the item
	if (vertical_edges_map.find(this->vertical_coord) == vertical_edges_map.end())
	{
		vector<VerticalEdge*> vertical_edges;
		vertical_edges_map[this->vertical_coord] = vertical_edges;
	}
	// diasassemble the edge by inner vertexs
	Vertex* vertex1 = this->vertexs[0];
	Vertex* vertex2;
	for (auto vertex : this->inner_vertexs)
	{
		vertex2 = vertex;
		VerticalEdge* edge = new VerticalEdge(vertex1, vertex2, vertex2->position.y - vertex1->position.y, this->edge_type);
		// associate vertex and edge
		vertex1->edges[2] = edge;  // down edge of vertex1
		vertex2->edges[0] = edge;  // up   edge of vertex2
		// add edge to horizontal edges list
		vertical_edges_map[this->vertical_coord].push_back(edge);
		vertex1 = vertex;
	}
	vertex2 = this->vertexs[1];
	VerticalEdge* edge = new VerticalEdge(vertex1, vertex2, vertex2->position.y - vertex1->position.y, this->edge_type);
	// associate vertex and edge
	vertex1->edges[2] = edge;  // down edge of vertex1
	vertex2->edges[0] = edge;  // up   edge of vertex2
	// add edge to horizontal edges list
	vertical_edges_map[this->vertical_coord].push_back(edge);
}

//void VerticalEdge::initial_adjacent_edges()
//{
//	if ((this->vertexs[0]->edges[1] and this->vertexs[1]->edges[1]) and (this->edge_type != ComponentBoundary or ((HananEdge*)(this->vertexs[0]->edges[1]))->edge_type != ComponentBoundary))
//	{
//		if (!(this->is_extended_adjvex((HananEdge*)(this->vertexs[0]->edges[1]))))
//		{
//			RoutingEdge* adjacent_edge = new RoutingEdge((HananEdge*)(this->vertexs[0]->edges[1]), 1);
//			this->adjacent_edges[1].push_back(adjacent_edge);
//
//			adjacent_edge = new RoutingEdge(this, 1);
//			((HananEdge*)(this->vertexs[0]->edges[1]))->adjacent_edges[1].push_back(adjacent_edge);
//
//			this->adjacent_edges[1].back()->inverse_edge = ((HananEdge*)(this->vertexs[0]->edges[1]))->adjacent_edges[1].back();
//			((HananEdge*)(this->vertexs[0]->edges[1]))->adjacent_edges[1].back()->inverse_edge = this->adjacent_edges[1].back();
//		}
//
//		if (!(this->is_extended_adjvex((HananEdge*)(this->vertexs[1]->edges[1]))))
//		{
//			RoutingEdge* adjacent_edge = new RoutingEdge((HananEdge*)(this->vertexs[1]->edges[1]), 1);
//			this->adjacent_edges[1].push_back(adjacent_edge);
//
//			adjacent_edge = new RoutingEdge(this, 1);
//			((HananEdge*)(this->vertexs[1]->edges[1]))->adjacent_edges[0].push_back(adjacent_edge);
//
//			this->adjacent_edges[1].back()->inverse_edge = ((HananEdge*)(this->vertexs[1]->edges[1]))->adjacent_edges[0].back();
//			((HananEdge*)(this->vertexs[1]->edges[1]))->adjacent_edges[0].back()->inverse_edge = this->adjacent_edges[1].back();
//		}
//
//		if (!(this->is_extended_adjvex((HananEdge*)(this->vertexs[0]->edges[1]->vertexs[1]->edges[2]))))
//		{
//			RoutingEdge* adjacent_edge = new RoutingEdge((HananEdge*)(this->vertexs[0]->edges[1]->vertexs[1]->edges[2]), 1);
//			this->adjacent_edges[1].push_back(adjacent_edge);
//
//			adjacent_edge = new RoutingEdge(this, 1);
//			((HananEdge*)(this->vertexs[0]->edges[1]->vertexs[1]->edges[2]))->adjacent_edges[0].push_back(adjacent_edge);
//
//			this->adjacent_edges[1].back()->inverse_edge = ((HananEdge*)(this->vertexs[0]->edges[1]->vertexs[1]->edges[2]))->adjacent_edges[0].back();
//			((HananEdge*)(this->vertexs[0]->edges[1]->vertexs[1]->edges[2]))->adjacent_edges[0].back()->inverse_edge = this->adjacent_edges[1].back();
//		}
//	}
//	if ((this->vertexs[0]->edges[3] and this->vertexs[1]->edges[3]) and (this->edge_type != ComponentBoundary or ((HananEdge*)(this->vertexs[0]->edges[3]))->edge_type != ComponentBoundary))
//	{
//		if (!(this->is_extended_adjvex((HananEdge*)(this->vertexs[0]->edges[3]))))
//		{
//			RoutingEdge* adjacent_edge = new RoutingEdge((HananEdge*)(this->vertexs[0]->edges[3]), 1);
//			this->adjacent_edges[0].push_back(adjacent_edge);
//
//			adjacent_edge = new RoutingEdge(this, 1);
//			((HananEdge*)(this->vertexs[0]->edges[3]))->adjacent_edges[1].push_back(adjacent_edge);
//
//			this->adjacent_edges[0].back()->inverse_edge = ((HananEdge*)(this->vertexs[0]->edges[3]))->adjacent_edges[1].back();
//			((HananEdge*)(this->vertexs[0]->edges[3]))->adjacent_edges[1].back()->inverse_edge = this->adjacent_edges[0].back();
//		}
//
//		if (!(this->is_extended_adjvex((HananEdge*)(this->vertexs[1]->edges[3]))))
//		{
//			RoutingEdge* adjacent_edge = new RoutingEdge((HananEdge*)(this->vertexs[1]->edges[3]), 1);
//			this->adjacent_edges[0].push_back(adjacent_edge);
//
//			adjacent_edge = new RoutingEdge(this, 1);
//			((HananEdge*)(this->vertexs[1]->edges[3]))->adjacent_edges[0].push_back(adjacent_edge);
//
//			this->adjacent_edges[0].back()->inverse_edge = ((HananEdge*)(this->vertexs[1]->edges[3]))->adjacent_edges[0].back();
//			((HananEdge*)(this->vertexs[1]->edges[3]))->adjacent_edges[0].back()->inverse_edge = this->adjacent_edges[0].back();
//		}
//
//		if (!(this->is_extended_adjvex((HananEdge*)(this->vertexs[0]->edges[3]->vertexs[0]->edges[2]))))
//		{
//			RoutingEdge* adjacent_edge = new RoutingEdge((HananEdge*)(this->vertexs[0]->edges[3]->vertexs[0]->edges[2]), 1);
//			this->adjacent_edges[0].push_back(adjacent_edge);
//
//			adjacent_edge = new RoutingEdge(this, 1);
//			((HananEdge*)(this->vertexs[0]->edges[3]->vertexs[0]->edges[2]))->adjacent_edges[1].push_back(adjacent_edge);
//
//			this->adjacent_edges[0].back()->inverse_edge = ((HananEdge*)(this->vertexs[0]->edges[3]->vertexs[0]->edges[2]))->adjacent_edges[1].back();
//			((HananEdge*)(this->vertexs[0]->edges[3]->vertexs[0]->edges[2]))->adjacent_edges[1].back()->inverse_edge = this->adjacent_edges[0].back();
//		}
//	}
//}

RoutingEdge::RoutingEdge(HananEdge* adjacent, float w)
{
	this->adjvex = adjacent;
	this->weight = w;
	this->inverse_edge = NULL;
}

Position HananEdge::center_position()
{
	return Position{ (this->vertexs[0]->position.x + this->vertexs[1]->position.x) / (float)2, (this->vertexs[0]->position.y + this->vertexs[1]->position.y) / (float)2 };
}

bool HananEdge::is_extended_adjvex(HananEdge* edge)
{
	bool flag = false;
	for (auto& adjacent_edge : this->adjacent_edges[0])
	{
		if (adjacent_edge->adjvex == edge)
		{
			flag = true;
			return flag;
		}
	}
	for (auto& adjacent_edge : this->adjacent_edges[1])
	{
		if (adjacent_edge->adjvex == edge)
		{
			flag = true;
			return flag;
		}
	}
	return flag;
}

bool HananEdge::is_leaf_node()
{
	return (!this->dynamic_graph_lchild) and (!this->dynamic_graph_rchild);
}

bool HananEdge::is_root_node()
{
	return !this->dynamic_graph_parent;
}

void HananEdge::leaf_node_list(vector<HananEdge*>& edge_list)
{
	/*
	* use pre-order traversal to find the leaf node
	*/
	if (this == NULL) return;
	edge_list.clear();
	stack<HananEdge*> s;
	s.push(this);
	while (!s.empty()) {
		HananEdge* nd = s.top();

		// visit(nd)
		if (nd->is_leaf_node())
		{
			edge_list.push_back(nd);
		}

		s.pop();
		if (nd->dynamic_graph_rchild != NULL)
			s.push(nd->dynamic_graph_rchild);
		if (nd->dynamic_graph_lchild != NULL)
			s.push(nd->dynamic_graph_lchild);
	}
}

HananEdge* HananEdge::root_node_edge()
{
	HananEdge* root_edge = nullptr;
	HananEdge* edge = this;
	while (true)
	{
		if (edge->is_root_node())
		{
			root_edge = edge;
			break;
		}
		edge = edge->dynamic_graph_parent;
	}
	return root_edge;
}

void HananEdge::split2child(float width)
{
	Vertex* center_vertex = new Vertex((this->vertexs[0]->position.x + this->vertexs[1]->position.x) / (float)2.0, (this->vertexs[0]->position.y + this->vertexs[1]->position.y) / (float)2.0);
	this->inner_center_vertexs.push_back(center_vertex);

	this->root_node_edge()->capacity -= width;

	HananEdge* lchild = new HananEdge(this->vertexs[0], center_vertex, -1, this->edge_type, this);
	lchild->cells[0] = this->cells[0];
	lchild->cells[1] = this->cells[1];
	
	HananEdge* rchild = new HananEdge(center_vertex, this->vertexs[1], -1, this->edge_type, this);
	rchild->cells[0] = this->cells[0];
	rchild->cells[1] = this->cells[1];
	
	//delete adjacent edges : edges -> this
	//for (auto& this2edge : this->adjacent_edges[0])
	//{
	//	vector<RoutingEdge*>::iterator it = this2edge->adjvex->adjacent_edges[0].begin();
	//	int flag = 0;
	//	while (it != this2edge->adjvex->adjacent_edges[1].end())
	//	{
	//		// find the adjacent edge : edge -> this
	//		if ((*it)->adjvex != this)
	//		{
	//			it++;
	//			if (it == this2edge->adjvex->adjacent_edges[0].end())
	//			{
	//				it = this2edge->adjvex->adjacent_edges[1].begin();
	//				flag = 1;
	//			}
	//			continue;
	//		}
	//		// delete the adjacent edge : edge -> this
	//		RoutingEdge* tmp = *it;
	//		delete tmp;
	//		it = this2edge->adjvex->adjacent_edges[flag].erase(it);
	//		break;
	//	}
	//}
	//for (auto& this2edge : this->adjacent_edges[1])
	//{
	//	vector<RoutingEdge*>::iterator it = this2edge->adjvex->adjacent_edges[0].begin();
	//	int flag = 0;
	//	while (it != this2edge->adjvex->adjacent_edges[1].end())
	//	{
	//		// find the adjacent edge : edge -> this
	//		if ((*it)->adjvex != this)
	//		{
	//			it++;
	//			if (it == this2edge->adjvex->adjacent_edges[0].end())
	//			{
	//				it = this2edge->adjvex->adjacent_edges[1].begin();
	//				flag = 1;
	//			}
	//			continue;
	//		}
	//		// delete the adjacent edge : edge -> this
	//		RoutingEdge* tmp = *it;
	//		delete tmp;
	//		it = this2edge->adjvex->adjacent_edges[flag].erase(it);
	//		break;
	//	}
	//}
	// add adjacent edges : lchild -> edges & edges -> lchild
	/*for (auto& this2edge : this->adjacent_edges)
	{
		RoutingEdge* adjacent_edge = new RoutingEdge(this2edge->adjvex, 1);
		lchild->adjacent_edges.push_back(adjacent_edge);

		adjacent_edge = new RoutingEdge(lchild, 1);
		this2edge->adjvex->adjacent_edges.push_back(adjacent_edge);

		lchild->adjacent_edges.back()->inverse_edge = this2edge->adjvex->adjacent_edges.back();
		this2edge->adjvex->adjacent_edges.back()->inverse_edge = lchild->adjacent_edges.back();
	}*/
	// add adjacent edges : rchild -> edges & edges -> rchild
	/*for (auto& this2edge : this->adjacent_edges)
	{
		RoutingEdge* adjacent_edge = new RoutingEdge(this2edge->adjvex, 1);
		rchild->adjacent_edges.push_back(adjacent_edge);

		adjacent_edge = new RoutingEdge(rchild, 1);
		this2edge->adjvex->adjacent_edges.push_back(adjacent_edge);

		rchild->adjacent_edges.back()->inverse_edge = this2edge->adjvex->adjacent_edges.back();
		this2edge->adjvex->adjacent_edges.back()->inverse_edge = rchild->adjacent_edges.back();
	}*/
	// delete adjacent edges : this -> edges
	/*for (auto& adjacent_edge : adjacent_edges[0])
	{
		delete adjacent_edge;
	}
	this->adjacent_edges[0].clear();
	for (auto& adjacent_edge : adjacent_edges[1])
	{
		delete adjacent_edge;
	}
	this->adjacent_edges[1].clear();*/

	this->dynamic_graph_lchild = lchild;
	this->dynamic_graph_rchild = rchild;
}
