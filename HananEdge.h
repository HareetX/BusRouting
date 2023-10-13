#pragma once
#include "ultis.h"
using namespace std;

class HananEdge;

class RoutingEdge
{
public:
	RoutingEdge(HananEdge* adjacent, float w);
	~RoutingEdge() {};

	HananEdge* adjvex;

	float weight;
	vector<int> conflict_id;

	RoutingEdge* inverse_edge;

private:

};

enum EdgeType
{
	BoardBoundary,
	ComponentBoundary,
	EdgeLine
};

class HananEdge : public Edge
{
public:
	HananEdge(Vertex* vertex_1, Vertex* vertex_2, float max_capacity, EdgeType type) : Edge(vertex_1, vertex_2)
	{
		capacity = max_capacity; edge_type = type; dynamic_graph_parent = NULL; dynamic_graph_lchild = NULL; dynamic_graph_rchild = NULL;
	};
	HananEdge(Vertex* vertex_1, Vertex* vertex_2, float max_capacity, EdgeType type, HananEdge* parent) : Edge(vertex_1, vertex_2)
	{
		capacity = max_capacity; edge_type = type; dynamic_graph_parent = parent; dynamic_graph_lchild = NULL; dynamic_graph_rchild = NULL;
	};
	~HananEdge() {};

	float capacity;

	vector<RoutingEdge*> adjacent_edges[2];

	EdgeType edge_type;

	vector<Vertex*> inner_center_vertexs;
	vector<Position> inner_bus_terminals;

	HananEdge* dynamic_graph_parent;
	HananEdge* dynamic_graph_lchild;
	HananEdge* dynamic_graph_rchild;

	Position center_position();

	bool is_extended_adjvex(HananEdge* edge);

	bool is_leaf_node();
	bool is_root_node();

	void leaf_node_list(vector<HananEdge*>& edge_list);
	HananEdge* root_node_edge();

	void split2child(float width);

private:

};


class HorizontalEdge : public HananEdge
{
public:
	HorizontalEdge(Vertex* vertex_1, Vertex* vertex_2, int max_capacity, EdgeType type) : HananEdge(vertex_1, vertex_2, max_capacity, type) { horizontal_coord = vertex_1->position.y; };
	~HorizontalEdge() {};

	float horizontal_coord;
	vector<Vertex*> inner_vertexs;

	bool is_inner_vertex(Position vertex_pos);

	void add_inner_vertex(Vertex* vertex);

	void disassemble_edge(map<float, vector<HorizontalEdge*>>& horizontal_edges_map);

	//void initial_adjacent_edges();

	friend bool HorizontalCompLess(const HorizontalEdge* a, const HorizontalEdge* b);
	friend bool HorizontalVertexCompLess(const Vertex* a, const Vertex* b);

private:
	
};

bool HorizontalCompLess(const HorizontalEdge* a, const HorizontalEdge* b);
bool HorizontalVertexCompLess(const Vertex* a, const Vertex* b);


class VerticalEdge : public HananEdge
{
public:
	VerticalEdge(Vertex* vertex_1, Vertex* vertex_2, int max_capacity, EdgeType type) : HananEdge(vertex_1, vertex_2, max_capacity, type) { vertical_coord = vertex_1->position.x; };
	~VerticalEdge() {};

	float vertical_coord;
	vector<Vertex*> inner_vertexs;

	bool is_inner_vertex(Position vertex_pos);

	void add_inner_vertex(Vertex* vertex);

	void disassemble_edge(map<float, vector<VerticalEdge*>>& vertical_edges_map);

	//void initial_adjacent_edges();

	friend bool VerticalCompLess(const VerticalEdge* a, const VerticalEdge* b);
	friend bool VerticalVertexCompLess(const Vertex* a, const Vertex* b);

private:
	
};

bool VerticalCompLess(const VerticalEdge* a, const VerticalEdge* b);
bool VerticalVertexCompLess(const Vertex* a, const Vertex* b);

