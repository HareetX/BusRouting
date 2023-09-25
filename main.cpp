#pragma once
#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>

#include "HananVertex.h"
#include "HananEdge.h"

#include "AStarRouter.h"

#include "matplotlibcpp.h"

using namespace std;
namespace plot = matplotlibcpp;

bool HorizontalCompLess1(const HorizontalEdge* edge, const ComponentVertex* vertex)
{
	return edge->horizontal_coord < vertex->position.y;
}

bool HorizontalCompLess2(const ComponentVertex* vertex, const HorizontalEdge* edge)
{
	return vertex->position.y < edge->horizontal_coord;
}

bool VerticalCompLess1(const VerticalEdge* edge, const ComponentVertex* vertex)
{
	return edge->vertical_coord < vertex->position.x;
}

bool VerticalCompLess2(const ComponentVertex* vertex, const VerticalEdge* edge)
{
	return vertex->position.x < edge->vertical_coord;
}

vector<HananVertex*> VertexList;
vector<ComponentVertex*> ComponentVertexList;
vector<HorizontalEdge*> HorizontalEdgeList;
vector<VerticalEdge*> VerticalEdgeList;

map<float, vector<HorizontalEdge*>> HorizontalEdgeMap;
map<float, vector<VerticalEdge*>> VerticalEdgeMap;

void InitialBoard(float boundary_point_1[2], float boundary_point_2[2])
{
	// Initialize VertexList
	HananVertex* point1 = new HananVertex(*boundary_point_1, *(boundary_point_1 + 1));
	HananVertex* point2 = new HananVertex(*boundary_point_2, *(boundary_point_1 + 1));
	HananVertex* point3 = new HananVertex(*boundary_point_1, *(boundary_point_2 + 1));
	HananVertex* point4 = new HananVertex(*boundary_point_2, *(boundary_point_2 + 1));
	VertexList.push_back(point1);
	VertexList.push_back(point2);
	VertexList.push_back(point3);
	VertexList.push_back(point4);

	// Initialize EdgeList
	HorizontalEdge* edge1 = new HorizontalEdge(point1, point2, 0, BoardBoundary);
	HorizontalEdge* edge2 = new HorizontalEdge(point3, point4, 0, BoardBoundary);
	HorizontalEdgeList.insert(lower_bound(HorizontalEdgeList.begin(), HorizontalEdgeList.end(), edge1, HorizontalCompLess), edge1);
	HorizontalEdgeList.insert(lower_bound(HorizontalEdgeList.begin(), HorizontalEdgeList.end(), edge2, HorizontalCompLess), edge2);

	VerticalEdge* edge3 = new VerticalEdge(point1, point3, 0, BoardBoundary);
	VerticalEdge* edge4 = new VerticalEdge(point2, point4, 0, BoardBoundary);
	VerticalEdgeList.insert(lower_bound(VerticalEdgeList.begin(), VerticalEdgeList.end(), edge3, VerticalCompLess), edge3);
	VerticalEdgeList.insert(lower_bound(VerticalEdgeList.begin(), VerticalEdgeList.end(), edge4, VerticalCompLess), edge4);
}

void InitialComponent(float boundary_point_1[2], float boundary_point_2[2])
{
	// Initialize VertexList
	HananVertex* point1 = new ComponentVertex(*boundary_point_1, *(boundary_point_1 + 1), upleft);
	HananVertex* point2 = new ComponentVertex(*boundary_point_2, *(boundary_point_1 + 1), upright);
	HananVertex* point3 = new ComponentVertex(*boundary_point_1, *(boundary_point_2 + 1), downleft);
	HananVertex* point4 = new ComponentVertex(*boundary_point_2, *(boundary_point_2 + 1), downright);
	VertexList.push_back(point1);
	VertexList.push_back(point2);
	VertexList.push_back(point3);
	VertexList.push_back(point4);

	// Initialize EdgeList
	HorizontalEdge* edge1 = new HorizontalEdge(point1, point2, 0, ComponentBoundary);
	HorizontalEdge* edge2 = new HorizontalEdge(point3, point4, 0, ComponentBoundary);
	HorizontalEdgeList.insert(lower_bound(HorizontalEdgeList.begin(), HorizontalEdgeList.end(), edge1, HorizontalCompLess), edge1);
	HorizontalEdgeList.insert(lower_bound(HorizontalEdgeList.begin(), HorizontalEdgeList.end(), edge2, HorizontalCompLess), edge2);

	VerticalEdge* edge3 = new VerticalEdge(point1, point3, 0, ComponentBoundary);
	VerticalEdge* edge4 = new VerticalEdge(point2, point4, 0, ComponentBoundary);
	VerticalEdgeList.insert(lower_bound(VerticalEdgeList.begin(), VerticalEdgeList.end(), edge3, VerticalCompLess), edge3);
	VerticalEdgeList.insert(lower_bound(VerticalEdgeList.begin(), VerticalEdgeList.end(), edge4, VerticalCompLess), edge4);

	// Initialize ComponentVertexList
	ComponentVertexList.push_back((ComponentVertex*)point1);
	ComponentVertexList.push_back((ComponentVertex*)point2);
	ComponentVertexList.push_back((ComponentVertex*)point3);
	ComponentVertexList.push_back((ComponentVertex*)point4);
}

void GenerateHananGrid()
{
	for (auto& component_vertex : ComponentVertexList)
	{
		// Extend up
		if (!component_vertex->is_extended[0] and (component_vertex->vertex_type == upleft or component_vertex->vertex_type == upright))
		{
			component_vertex->is_extended[0] = true;
			// Find the closest edge
			vector<HorizontalEdge*>::iterator it = lower_bound(HorizontalEdgeList.begin(), HorizontalEdgeList.end(), component_vertex, HorizontalCompLess1);
			// Inner Vertexs Buffer
			vector<HananVertex*> vertex_buffer;
			while (it != HorizontalEdgeList.begin())
			{
				it = it - 1;
				if ((*it)->edge_type == BoardBoundary)
				{
					HananVertex* vertex = new HananVertex(component_vertex->position.x, (*it)->horizontal_coord);
					// Add the new inner vertex to the edge
					(*it)->add_inner_vertex(vertex);

					// Create a new edge
					VerticalEdge* edge = new VerticalEdge(vertex, component_vertex, 0, EdgeLine);

					// Pop out the vertex from the buffer & Add them to the inner_vertexs
					for (auto inner_vertex : vertex_buffer)
					{
						edge->add_inner_vertex(inner_vertex);
					}

					// Add new vertex and new edge to VertexList and EdgeList
					VertexList.push_back(vertex);
					VerticalEdgeList.insert(lower_bound(VerticalEdgeList.begin(), VerticalEdgeList.end(), edge, VerticalCompLess), edge);

					break;
				}
				else if ((*it)->is_inner_vertex(component_vertex))
				{
					HananVertex* vertex = new HananVertex(component_vertex->position.x, (*it)->horizontal_coord);
					// Add the new inner vertex to the edge
					(*it)->add_inner_vertex(vertex);

					if ((*it)->edge_type == ComponentBoundary)
					{
						// Create a new edge
						VerticalEdge* edge = new VerticalEdge(vertex, component_vertex, 0, EdgeLine);

						// Pop out the vertex from the buffer & Add them to the inner_vertexs
						for (auto inner_vertex : vertex_buffer)
						{
							edge->add_inner_vertex(inner_vertex);
						}

						// Add new vertex and new edge to VertexList and EdgeList
						VertexList.push_back(vertex);
						VerticalEdgeList.insert(lower_bound(VerticalEdgeList.begin(), VerticalEdgeList.end(), edge, VerticalCompLess), edge);

						break;
					}
					else if ((*it)->edge_type == EdgeLine)
					{
						// Add the new inner vertex to buffer
						vertex_buffer.push_back(vertex);

						// Add new vertex to VertexList
						VertexList.push_back(vertex);
					}
				}
				else if ((*it)->vertexs[0]->position.x == component_vertex->position.x)
				{
					((ComponentVertex*)((*it)->vertexs[0]))->is_extended[0] = true;
					// Create a new edge
					VerticalEdge* edge = new VerticalEdge((*it)->vertexs[0], component_vertex, 0, EdgeLine);

					// Pop out the vertex from the buffer & Add them to the inner_vertexs
					for (auto inner_vertex : vertex_buffer)
					{
						edge->add_inner_vertex(inner_vertex);
					}

					// Add new vertex and new edge to VertexList and EdgeList
					VerticalEdgeList.insert(lower_bound(VerticalEdgeList.begin(), VerticalEdgeList.end(), edge, VerticalCompLess), edge);

					break;
				}
				else if ((*it)->vertexs[1]->position.x == component_vertex->position.x)
				{
					((ComponentVertex*)((*it)->vertexs[1]))->is_extended[0] = true;
					// Create a new edge
					VerticalEdge* edge = new VerticalEdge((*it)->vertexs[1], component_vertex, 0, EdgeLine);

					// Pop out the vertex from the buffer & Add them to the inner_vertexs
					for (auto inner_vertex : vertex_buffer)
					{
						edge->add_inner_vertex(inner_vertex);
					}

					// Add new vertex and new edge to VertexList and EdgeList
					VerticalEdgeList.insert(lower_bound(VerticalEdgeList.begin(), VerticalEdgeList.end(), edge, VerticalCompLess), edge);

					break;
				}
			}
		}
		// Extend left
		if (!component_vertex->is_extended[1] and (component_vertex->vertex_type == upleft or component_vertex->vertex_type == downleft))
		{
			component_vertex->is_extended[1] = true;
			// Find the closest edge
			vector<VerticalEdge*>::iterator it = lower_bound(VerticalEdgeList.begin(), VerticalEdgeList.end(), component_vertex, VerticalCompLess1);
			// Inner Vertexs Buffer
			vector<HananVertex*> vertex_buffer;
			while (it != VerticalEdgeList.begin())
			{
				it = it - 1;
				if ((*it)->edge_type == BoardBoundary)
				{
					HananVertex* vertex = new HananVertex((*it)->vertical_coord, component_vertex->position.y);
					// Add the new inner vertex to the edge
					(*it)->add_inner_vertex(vertex);

					// Create a new edge
					HorizontalEdge* edge = new HorizontalEdge(vertex, component_vertex, 0, EdgeLine);

					// Pop out the vertex from the buffer & Add them to the inner_vertexs
					for (auto inner_vertex : vertex_buffer)
					{
						edge->add_inner_vertex(inner_vertex);
					}

					// Add new vertex and new edge to VertexList and EdgeList
					VertexList.push_back(vertex);
					HorizontalEdgeList.insert(lower_bound(HorizontalEdgeList.begin(), HorizontalEdgeList.end(), edge, HorizontalCompLess), edge);

					break;
				}
				else if ((*it)->is_inner_vertex(component_vertex))
				{
					HananVertex* vertex = new HananVertex((*it)->vertical_coord, component_vertex->position.y);
					// Add the new inner vertex to the edge
					(*it)->add_inner_vertex(vertex);

					if ((*it)->edge_type == ComponentBoundary)
					{
						// Create a new edge
						HorizontalEdge* edge = new HorizontalEdge(vertex, component_vertex, 0, EdgeLine);

						// Pop out the vertex from the buffer & Add them to the inner_vertexs
						for (auto inner_vertex : vertex_buffer)
						{
							edge->add_inner_vertex(inner_vertex);
						}

						// Add new vertex and new edge to VertexList and EdgeList
						VertexList.push_back(vertex);
						HorizontalEdgeList.insert(lower_bound(HorizontalEdgeList.begin(), HorizontalEdgeList.end(), edge, HorizontalCompLess), edge);

						break;
					}
					else if ((*it)->edge_type == EdgeLine)
					{
						// Add the new inner vertex to buffer
						vertex_buffer.push_back(vertex);

						// Add new vertex to VertexList
						VertexList.push_back(vertex);
					}
				}
				else if ((*it)->vertexs[0]->position.y == component_vertex->position.y)
				{
					((ComponentVertex*)((*it)->vertexs[0]))->is_extended[1] = true;
					// Create a new edge
					HorizontalEdge* edge = new HorizontalEdge((*it)->vertexs[0], component_vertex, 0, EdgeLine);

					// Pop out the vertex from the buffer & Add them to the inner_vertexs
					for (auto inner_vertex : vertex_buffer)
					{
						edge->add_inner_vertex(inner_vertex);
					}

					// Add new vertex and new edge to VertexList and EdgeList
					HorizontalEdgeList.insert(lower_bound(HorizontalEdgeList.begin(), HorizontalEdgeList.end(), edge, HorizontalCompLess), edge);

					break;
				}
				else if ((*it)->vertexs[1]->position.y == component_vertex->position.y)
				{
					((ComponentVertex*)((*it)->vertexs[1]))->is_extended[1] = true;
					// Create a new edge
					HorizontalEdge* edge = new HorizontalEdge((*it)->vertexs[1], component_vertex, 0, EdgeLine);

					// Pop out the vertex from the buffer & Add them to the inner_vertexs
					for (auto inner_vertex : vertex_buffer)
					{
						edge->add_inner_vertex(inner_vertex);
					}

					// Add new vertex and new edge to VertexList and EdgeList
					HorizontalEdgeList.insert(lower_bound(HorizontalEdgeList.begin(), HorizontalEdgeList.end(), edge, HorizontalCompLess), edge);

					break;
				}
			}
		}
		// Extend down
		if (!component_vertex->is_extended[0] and (component_vertex->vertex_type == downleft or component_vertex->vertex_type == downright))
		{
			component_vertex->is_extended[0] = true;
			// Find the closest edge
			vector<HorizontalEdge*>::iterator it = upper_bound(HorizontalEdgeList.begin(), HorizontalEdgeList.end(), component_vertex, HorizontalCompLess2);
			// Inner Vertexs Buffer
			vector<HananVertex*> vertex_buffer;
			while (it != HorizontalEdgeList.end())
			{
				if ((*it)->edge_type == BoardBoundary)
				{
					HananVertex* vertex = new HananVertex(component_vertex->position.x, (*it)->horizontal_coord);
					// Add the new inner vertex to the edge
					(*it)->add_inner_vertex(vertex);

					// Create a new edge
					VerticalEdge* edge = new VerticalEdge(component_vertex, vertex, 0, EdgeLine);

					// Pop out the vertex from the buffer & Add them to the inner_vertexs
					for (auto inner_vertex : vertex_buffer)
					{
						edge->add_inner_vertex(inner_vertex);
					}

					// Add new vertex and new edge to VertexList and EdgeList
					VertexList.push_back(vertex);
					VerticalEdgeList.insert(lower_bound(VerticalEdgeList.begin(), VerticalEdgeList.end(), edge, VerticalCompLess), edge);

					break;
				}
				else if ((*it)->is_inner_vertex(component_vertex))
				{
					HananVertex* vertex = new HananVertex(component_vertex->position.x, (*it)->horizontal_coord);
					// Add the new inner vertex to the edge
					(*it)->add_inner_vertex(vertex);

					if ((*it)->edge_type == ComponentBoundary)
					{
						// Create a new edge
						VerticalEdge* edge = new VerticalEdge(component_vertex, vertex, 0, EdgeLine);

						// Pop out the vertex from the buffer & Add them to the inner_vertexs
						for (auto inner_vertex : vertex_buffer)
						{
							edge->add_inner_vertex(inner_vertex);
						}

						// Add new vertex and new edge to VertexList and EdgeList
						VertexList.push_back(vertex);
						VerticalEdgeList.insert(lower_bound(VerticalEdgeList.begin(), VerticalEdgeList.end(), edge, VerticalCompLess), edge);

						break;
					}
					else if ((*it)->edge_type == EdgeLine)
					{
						// Add the new inner vertex to buffer
						vertex_buffer.push_back(vertex);

						// Add new vertex to VertexList
						VertexList.push_back(vertex);
					}
				}
				else if ((*it)->vertexs[0]->position.x == component_vertex->position.x)
				{
					((ComponentVertex*)((*it)->vertexs[0]))->is_extended[0] = true;
					// Create a new edge
					VerticalEdge* edge = new VerticalEdge(component_vertex, (*it)->vertexs[0], 0, EdgeLine);

					// Pop out the vertex from the buffer & Add them to the inner_vertexs
					for (auto inner_vertex : vertex_buffer)
					{
						edge->add_inner_vertex(inner_vertex);
					}

					// Add new vertex and new edge to VertexList and EdgeList
					VerticalEdgeList.insert(lower_bound(VerticalEdgeList.begin(), VerticalEdgeList.end(), edge, VerticalCompLess), edge);

					break;
				}
				else if ((*it)->vertexs[1]->position.x == component_vertex->position.x)
				{
					((ComponentVertex*)((*it)->vertexs[1]))->is_extended[0] = true;
					// Create a new edge
					VerticalEdge* edge = new VerticalEdge(component_vertex, (*it)->vertexs[1], 0, EdgeLine);

					// Pop out the vertex from the buffer & Add them to the inner_vertexs
					for (auto inner_vertex : vertex_buffer)
					{
						edge->add_inner_vertex(inner_vertex);
					}

					// Add new vertex and new edge to VertexList and EdgeList
					VerticalEdgeList.insert(lower_bound(VerticalEdgeList.begin(), VerticalEdgeList.end(), edge, VerticalCompLess), edge);

					break;
				}
				it = it + 1;
			}
		}
		// Extend right
		if (!component_vertex->is_extended[1] and (component_vertex->vertex_type == upright or component_vertex->vertex_type == downright))
		{
			component_vertex->is_extended[1] = true;
			// Find the closest edge
			vector<VerticalEdge*>::iterator it = upper_bound(VerticalEdgeList.begin(), VerticalEdgeList.end(), component_vertex, VerticalCompLess2);
			// Inner Vertexs Buffer
			vector<HananVertex*> vertex_buffer;
			while (it != VerticalEdgeList.end())
			{
				if ((*it)->edge_type == BoardBoundary)
				{
					HananVertex* vertex = new HananVertex((*it)->vertical_coord, component_vertex->position.y);
					// Add the new inner vertex to the edge
					(*it)->add_inner_vertex(vertex);

					// Create a new edge
					HorizontalEdge* edge = new HorizontalEdge(component_vertex, vertex, 0, EdgeLine);

					// Pop out the vertex from the buffer & Add them to the inner_vertexs
					for (auto inner_vertex : vertex_buffer)
					{
						edge->add_inner_vertex(inner_vertex);
					}

					// Add new vertex and new edge to VertexList and EdgeList
					VertexList.push_back(vertex);
					HorizontalEdgeList.insert(lower_bound(HorizontalEdgeList.begin(), HorizontalEdgeList.end(), edge, HorizontalCompLess), edge);

					break;
				}
				else if ((*it)->is_inner_vertex(component_vertex))
				{
					HananVertex* vertex = new HananVertex((*it)->vertical_coord, component_vertex->position.y);
					// Add the new inner vertex to the edge
					(*it)->add_inner_vertex(vertex);

					if ((*it)->edge_type == ComponentBoundary)
					{
						// Create a new edge
						HorizontalEdge* edge = new HorizontalEdge(component_vertex, vertex, 0, EdgeLine);

						// Pop out the vertex from the buffer & Add them to the inner_vertexs
						for (auto inner_vertex : vertex_buffer)
						{
							edge->add_inner_vertex(inner_vertex);
						}

						// Add new vertex and new edge to VertexList and EdgeList
						VertexList.push_back(vertex);
						HorizontalEdgeList.insert(lower_bound(HorizontalEdgeList.begin(), HorizontalEdgeList.end(), edge, HorizontalCompLess), edge);

						break;
					}
					else if ((*it)->edge_type == EdgeLine)
					{
						// Add the new inner vertex to buffer
						vertex_buffer.push_back(vertex);

						// Add new vertex to VertexList
						VertexList.push_back(vertex);
					}
				}
				else if ((*it)->vertexs[0]->position.y == component_vertex->position.y)
				{
					((ComponentVertex*)((*it)->vertexs[0]))->is_extended[1] = true;
					// Create a new edge
					HorizontalEdge* edge = new HorizontalEdge(component_vertex, (*it)->vertexs[0], 0, EdgeLine);

					// Pop out the vertex from the buffer & Add them to the inner_vertexs
					for (auto inner_vertex : vertex_buffer)
					{
						edge->add_inner_vertex(inner_vertex);
					}

					// Add new vertex and new edge to VertexList and EdgeList
					HorizontalEdgeList.insert(lower_bound(HorizontalEdgeList.begin(), HorizontalEdgeList.end(), edge, HorizontalCompLess), edge);

					break;
				}
				else if ((*it)->vertexs[1]->position.y == component_vertex->position.y)
				{
					((ComponentVertex*)((*it)->vertexs[1]))->is_extended[1] = true;
					// Create a new edge
					HorizontalEdge* edge = new HorizontalEdge(component_vertex, (*it)->vertexs[1], 0, EdgeLine);

					// Pop out the vertex from the buffer & Add them to the inner_vertexs
					for (auto inner_vertex : vertex_buffer)
					{
						edge->add_inner_vertex(inner_vertex);
					}

					// Add new vertex and new edge to VertexList and EdgeList
					HorizontalEdgeList.insert(lower_bound(HorizontalEdgeList.begin(), HorizontalEdgeList.end(), edge, HorizontalCompLess), edge);

					break;
				}
				it = it + 1;
			}
		}
	}

	// disassemble the edges
	for (auto edge : HorizontalEdgeList)
	{
		edge->disassemble_edge(HorizontalEdgeMap);
	}
	for (auto edge : VerticalEdgeList)
	{
		edge->disassemble_edge(VerticalEdgeMap);
	}
}

void InitialRoutingGraph()
{
	// initial adjacent edges of the edges
	for (auto& edges : HorizontalEdgeMap)
	{
		for (auto edge : edges.second)
		{
			edge->initial_adjacent_edges();
		}
	}
	for (auto& edges : VerticalEdgeMap)
	{
		for (auto edge : edges.second)
		{
			edge->initial_adjacent_edges();
		}
	}
}

void UpdateRoutingGraph(vector<HananEdge*>& path)
{
	// Generate new routing graph that does not take weight into account
	for (auto& edge : path)
	{
		edge->split2child();
	}
}

void PrintHananGrid()
{
	// Print HorizontalEdge
	cout << "----------- Horizontal Edge List -----------" << endl;
	int i = 0;
	vector<float> x(2);
	vector<float> y(2);
	for (auto edge : HorizontalEdgeList)
	{
		i++;
		cout << "horizontal edge " << i << endl;

		cout << "vertex1 : "; edge->vertexs[0]->PrintVertex(); cout << "  ";
		x[0] = edge->vertexs[0]->position.x;
		y[0] = edge->vertexs[0]->position.y;
		cout << "vertex2 : "; edge->vertexs[1]->PrintVertex(); cout << endl;
		x[1] = edge->vertexs[1]->position.x;
		y[1] = edge->vertexs[1]->position.y;

		cout << "horizontal coord = " << edge->horizontal_coord << endl;
		if (edge->edge_type == BoardBoundary)
		{
			cout << "edge type = BoardBoundary" << endl;
			plot::plot(x, y, "r");
		}
		else if (edge->edge_type == ComponentBoundary)
		{
			cout << "edge type = ComponentBoundary" << endl;
			plot::plot(x, y, "r");
		}
		else if (edge->edge_type == EdgeLine)
		{
			cout << "edge type = EdgeLine" << endl;
			plot::plot(x, y, "b");
		}
		cout << "inner vertexs : ";
		for (auto vertex : edge->inner_vertexs)
		{
			vertex->PrintVertex(); cout << " ; ";
		}
		cout << endl << endl;
	}

	// Print VerticalEdge
	cout << "----------- Vertical Edge List -----------" << endl;
	i = 0;
	for (auto edge : VerticalEdgeList)
	{
		i++;
		cout << "vertical edge " << i << endl;

		cout << "vertex1 : "; edge->vertexs[0]->PrintVertex(); cout << "  ";
		x[0] = edge->vertexs[0]->position.x;
		y[0] = edge->vertexs[0]->position.y;
		cout << "vertex2 : "; edge->vertexs[1]->PrintVertex(); cout << endl;
		x[1] = edge->vertexs[1]->position.x;
		y[1] = edge->vertexs[1]->position.y;

		cout << "vertical coord = " << edge->vertical_coord << endl;
		if (edge->edge_type == BoardBoundary)
		{
			cout << "edge type = BoardBoundary" << endl;
			plot::plot(x, y, "r");
		}
		else if (edge->edge_type == ComponentBoundary)
		{
			cout << "edge type = ComponentBoundary" << endl;
			plot::plot(x, y, "r");
		}
		else if (edge->edge_type == EdgeLine)
		{
			cout << "edge type = EdgeLine" << endl;
			plot::plot(x, y, "b");
		}
		cout << "inner vertexs : ";
		for (auto vertex : edge->inner_vertexs)
		{
			vertex->PrintVertex(); cout << " ; ";
		}
		cout << endl << endl;
	}

	plot::show();
}

void PrintRoutingGraph()
{
	vector<float> x(2);
	vector<float> y(2);
	// draw Horizontal Edges
	for (auto& edges : HorizontalEdgeMap)
	{
		for (auto& edge : edges.second)
		{
			// draw the Hanan Grid edge
			x = { edge->vertexs[0]->position.x, edge->vertexs[1]->position.x };
			y = { edge->vertexs[0]->position.y, edge->vertexs[1]->position.y };
			if (edge->edge_type == EdgeLine)
			{
				plot::plot(x, y, "b");
			}
			else
			{
				plot::plot(x, y, "r");
			}

			/*
			* draw the Routing Graph edge
			* use pre-order traversal to find the leaf node
			*/
			if (edge == NULL) return;
			stack<HananEdge*> s;
			s.push(edge);
			while (!s.empty()) {
				HananEdge* nd = s.top();

				// visit(nd)
				if (nd->is_leaf_node())
				{
					x[0] = (nd->vertexs[0]->position.x + nd->vertexs[1]->position.x) / 2.0;
					y[0] = (nd->vertexs[0]->position.y + nd->vertexs[1]->position.y) / 2.0;
					for (auto& routing_edge : nd->adjacent_edges)
					{
						x[1] = (routing_edge->adjvex->vertexs[0]->position.x + routing_edge->adjvex->vertexs[1]->position.x) / 2.0;
						y[1] = (routing_edge->adjvex->vertexs[0]->position.y + routing_edge->adjvex->vertexs[1]->position.y) / 2.0;
						plot::plot(x, y, "g");
					}
				}

				s.pop();
				if (nd->dynamic_graph_rchild != NULL)
					s.push(nd->dynamic_graph_rchild);
				if (nd->dynamic_graph_lchild != NULL)
					s.push(nd->dynamic_graph_lchild);
			}
		}
	}
	// draw Vertical Edges
	for (auto& edges : VerticalEdgeMap)
	{
		for (auto& edge : edges.second)
		{
			// draw the Hanan Grid edge
			x = { edge->vertexs[0]->position.x, edge->vertexs[1]->position.x };
			y = { edge->vertexs[0]->position.y, edge->vertexs[1]->position.y };
			if (edge->edge_type == EdgeLine)
			{
				plot::plot(x, y, "b");
			}
			else
			{
				plot::plot(x, y, "r");
			}

			// draw the Routing Graph edge
			x[0] = (edge->vertexs[0]->position.x + edge->vertexs[1]->position.x) / 2.0;
			y[0] = (edge->vertexs[0]->position.y + edge->vertexs[1]->position.y) / 2.0;
			for (auto& routing_edge : edge->adjacent_edges)
			{
				x[1] = (routing_edge->adjvex->vertexs[0]->position.x + routing_edge->adjvex->vertexs[1]->position.x) / 2.0;
				y[1] = (routing_edge->adjvex->vertexs[0]->position.y + routing_edge->adjvex->vertexs[1]->position.y) / 2.0;
				plot::plot(x, y, "g");
			}
		}
	}
	plot::show();
}

void printRoutingPath(vector<HananEdge*>& path)
{
	vector<float> x(2);
	vector<float> y(2);
	// draw Horizontal Edges
	for (auto& edges : HorizontalEdgeMap)
	{
		for (auto& edge : edges.second)
		{
			// draw the Hanan Grid edge
			x = { edge->vertexs[0]->position.x, edge->vertexs[1]->position.x };
			y = { edge->vertexs[0]->position.y, edge->vertexs[1]->position.y };
			if (edge->edge_type == EdgeLine)
			{
				plot::plot(x, y, "b");
			}
			else
			{
				plot::plot(x, y, "r");
			}

			/*
			* draw the Routing Graph edge
			* use pre-order traversal to find the leaf node
			*/
			if (edge == NULL) return;
			stack<HananEdge*> s;
			s.push(edge);
			while (!s.empty()) {
				HananEdge* nd = s.top();
				
				// visit(nd)
				if (nd->is_leaf_node())
				{
					x[0] = (nd->vertexs[0]->position.x + nd->vertexs[1]->position.x) / 2.0;
					y[0] = (nd->vertexs[0]->position.y + nd->vertexs[1]->position.y) / 2.0;
					for (auto& routing_edge : nd->adjacent_edges)
					{
						x[1] = (routing_edge->adjvex->vertexs[0]->position.x + routing_edge->adjvex->vertexs[1]->position.x) / 2.0;
						y[1] = (routing_edge->adjvex->vertexs[0]->position.y + routing_edge->adjvex->vertexs[1]->position.y) / 2.0;
						plot::plot(x, y, "g");
					}
				}

				s.pop();
				if (nd->dynamic_graph_rchild != NULL)
					s.push(nd->dynamic_graph_rchild);
				if (nd->dynamic_graph_lchild != NULL)
					s.push(nd->dynamic_graph_lchild);
			}
		}
	}
	// draw Vertical Edges
	for (auto& edges : VerticalEdgeMap)
	{
		for (auto& edge : edges.second)
		{
			// draw the Hanan Grid edge
			x = { edge->vertexs[0]->position.x, edge->vertexs[1]->position.x };
			y = { edge->vertexs[0]->position.y, edge->vertexs[1]->position.y };
			if (edge->edge_type == EdgeLine)
			{
				plot::plot(x, y, "b");
			}
			else
			{
				plot::plot(x, y, "r");
			}

			// draw the Routing Graph edge
			x[0] = (edge->vertexs[0]->position.x + edge->vertexs[1]->position.x) / 2.0;
			y[0] = (edge->vertexs[0]->position.y + edge->vertexs[1]->position.y) / 2.0;
			for (auto& routing_edge : edge->adjacent_edges)
			{
				x[1] = (routing_edge->adjvex->vertexs[0]->position.x + routing_edge->adjvex->vertexs[1]->position.x) / 2.0;
				y[1] = (routing_edge->adjvex->vertexs[0]->position.y + routing_edge->adjvex->vertexs[1]->position.y) / 2.0;
				plot::plot(x, y, "g");
			}
		}
	}
	
	// draw the Routing path
	for (auto path_node : path)
	{
		if (path_node == path[0])
		{
			x[0] = (path_node->vertexs[0]->position.x + path_node->vertexs[1]->position.x) / 2.0;
			y[0] = (path_node->vertexs[0]->position.y + path_node->vertexs[1]->position.y) / 2.0;
			continue;
		}
		x[1] = (path_node->vertexs[0]->position.x + path_node->vertexs[1]->position.x) / 2.0;
		y[1] = (path_node->vertexs[0]->position.y + path_node->vertexs[1]->position.y) / 2.0;
		plot::plot(x, y, "k");
		x[0] = x[1];
		y[0] = y[1];
	}
	plot::show();
}

int main()
{
	float boundary_point_1[2] = { 0, 0 };
	float boundary_point_2[2] = { 36, 27 };

	//float component_boundary_1[2][2][2] = { {{7, 5}, {12, 8}}, {{17, 7}, {21, 12}} };
	//float component_boundary_1[2][2][2] = { {{7, 5}, {12, 8}}, {{7, 12}, {21, 17}} };
	//float component_boundary_1[2][2][2] = { {{7, 5}, {12, 8}}, {{15, 5}, {21, 17}} };
	float component_boundary_1[4][2][2] = { {{7, 5}, {12, 8}}, {{17, 7}, {21, 12}}, {{7, 15}, {21, 20}}, {{25, 7}, {32, 16}} };

	// Initial Board
	InitialBoard(boundary_point_1, boundary_point_2);

	// Initial Component
	InitialComponent(component_boundary_1[0][0], component_boundary_1[0][1]);
	InitialComponent(component_boundary_1[1][0], component_boundary_1[1][1]);
	InitialComponent(component_boundary_1[2][0], component_boundary_1[2][1]);
	InitialComponent(component_boundary_1[3][0], component_boundary_1[3][1]);

	// Print the Initial Grid Message
	PrintHananGrid();

	// Generate Hanan Grid
	GenerateHananGrid();

	// Print the Hanan Grid Message
	PrintHananGrid();

	// Initial Routing Graph
	InitialRoutingGraph();

	// Print the Routing Graph
	PrintRoutingGraph();

	AStarRouter router;
	vector<HananEdge*> path;
	router.a_star_router(VerticalEdgeMap.at(7)[5], HorizontalEdgeMap.at(16)[2], path);

	UpdateRoutingGraph(path);

	//PrintRoutingGraph();
	printRoutingPath(path);

	return 0;
}