#include "HananRoutingGraph.h"

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

HananRoutingGraph::HananRoutingGraph(float board[2][2], vector<float(*)[2]>& component_list)
{
	// Initial Board
	this->initialBoard(board[0], board[1]);

	// Initial Component
	this->initialComponent(component_list[0][0], component_list[0][1]);
	this->initialComponent(component_list[1][0], component_list[1][1]);
	this->initialComponent(component_list[2][0], component_list[2][1]);
	this->initialComponent(component_list[3][0], component_list[3][1]);

	// Print the Initial Grid Message
	this->printHananGrid();

	// Generate Hanan Grid
	this->generateHananGrid();

	// Print the Hanan Grid Message
	this->printHananGrid();

	// Initial Routing Graph
	this->generateHananRoutingGraph();

	// Print the Routing Graph
	this->printRoutingGraph();
}

void HananRoutingGraph::initialBoard(float boundary_point_1[2], float boundary_point_2[2])
{
	// Initialize VertexList
	HananVertex* point1 = new HananVertex(*boundary_point_1, *(boundary_point_1 + 1));
	HananVertex* point2 = new HananVertex(*boundary_point_2, *(boundary_point_1 + 1));
	HananVertex* point3 = new HananVertex(*boundary_point_1, *(boundary_point_2 + 1));
	HananVertex* point4 = new HananVertex(*boundary_point_2, *(boundary_point_2 + 1));
	this->VertexList.push_back(point1);
	this->VertexList.push_back(point2);
	this->VertexList.push_back(point3);
	this->VertexList.push_back(point4);

	// Initialize EdgeList
	HorizontalEdge* edge1 = new HorizontalEdge(point1, point2, 0, BoardBoundary);
	HorizontalEdge* edge2 = new HorizontalEdge(point3, point4, 0, BoardBoundary);
	this->HorizontalEdgeList.insert(lower_bound(this->HorizontalEdgeList.begin(), this->HorizontalEdgeList.end(), edge1, HorizontalCompLess), edge1);
	this->HorizontalEdgeList.insert(lower_bound(this->HorizontalEdgeList.begin(), this->HorizontalEdgeList.end(), edge2, HorizontalCompLess), edge2);

	VerticalEdge* edge3 = new VerticalEdge(point1, point3, 0, BoardBoundary);
	VerticalEdge* edge4 = new VerticalEdge(point2, point4, 0, BoardBoundary);
	this->VerticalEdgeList.insert(lower_bound(this->VerticalEdgeList.begin(), this->VerticalEdgeList.end(), edge3, VerticalCompLess), edge3);
	this->VerticalEdgeList.insert(lower_bound(this->VerticalEdgeList.begin(), this->VerticalEdgeList.end(), edge4, VerticalCompLess), edge4);
}

void HananRoutingGraph::initialComponent(float boundary_point_1[2], float boundary_point_2[2])
{
	// Initialize VertexList
	HananVertex* point1 = new ComponentVertex(*boundary_point_1, *(boundary_point_1 + 1), upleft);
	HananVertex* point2 = new ComponentVertex(*boundary_point_2, *(boundary_point_1 + 1), upright);
	HananVertex* point3 = new ComponentVertex(*boundary_point_1, *(boundary_point_2 + 1), downleft);
	HananVertex* point4 = new ComponentVertex(*boundary_point_2, *(boundary_point_2 + 1), downright);
	this->VertexList.push_back(point1);
	this->VertexList.push_back(point2);
	this->VertexList.push_back(point3);
	this->VertexList.push_back(point4);

	// Initialize EdgeList
	HorizontalEdge* edge1 = new HorizontalEdge(point1, point2, 0, ComponentBoundary);
	HorizontalEdge* edge2 = new HorizontalEdge(point3, point4, 0, ComponentBoundary);
	this->HorizontalEdgeList.insert(lower_bound(this->HorizontalEdgeList.begin(), this->HorizontalEdgeList.end(), edge1, HorizontalCompLess), edge1);
	this->HorizontalEdgeList.insert(lower_bound(this->HorizontalEdgeList.begin(), this->HorizontalEdgeList.end(), edge2, HorizontalCompLess), edge2);

	VerticalEdge* edge3 = new VerticalEdge(point1, point3, 0, ComponentBoundary);
	VerticalEdge* edge4 = new VerticalEdge(point2, point4, 0, ComponentBoundary);
	this->VerticalEdgeList.insert(lower_bound(this->VerticalEdgeList.begin(), this->VerticalEdgeList.end(), edge3, VerticalCompLess), edge3);
	this->VerticalEdgeList.insert(lower_bound(this->VerticalEdgeList.begin(), this->VerticalEdgeList.end(), edge4, VerticalCompLess), edge4);

	// Initialize ComponentVertexList
	this->ComponentVertexList.push_back((ComponentVertex*)point1);
	this->ComponentVertexList.push_back((ComponentVertex*)point2);
	this->ComponentVertexList.push_back((ComponentVertex*)point3);
	this->ComponentVertexList.push_back((ComponentVertex*)point4);
}

void HananRoutingGraph::generateHananGrid()
{
	for (auto& component_vertex : this->ComponentVertexList)
	{
		// Extend up
		if (!component_vertex->is_extended[0] and (component_vertex->vertex_type == upleft or component_vertex->vertex_type == upright))
		{
			component_vertex->is_extended[0] = true;
			// Find the closest edge
			vector<HorizontalEdge*>::iterator it = lower_bound(this->HorizontalEdgeList.begin(), this->HorizontalEdgeList.end(), component_vertex, HorizontalCompLess1);
			// Inner Vertexs Buffer
			vector<HananVertex*> vertex_buffer;
			while (it != this->HorizontalEdgeList.begin())
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
					this->VertexList.push_back(vertex);
					this->VerticalEdgeList.insert(lower_bound(this->VerticalEdgeList.begin(), this->VerticalEdgeList.end(), edge, VerticalCompLess), edge);

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
						this->VertexList.push_back(vertex);
						this->VerticalEdgeList.insert(lower_bound(this->VerticalEdgeList.begin(), this->VerticalEdgeList.end(), edge, VerticalCompLess), edge);

						break;
					}
					else if ((*it)->edge_type == EdgeLine)
					{
						// Add the new inner vertex to buffer
						vertex_buffer.push_back(vertex);

						// Add new vertex to VertexList
						this->VertexList.push_back(vertex);
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
					this->VerticalEdgeList.insert(lower_bound(this->VerticalEdgeList.begin(), this->VerticalEdgeList.end(), edge, VerticalCompLess), edge);

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
					this->VerticalEdgeList.insert(lower_bound(this->VerticalEdgeList.begin(), this->VerticalEdgeList.end(), edge, VerticalCompLess), edge);

					break;
				}
			}
		}
		// Extend left
		if (!component_vertex->is_extended[1] and (component_vertex->vertex_type == upleft or component_vertex->vertex_type == downleft))
		{
			component_vertex->is_extended[1] = true;
			// Find the closest edge
			vector<VerticalEdge*>::iterator it = lower_bound(this->VerticalEdgeList.begin(), this->VerticalEdgeList.end(), component_vertex, VerticalCompLess1);
			// Inner Vertexs Buffer
			vector<HananVertex*> vertex_buffer;
			while (it != this->VerticalEdgeList.begin())
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
					this->VertexList.push_back(vertex);
					this->HorizontalEdgeList.insert(lower_bound(this->HorizontalEdgeList.begin(), this->HorizontalEdgeList.end(), edge, HorizontalCompLess), edge);

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
						this->VertexList.push_back(vertex);
						this->HorizontalEdgeList.insert(lower_bound(this->HorizontalEdgeList.begin(), this->HorizontalEdgeList.end(), edge, HorizontalCompLess), edge);

						break;
					}
					else if ((*it)->edge_type == EdgeLine)
					{
						// Add the new inner vertex to buffer
						vertex_buffer.push_back(vertex);

						// Add new vertex to VertexList
						this->VertexList.push_back(vertex);
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
					this->HorizontalEdgeList.insert(lower_bound(this->HorizontalEdgeList.begin(), this->HorizontalEdgeList.end(), edge, HorizontalCompLess), edge);

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
					this->HorizontalEdgeList.insert(lower_bound(this->HorizontalEdgeList.begin(), this->HorizontalEdgeList.end(), edge, HorizontalCompLess), edge);

					break;
				}
			}
		}
		// Extend down
		if (!component_vertex->is_extended[0] and (component_vertex->vertex_type == downleft or component_vertex->vertex_type == downright))
		{
			component_vertex->is_extended[0] = true;
			// Find the closest edge
			vector<HorizontalEdge*>::iterator it = upper_bound(this->HorizontalEdgeList.begin(), this->HorizontalEdgeList.end(), component_vertex, HorizontalCompLess2);
			// Inner Vertexs Buffer
			vector<HananVertex*> vertex_buffer;
			while (it != this->HorizontalEdgeList.end())
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
					this->VertexList.push_back(vertex);
					this->VerticalEdgeList.insert(lower_bound(this->VerticalEdgeList.begin(), this->VerticalEdgeList.end(), edge, VerticalCompLess), edge);

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
						this->VertexList.push_back(vertex);
						this->VerticalEdgeList.insert(lower_bound(this->VerticalEdgeList.begin(), this->VerticalEdgeList.end(), edge, VerticalCompLess), edge);

						break;
					}
					else if ((*it)->edge_type == EdgeLine)
					{
						// Add the new inner vertex to buffer
						vertex_buffer.push_back(vertex);

						// Add new vertex to VertexList
						this->VertexList.push_back(vertex);
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
					this->VerticalEdgeList.insert(lower_bound(this->VerticalEdgeList.begin(), this->VerticalEdgeList.end(), edge, VerticalCompLess), edge);

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
					this->VerticalEdgeList.insert(lower_bound(this->VerticalEdgeList.begin(), this->VerticalEdgeList.end(), edge, VerticalCompLess), edge);

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
			vector<VerticalEdge*>::iterator it = upper_bound(this->VerticalEdgeList.begin(), this->VerticalEdgeList.end(), component_vertex, VerticalCompLess2);
			// Inner Vertexs Buffer
			vector<HananVertex*> vertex_buffer;
			while (it != this->VerticalEdgeList.end())
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
					this->VertexList.push_back(vertex);
					this->HorizontalEdgeList.insert(lower_bound(this->HorizontalEdgeList.begin(), this->HorizontalEdgeList.end(), edge, HorizontalCompLess), edge);

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
						this->VertexList.push_back(vertex);
						this->HorizontalEdgeList.insert(lower_bound(this->HorizontalEdgeList.begin(), this->HorizontalEdgeList.end(), edge, HorizontalCompLess), edge);

						break;
					}
					else if ((*it)->edge_type == EdgeLine)
					{
						// Add the new inner vertex to buffer
						vertex_buffer.push_back(vertex);

						// Add new vertex to VertexList
						this->VertexList.push_back(vertex);
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
					this->HorizontalEdgeList.insert(lower_bound(this->HorizontalEdgeList.begin(), this->HorizontalEdgeList.end(), edge, HorizontalCompLess), edge);

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
					this->HorizontalEdgeList.insert(lower_bound(this->HorizontalEdgeList.begin(), this->HorizontalEdgeList.end(), edge, HorizontalCompLess), edge);

					break;
				}
				it = it + 1;
			}
		}
	}

	// disassemble the edges
	for (auto edge : this->HorizontalEdgeList)
	{
		edge->disassemble_edge(this->HorizontalEdgeMap);
	}
	for (auto edge : this->VerticalEdgeList)
	{
		edge->disassemble_edge(this->VerticalEdgeMap);
	}
}

void HananRoutingGraph::initialHananCellH(HorizontalEdge* edge)
{
	if ((edge->vertexs[0]->edges[2] and edge->vertexs[1]->edges[2]))
	{
		if (edge->edge_type != ComponentBoundary or ((HananEdge*)(edge->vertexs[0]->edges[2]))->edge_type != ComponentBoundary or ((HananEdge*)(edge->vertexs[1]->edges[2]))->edge_type != ComponentBoundary)
		{
			HananCell* cell = new HananCell(edge, (HananEdge*)(edge->vertexs[1]->edges[2]), (HananEdge*)(edge->vertexs[0]->edges[2]->vertexs[1]->edges[1]), (HananEdge*)(edge->vertexs[0]->edges[2]));
			RoutingArea* area = new RoutingArea;
			area->contain_edges.push_back(edge);
			area->contain_edges.push_back((HananEdge*)(edge->vertexs[1]->edges[2]));
			area->contain_edges.push_back((HananEdge*)(edge->vertexs[0]->edges[2]->vertexs[1]->edges[1]));
			area->contain_edges.push_back((HananEdge*)(edge->vertexs[0]->edges[2]));
			cell->routing_area_list.push_back(area);

			this->CellList.push_back(cell);

			edge->cells[1] = cell;
			edge->vertexs[1]->edges[2]->cells[0] = cell;
			edge->vertexs[0]->edges[2]->vertexs[1]->edges[1]->cells[0] = cell;
			edge->vertexs[0]->edges[2]->cells[1] = cell;
		}
	}
}

void HananRoutingGraph::initialAdjacentEdgeH(HorizontalEdge* edge)
{
	if (edge->vertexs[0]->edges[0] and edge->vertexs[1]->edges[0])
	{
		if (edge->edge_type != ComponentBoundary or ((HananEdge*)(edge->vertexs[0]->edges[0]))->edge_type != ComponentBoundary or ((HananEdge*)(edge->vertexs[1]->edges[0]))->edge_type != ComponentBoundary)
		{
			if (!(edge->is_extended_adjvex((HananEdge*)(edge->vertexs[0]->edges[0]))))
			{
				HananEdge* extend_routing_edge = (HananEdge*)(edge->vertexs[0]->edges[0]);
				float routing_weight = diagonal_distance(extend_routing_edge->root_node_edge()->center_position(), edge->root_node_edge()->center_position());
				
				RoutingEdge* adjacent_edge = new RoutingEdge(extend_routing_edge, routing_weight);
				edge->adjacent_edges[0].push_back(adjacent_edge);

				adjacent_edge = new RoutingEdge(edge, routing_weight);
				extend_routing_edge->adjacent_edges[1].push_back(adjacent_edge);

				edge->adjacent_edges[0].back()->inverse_edge = extend_routing_edge->adjacent_edges[1].back();
				extend_routing_edge->adjacent_edges[1].back()->inverse_edge = edge->adjacent_edges[0].back();

				// Add the Adjacent Edge to the Routing Area of the Cell
				/*if (edge->cells[0])
				{
					((HananCell*)(edge->cells[0]))->routing_area_list[0]->contain_routing_edges.push_back(edge->adjacent_edges.back());
					((HananCell*)(edge->cells[0]))->routing_area_list[0]->contain_routing_edges.push_back(((HananEdge*)(edge->vertexs[0]->edges[0]))->adjacent_edges.back());
				}*/
			}

			if (!(edge->is_extended_adjvex((HananEdge*)(edge->vertexs[1]->edges[0]))))
			{
				HananEdge* extend_routing_edge = (HananEdge*)(edge->vertexs[1]->edges[0]);
				float routing_weight = diagonal_distance(extend_routing_edge->root_node_edge()->center_position(), edge->root_node_edge()->center_position());

				RoutingEdge* adjacent_edge = new RoutingEdge(extend_routing_edge, routing_weight);
				edge->adjacent_edges[0].push_back(adjacent_edge);

				adjacent_edge = new RoutingEdge(edge, routing_weight);
				extend_routing_edge->adjacent_edges[0].push_back(adjacent_edge);

				edge->adjacent_edges[0].back()->inverse_edge = extend_routing_edge->adjacent_edges[0].back();
				extend_routing_edge->adjacent_edges[0].back()->inverse_edge = edge->adjacent_edges[0].back();

				// Add the Adjacent Edge to the Routing Area of the Cell
				/*if (edge->cells[0])
				{
					((HananCell*)(edge->cells[0]))->routing_area_list[0]->contain_routing_edges.push_back(edge->adjacent_edges.back());
					((HananCell*)(edge->cells[0]))->routing_area_list[0]->contain_routing_edges.push_back(((HananEdge*)(edge->vertexs[1]->edges[0]))->adjacent_edges.back());
				}*/
			}

			if (!(edge->is_extended_adjvex((HananEdge*)(edge->vertexs[0]->edges[0]->vertexs[0]->edges[1]))))
			{
				HananEdge* extend_routing_edge = (HananEdge*)(edge->vertexs[0]->edges[0]->vertexs[0]->edges[1]);
				float routing_weight = diagonal_distance(extend_routing_edge->root_node_edge()->center_position(), edge->root_node_edge()->center_position());

				RoutingEdge* adjacent_edge = new RoutingEdge(extend_routing_edge, routing_weight);
				edge->adjacent_edges[0].push_back(adjacent_edge);

				adjacent_edge = new RoutingEdge(edge, routing_weight);
				extend_routing_edge->adjacent_edges[1].push_back(adjacent_edge);

				edge->adjacent_edges[0].back()->inverse_edge = extend_routing_edge->adjacent_edges[1].back();
				extend_routing_edge->adjacent_edges[1].back()->inverse_edge = edge->adjacent_edges[0].back();

				// Add the Adjacent Edge to the Routing Area of the Cell
				/*if (edge->cells[0])
				{
					((HananCell*)(edge->cells[0]))->routing_area_list[0]->contain_routing_edges.push_back(edge->adjacent_edges.back());
					((HananCell*)(edge->cells[0]))->routing_area_list[0]->contain_routing_edges.push_back(((HananEdge*)(edge->vertexs[0]->edges[0]->vertexs[0]->edges[1]))->adjacent_edges.back());
				}*/
			}
		}
	}
	if (edge->vertexs[0]->edges[2] and edge->vertexs[1]->edges[2])
	{
		if (edge->edge_type != ComponentBoundary or ((HananEdge*)(edge->vertexs[0]->edges[2]))->edge_type != ComponentBoundary or ((HananEdge*)(edge->vertexs[1]->edges[2]))->edge_type != ComponentBoundary)
		{
			if (!(edge->is_extended_adjvex((HananEdge*)(edge->vertexs[0]->edges[2]))))
			{
				HananEdge* extend_routing_edge = (HananEdge*)(edge->vertexs[0]->edges[2]);
				float routing_weight = diagonal_distance(extend_routing_edge->root_node_edge()->center_position(), edge->root_node_edge()->center_position());

				RoutingEdge* adjacent_edge = new RoutingEdge(extend_routing_edge, routing_weight);
				edge->adjacent_edges[1].push_back(adjacent_edge);

				adjacent_edge = new RoutingEdge(edge, routing_weight);
				extend_routing_edge->adjacent_edges[1].push_back(adjacent_edge);

				edge->adjacent_edges[1].back()->inverse_edge = extend_routing_edge->adjacent_edges[1].back();
				extend_routing_edge->adjacent_edges[1].back()->inverse_edge = edge->adjacent_edges[1].back();

				// Add the Adjacent Edge to the Routing Area of the Cell
				/*if (edge->cells[1])
				{
					((HananCell*)(edge->cells[1]))->routing_area_list[0]->contain_routing_edges.push_back(edge->adjacent_edges.back());
					((HananCell*)(edge->cells[1]))->routing_area_list[0]->contain_routing_edges.push_back(((HananEdge*)(edge->vertexs[0]->edges[2]))->adjacent_edges.back());
				}*/
			}

			if (!(edge->is_extended_adjvex((HananEdge*)(edge->vertexs[1]->edges[2]))))
			{
				HananEdge* extend_routing_edge = (HananEdge*)(edge->vertexs[1]->edges[2]);
				float routing_weight = diagonal_distance(extend_routing_edge->root_node_edge()->center_position(), edge->root_node_edge()->center_position());

				RoutingEdge* adjacent_edge = new RoutingEdge(extend_routing_edge, routing_weight);
				edge->adjacent_edges[1].push_back(adjacent_edge);

				adjacent_edge = new RoutingEdge(edge, routing_weight);
				extend_routing_edge->adjacent_edges[0].push_back(adjacent_edge);

				edge->adjacent_edges[1].back()->inverse_edge = extend_routing_edge->adjacent_edges[0].back();
				extend_routing_edge->adjacent_edges[0].back()->inverse_edge = edge->adjacent_edges[1].back();

				// Add the Adjacent Edge to the Routing Area of the Cell
				/*if (edge->cells[1])
				{
					((HananCell*)(edge->cells[1]))->routing_area_list[0]->contain_routing_edges.push_back(edge->adjacent_edges.back());
					((HananCell*)(edge->cells[1]))->routing_area_list[0]->contain_routing_edges.push_back(((HananEdge*)(edge->vertexs[1]->edges[2]))->adjacent_edges.back());
				}*/
			}

			if (!(edge->is_extended_adjvex((HananEdge*)(edge->vertexs[0]->edges[2]->vertexs[1]->edges[1]))))
			{
				HananEdge* extend_routing_edge = (HananEdge*)(edge->vertexs[0]->edges[2]->vertexs[1]->edges[1]);
				float routing_weight = diagonal_distance(extend_routing_edge->root_node_edge()->center_position(), edge->root_node_edge()->center_position());

				RoutingEdge* adjacent_edge = new RoutingEdge(extend_routing_edge, routing_weight);
				edge->adjacent_edges[1].push_back(adjacent_edge);

				adjacent_edge = new RoutingEdge(edge, routing_weight);
				extend_routing_edge->adjacent_edges[0].push_back(adjacent_edge);

				edge->adjacent_edges[1].back()->inverse_edge = extend_routing_edge->adjacent_edges[0].back();
				extend_routing_edge->adjacent_edges[0].back()->inverse_edge = edge->adjacent_edges[1].back();

				// Add the Adjacent Edge to the Routing Area of the Cell
				/*if (edge->cells[1])
				{
					((HananCell*)(edge->cells[1]))->routing_area_list[0]->contain_routing_edges.push_back(edge->adjacent_edges.back());
					((HananCell*)(edge->cells[1]))->routing_area_list[0]->contain_routing_edges.push_back(((HananEdge*)(edge->vertexs[0]->edges[2]->vertexs[1]->edges[1]))->adjacent_edges.back());
				}*/
			}
		}
	}
}

void HananRoutingGraph::initialAdjacentEdgeV(VerticalEdge* edge)
{
	if (edge->vertexs[0]->edges[1] and edge->vertexs[1]->edges[1])
	{
		if (edge->edge_type != ComponentBoundary or ((HananEdge*)(edge->vertexs[0]->edges[1]))->edge_type != ComponentBoundary or ((HananEdge*)(edge->vertexs[1]->edges[1]))->edge_type != ComponentBoundary)
		{
			if (!(edge->is_extended_adjvex((HananEdge*)(edge->vertexs[0]->edges[1]))))
			{
				HananEdge* extend_routing_edge = (HananEdge*)(edge->vertexs[0]->edges[1]);
				float routing_weight = diagonal_distance(extend_routing_edge->root_node_edge()->center_position(), edge->root_node_edge()->center_position());

				RoutingEdge* adjacent_edge = new RoutingEdge(extend_routing_edge, routing_weight);
				edge->adjacent_edges[1].push_back(adjacent_edge);

				adjacent_edge = new RoutingEdge(edge, routing_weight);
				extend_routing_edge->adjacent_edges[1].push_back(adjacent_edge);

				edge->adjacent_edges[1].back()->inverse_edge = extend_routing_edge->adjacent_edges[1].back();
				extend_routing_edge->adjacent_edges[1].back()->inverse_edge = edge->adjacent_edges[1].back();

				// Add the Adjacent Edge to the Routing Area of the Cell
				/*if (edge->cells[1])
				{
					((HananCell*)(edge->cells[1]))->routing_area_list[0]->contain_routing_edges.push_back(edge->adjacent_edges.back());
					((HananCell*)(edge->cells[1]))->routing_area_list[0]->contain_routing_edges.push_back(((HananEdge*)(edge->vertexs[0]->edges[1]))->adjacent_edges.back());
				}*/
			}

			if (!(edge->is_extended_adjvex((HananEdge*)(edge->vertexs[1]->edges[1]))))
			{
				HananEdge* extend_routing_edge = (HananEdge*)(edge->vertexs[1]->edges[1]);
				float routing_weight = diagonal_distance(extend_routing_edge->root_node_edge()->center_position(), edge->root_node_edge()->center_position());

				RoutingEdge* adjacent_edge = new RoutingEdge(extend_routing_edge, routing_weight);
				edge->adjacent_edges[1].push_back(adjacent_edge);

				adjacent_edge = new RoutingEdge(edge, routing_weight);
				extend_routing_edge->adjacent_edges[0].push_back(adjacent_edge);

				edge->adjacent_edges[1].back()->inverse_edge = extend_routing_edge->adjacent_edges[0].back();
				extend_routing_edge->adjacent_edges[0].back()->inverse_edge = edge->adjacent_edges[1].back();

				// Add the Adjacent Edge to the Routing Area of the Cell
				/*if (edge->cells[1])
				{
					((HananCell*)(edge->cells[1]))->routing_area_list[0]->contain_routing_edges.push_back(edge->adjacent_edges.back());
					((HananCell*)(edge->cells[1]))->routing_area_list[0]->contain_routing_edges.push_back(((HananEdge*)(edge->vertexs[1]->edges[1]))->adjacent_edges.back());
				}*/
			}

			if (!(edge->is_extended_adjvex((HananEdge*)(edge->vertexs[0]->edges[1]->vertexs[1]->edges[2]))))
			{
				HananEdge* extend_routing_edge = (HananEdge*)(edge->vertexs[0]->edges[1]->vertexs[1]->edges[2]);
				float routing_weight = diagonal_distance(extend_routing_edge->root_node_edge()->center_position(), edge->root_node_edge()->center_position());

				RoutingEdge* adjacent_edge = new RoutingEdge(extend_routing_edge, routing_weight);
				edge->adjacent_edges[1].push_back(adjacent_edge);

				adjacent_edge = new RoutingEdge(edge, routing_weight);
				extend_routing_edge->adjacent_edges[0].push_back(adjacent_edge);

				edge->adjacent_edges[1].back()->inverse_edge = extend_routing_edge->adjacent_edges[0].back();
				extend_routing_edge->adjacent_edges[0].back()->inverse_edge = edge->adjacent_edges[1].back();

				// Add the Adjacent Edge to the Routing Area of the Cell
				/*if (edge->cells[1])
				{
					((HananCell*)(edge->cells[1]))->routing_area_list[0]->contain_routing_edges.push_back(edge->adjacent_edges.back());
					((HananCell*)(edge->cells[1]))->routing_area_list[0]->contain_routing_edges.push_back(((HananEdge*)(edge->vertexs[0]->edges[1]->vertexs[1]->edges[2]))->adjacent_edges.back());
				}*/
			}
		}
	}
	if (edge->vertexs[0]->edges[3] and edge->vertexs[1]->edges[3])
	{
		if (edge->edge_type != ComponentBoundary or ((HananEdge*)(edge->vertexs[0]->edges[3]))->edge_type != ComponentBoundary or ((HananEdge*)(edge->vertexs[1]->edges[3]))->edge_type != ComponentBoundary)
		{
			if (!(edge->is_extended_adjvex((HananEdge*)(edge->vertexs[0]->edges[3]))))
			{
				HananEdge* extend_routing_edge = (HananEdge*)(edge->vertexs[0]->edges[3]);
				float routing_weight = diagonal_distance(extend_routing_edge->root_node_edge()->center_position(), edge->root_node_edge()->center_position());

				RoutingEdge* adjacent_edge = new RoutingEdge(extend_routing_edge, routing_weight);
				edge->adjacent_edges[0].push_back(adjacent_edge);

				adjacent_edge = new RoutingEdge(edge, routing_weight);
				extend_routing_edge->adjacent_edges[1].push_back(adjacent_edge);

				edge->adjacent_edges[0].back()->inverse_edge = extend_routing_edge->adjacent_edges[1].back();
				extend_routing_edge->adjacent_edges[1].back()->inverse_edge = edge->adjacent_edges[0].back();

				// Add the Adjacent Edge to the Routing Area of the Cell
				/*if (edge->cells[0])
				{
					((HananCell*)(edge->cells[0]))->routing_area_list[0]->contain_routing_edges.push_back(edge->adjacent_edges.back());
					((HananCell*)(edge->cells[0]))->routing_area_list[0]->contain_routing_edges.push_back(((HananEdge*)(edge->vertexs[0]->edges[3]))->adjacent_edges.back());
				}*/
			}

			if (!(edge->is_extended_adjvex((HananEdge*)(edge->vertexs[1]->edges[3]))))
			{
				HananEdge* extend_routing_edge = (HananEdge*)(edge->vertexs[1]->edges[3]);
				float routing_weight = diagonal_distance(extend_routing_edge->root_node_edge()->center_position(), edge->root_node_edge()->center_position());

				RoutingEdge* adjacent_edge = new RoutingEdge(extend_routing_edge, routing_weight);
				edge->adjacent_edges[0].push_back(adjacent_edge);

				adjacent_edge = new RoutingEdge(edge, routing_weight);
				extend_routing_edge->adjacent_edges[0].push_back(adjacent_edge);

				edge->adjacent_edges[0].back()->inverse_edge = extend_routing_edge->adjacent_edges[0].back();
				extend_routing_edge->adjacent_edges[0].back()->inverse_edge = edge->adjacent_edges[0].back();

				// Add the Adjacent Edge to the Routing Area of the Cell
				/*if (edge->cells[0])
				{
					((HananCell*)(edge->cells[0]))->routing_area_list[0]->contain_routing_edges.push_back(edge->adjacent_edges.back());
					((HananCell*)(edge->cells[0]))->routing_area_list[0]->contain_routing_edges.push_back(((HananEdge*)(edge->vertexs[1]->edges[3]))->adjacent_edges.back());
				}*/
			}

			if (!(edge->is_extended_adjvex((HananEdge*)(edge->vertexs[0]->edges[3]->vertexs[0]->edges[2]))))
			{
				HananEdge* extend_routing_edge = (HananEdge*)(edge->vertexs[0]->edges[3]->vertexs[0]->edges[2]);
				float routing_weight = diagonal_distance(extend_routing_edge->root_node_edge()->center_position(), edge->root_node_edge()->center_position());

				RoutingEdge* adjacent_edge = new RoutingEdge(extend_routing_edge, routing_weight);
				edge->adjacent_edges[0].push_back(adjacent_edge);

				adjacent_edge = new RoutingEdge(edge, routing_weight);
				extend_routing_edge->adjacent_edges[1].push_back(adjacent_edge);

				edge->adjacent_edges[0].back()->inverse_edge = extend_routing_edge->adjacent_edges[1].back();
				extend_routing_edge->adjacent_edges[1].back()->inverse_edge = edge->adjacent_edges[0].back();

				// Add the Adjacent Edge to the Routing Area of the Cell
				/*if (edge->cells[0])
				{
					((HananCell*)(edge->cells[0]))->routing_area_list[0]->contain_routing_edges.push_back(edge->adjacent_edges.back());
					((HananCell*)(edge->cells[0]))->routing_area_list[0]->contain_routing_edges.push_back(((HananEdge*)(edge->vertexs[0]->edges[3]->vertexs[0]->edges[2]))->adjacent_edges.back());
				}*/
			}
		}
	}
}

void HananRoutingGraph::generateHananRoutingGraph()
{
	// initial adjacent edges of the edges
	for (auto& edges : this->HorizontalEdgeMap)
	{
		for (auto& edge : edges.second)
		{
			//edge->initial_adjacent_edges();
			
			// Generate the basic Hanan Cell
			this->initialHananCellH(edge);
			
			// Generate the Adjacent Edge by Horizontal Edge and divide them to Routing Area
			this->initialAdjacentEdgeH(edge);
		}
	}
	for (auto& edges : this->VerticalEdgeMap)
	{
		for (auto& edge : edges.second)
		{
			//edge->initial_adjacent_edges();

			// Generate the Adjacent Edge by Vertical Edge and divide them to Routing Area
			this->initialAdjacentEdgeV(edge);
		}
	}
}

HananCell* HananRoutingGraph::findHananCell(HananEdge* edge1, HananEdge* edge2)
{
	if (edge1->cells[0] == edge2->cells[0] or edge1->cells[0] == edge2->cells[1])
	{
		return (HananCell*)(edge1->cells[0]);
	}
	if (edge1->cells[1] == edge2->cells[0] or edge1->cells[1] == edge2->cells[1])
	{
		return (HananCell*)(edge1->cells[1]);
	}
	return nullptr;
}

void HananRoutingGraph::updateRoutingGraph(vector<HananEdge*>& path, int bus_id)
{
	// Store the path
	this->pathList.push_back(path);

	// Generate the empty children edges of the path edges
	for (auto& edge : path)
	{
		edge->split2child();
	}
	
	int path_size = path.size();
	// Visit all the cells on the path
	for (int path_i = 0; path_i < path_size - 1; path_i++)
	{
		HananCell* cell = this->findHananCell(path[path_i], path[path_i + 1]);
		cell->path_line.push_back(pair<Line, int>{Line(path[path_i]->center_position(), path[path_i + 1]->center_position()), bus_id});
		
		/*
		* Clear all the adjacent edges in the cell
		* use pre-order traversal to find the non-leaf node
		*/
		for (int i = 0; i < 4; i++)
		{
			if ((HananEdge*)(cell->edges[i]) == NULL) return;
			stack<HananEdge*> s;
			s.push((HananEdge*)(cell->edges[i]));
			while (!s.empty()) {
				HananEdge* nd = s.top();

				// visit(nd)
				int direct = 0;
				if (i == 0 or i == 3)
				{
					direct = 1;
				}
				for (auto& routing_edge : nd->adjacent_edges[direct])
				{
					delete routing_edge;
				}
				nd->adjacent_edges[direct].clear();

				s.pop();
				if (nd->dynamic_graph_rchild != NULL)
					s.push(nd->dynamic_graph_rchild);
				if (nd->dynamic_graph_lchild != NULL)
					s.push(nd->dynamic_graph_lchild);
			}
		}
		/*for (auto& routing_edge : ((HananEdge*)(cell->edges[0]))->adjacent_edges[1])
		{
			delete routing_edge;
		}
		((HananEdge*)(cell->edges[0]))->adjacent_edges[1].clear();*/
		
		/*for (auto& routing_edge : ((HananEdge*)(cell->edges[1]))->adjacent_edges[0])
		{
			delete routing_edge;
		}
		((HananEdge*)(cell->edges[1]))->adjacent_edges[0].clear();*/
		
		/*for (auto& routing_edge : ((HananEdge*)(cell->edges[2]))->adjacent_edges[0])
		{
			delete routing_edge;
		}
		((HananEdge*)(cell->edges[2]))->adjacent_edges[0].clear();*/
		
		/*for (auto& routing_edge : ((HananEdge*)(cell->edges[3]))->adjacent_edges[1])
		{
			delete routing_edge;
		}
		((HananEdge*)(cell->edges[3]))->adjacent_edges[1].clear();*/

		vector<HananEdge*> hanan_edges[4];
		((HananEdge*)(cell->edges[0]))->leaf_node_list(hanan_edges[0]);
		((HananEdge*)(cell->edges[1]))->leaf_node_list(hanan_edges[1]);
		((HananEdge*)(cell->edges[2]))->leaf_node_list(hanan_edges[2]);
		((HananEdge*)(cell->edges[3]))->leaf_node_list(hanan_edges[3]);

		for (auto& node : hanan_edges[0])
		{
			for (auto& edge_node : hanan_edges[1])
			{
				if (!(node->is_extended_adjvex(edge_node)))
				{
					float routing_weight = diagonal_distance(node->root_node_edge()->center_position(), edge_node->root_node_edge()->center_position());

					RoutingEdge* adjacent_edge = new RoutingEdge(edge_node, routing_weight);
					node->adjacent_edges[1].push_back(adjacent_edge);

					adjacent_edge = new RoutingEdge(node, routing_weight);
					edge_node->adjacent_edges[0].push_back(adjacent_edge);

					Line routing_line = Line(node->center_position(), edge_node->center_position());
					for (auto& path_l : cell->path_line)
					{
						if (is_intersect(path_l.first, routing_line))
						{
							node->adjacent_edges[1].back()->conflict_id.push_back(path_l.second);
							edge_node->adjacent_edges[0].back()->conflict_id.push_back(path_l.second);
						}
					}

					node->adjacent_edges[1].back()->inverse_edge = edge_node->adjacent_edges[0].back();
					edge_node->adjacent_edges[0].back()->inverse_edge = node->adjacent_edges[1].back();
				}
			}
			for (auto& edge_node : hanan_edges[2])
			{
				if (!(node->is_extended_adjvex(edge_node)))
				{
					float routing_weight = diagonal_distance(node->root_node_edge()->center_position(), edge_node->root_node_edge()->center_position());

					RoutingEdge* adjacent_edge = new RoutingEdge(edge_node, routing_weight);
					node->adjacent_edges[1].push_back(adjacent_edge);

					adjacent_edge = new RoutingEdge(node, routing_weight);
					edge_node->adjacent_edges[0].push_back(adjacent_edge);

					Line routing_line = Line(node->center_position(), edge_node->center_position());
					for (auto& path_l : cell->path_line)
					{
						if (is_intersect(path_l.first, routing_line))
						{
							node->adjacent_edges[1].back()->conflict_id.push_back(path_l.second);
							edge_node->adjacent_edges[0].back()->conflict_id.push_back(path_l.second);
						}
					}

					node->adjacent_edges[1].back()->inverse_edge = edge_node->adjacent_edges[0].back();
					edge_node->adjacent_edges[0].back()->inverse_edge = node->adjacent_edges[1].back();
				}
			}
			for (auto& edge_node : hanan_edges[3])
			{
				if (!(node->is_extended_adjvex(edge_node)))
				{
					float routing_weight = diagonal_distance(node->root_node_edge()->center_position(), edge_node->root_node_edge()->center_position());

					RoutingEdge* adjacent_edge = new RoutingEdge(edge_node, routing_weight);
					node->adjacent_edges[1].push_back(adjacent_edge);

					adjacent_edge = new RoutingEdge(node, routing_weight);
					edge_node->adjacent_edges[1].push_back(adjacent_edge);

					Line routing_line = Line(node->center_position(), edge_node->center_position());
					for (auto& path_l : cell->path_line)
					{
						if (is_intersect(path_l.first, routing_line))
						{
							node->adjacent_edges[1].back()->conflict_id.push_back(path_l.second);
							edge_node->adjacent_edges[1].back()->conflict_id.push_back(path_l.second);
						}
					}

					node->adjacent_edges[1].back()->inverse_edge = edge_node->adjacent_edges[1].back();
					edge_node->adjacent_edges[1].back()->inverse_edge = node->adjacent_edges[1].back();
				}
			}
		}

		for (auto& node : hanan_edges[1])
		{
			for (auto& edge_node : hanan_edges[2])
			{
				if (!(node->is_extended_adjvex(edge_node)))
				{
					float routing_weight = diagonal_distance(node->root_node_edge()->center_position(), edge_node->root_node_edge()->center_position());

					RoutingEdge* adjacent_edge = new RoutingEdge(edge_node, routing_weight);
					node->adjacent_edges[0].push_back(adjacent_edge);

					adjacent_edge = new RoutingEdge(node, routing_weight);
					edge_node->adjacent_edges[0].push_back(adjacent_edge);

					Line routing_line = Line(node->center_position(), edge_node->center_position());
					for (auto& path_l : cell->path_line)
					{
						if (is_intersect(path_l.first, routing_line))
						{
							node->adjacent_edges[0].back()->conflict_id.push_back(path_l.second);
							edge_node->adjacent_edges[0].back()->conflict_id.push_back(path_l.second);
						}
					}

					node->adjacent_edges[0].back()->inverse_edge = edge_node->adjacent_edges[0].back();
					edge_node->adjacent_edges[0].back()->inverse_edge = node->adjacent_edges[0].back();
				}
			}
			for (auto& edge_node : hanan_edges[3])
			{
				if (!(node->is_extended_adjvex(edge_node)))
				{
					float routing_weight = diagonal_distance(node->root_node_edge()->center_position(), edge_node->root_node_edge()->center_position());

					RoutingEdge* adjacent_edge = new RoutingEdge(edge_node, routing_weight);
					node->adjacent_edges[0].push_back(adjacent_edge);

					adjacent_edge = new RoutingEdge(node, routing_weight);
					edge_node->adjacent_edges[1].push_back(adjacent_edge);

					Line routing_line = Line(node->center_position(), edge_node->center_position());
					for (auto& path_l : cell->path_line)
					{
						if (is_intersect(path_l.first, routing_line))
						{
							node->adjacent_edges[0].back()->conflict_id.push_back(path_l.second);
							edge_node->adjacent_edges[1].back()->conflict_id.push_back(path_l.second);
						}
					}

					node->adjacent_edges[0].back()->inverse_edge = edge_node->adjacent_edges[1].back();
					edge_node->adjacent_edges[1].back()->inverse_edge = node->adjacent_edges[0].back();
				}
			}
			for (auto& edge_node : hanan_edges[0])
			{
				if (!(node->is_extended_adjvex(edge_node)))
				{
					float routing_weight = diagonal_distance(node->root_node_edge()->center_position(), edge_node->root_node_edge()->center_position());

					RoutingEdge* adjacent_edge = new RoutingEdge(edge_node, routing_weight);
					node->adjacent_edges[0].push_back(adjacent_edge);

					adjacent_edge = new RoutingEdge(node, routing_weight);
					edge_node->adjacent_edges[1].push_back(adjacent_edge);

					Line routing_line = Line(node->center_position(), edge_node->center_position());
					for (auto& path_l : cell->path_line)
					{
						if (is_intersect(path_l.first, routing_line))
						{
							node->adjacent_edges[0].back()->conflict_id.push_back(path_l.second);
							edge_node->adjacent_edges[1].back()->conflict_id.push_back(path_l.second);
						}
					}

					node->adjacent_edges[0].back()->inverse_edge = edge_node->adjacent_edges[1].back();
					edge_node->adjacent_edges[1].back()->inverse_edge = node->adjacent_edges[0].back();
				}
			}
		}

		for (auto& node : hanan_edges[2])
		{
			for (auto& edge_node : hanan_edges[3])
			{
				if (!(node->is_extended_adjvex(edge_node)))
				{
					float routing_weight = diagonal_distance(node->root_node_edge()->center_position(), edge_node->root_node_edge()->center_position());

					RoutingEdge* adjacent_edge = new RoutingEdge(edge_node, routing_weight);
					node->adjacent_edges[0].push_back(adjacent_edge);

					adjacent_edge = new RoutingEdge(node, routing_weight);
					edge_node->adjacent_edges[1].push_back(adjacent_edge);

					Line routing_line = Line(node->center_position(), edge_node->center_position());
					for (auto& path_l : cell->path_line)
					{
						if (is_intersect(path_l.first, routing_line))
						{
							node->adjacent_edges[0].back()->conflict_id.push_back(path_l.second);
							edge_node->adjacent_edges[1].back()->conflict_id.push_back(path_l.second);
						}
					}

					node->adjacent_edges[0].back()->inverse_edge = edge_node->adjacent_edges[1].back();
					edge_node->adjacent_edges[1].back()->inverse_edge = node->adjacent_edges[0].back();
				}
			}
			for (auto& edge_node : hanan_edges[0])
			{
				if (!(node->is_extended_adjvex(edge_node)))
				{
					float routing_weight = diagonal_distance(node->root_node_edge()->center_position(), edge_node->root_node_edge()->center_position());

					RoutingEdge* adjacent_edge = new RoutingEdge(edge_node, routing_weight);
					node->adjacent_edges[0].push_back(adjacent_edge);

					adjacent_edge = new RoutingEdge(node, routing_weight);
					edge_node->adjacent_edges[1].push_back(adjacent_edge);

					Line routing_line = Line(node->center_position(), edge_node->center_position());
					for (auto& path_l : cell->path_line)
					{
						if (is_intersect(path_l.first, routing_line))
						{
							node->adjacent_edges[0].back()->conflict_id.push_back(path_l.second);
							edge_node->adjacent_edges[1].back()->conflict_id.push_back(path_l.second);
						}
					}

					node->adjacent_edges[0].back()->inverse_edge = edge_node->adjacent_edges[1].back();
					edge_node->adjacent_edges[1].back()->inverse_edge = node->adjacent_edges[0].back();
				}
			}
			for (auto& edge_node : hanan_edges[1])
			{
				if (!(node->is_extended_adjvex(edge_node)))
				{
					float routing_weight = diagonal_distance(node->root_node_edge()->center_position(), edge_node->root_node_edge()->center_position());

					RoutingEdge* adjacent_edge = new RoutingEdge(edge_node, routing_weight);
					node->adjacent_edges[0].push_back(adjacent_edge);

					adjacent_edge = new RoutingEdge(node, routing_weight);
					edge_node->adjacent_edges[0].push_back(adjacent_edge);

					Line routing_line = Line(node->center_position(), edge_node->center_position());
					for (auto& path_l : cell->path_line)
					{
						if (is_intersect(path_l.first, routing_line))
						{
							node->adjacent_edges[0].back()->conflict_id.push_back(path_l.second);
							edge_node->adjacent_edges[0].back()->conflict_id.push_back(path_l.second);
						}
					}

					node->adjacent_edges[0].back()->inverse_edge = edge_node->adjacent_edges[0].back();
					edge_node->adjacent_edges[0].back()->inverse_edge = node->adjacent_edges[0].back();
				}
			}
		}

		for (auto& node : hanan_edges[3])
		{
			for (auto& edge_node : hanan_edges[0])
			{
				if (!(node->is_extended_adjvex(edge_node)))
				{
					float routing_weight = diagonal_distance(node->root_node_edge()->center_position(), edge_node->root_node_edge()->center_position());

					RoutingEdge* adjacent_edge = new RoutingEdge(edge_node, routing_weight);
					node->adjacent_edges[1].push_back(adjacent_edge);

					adjacent_edge = new RoutingEdge(node, routing_weight);
					edge_node->adjacent_edges[1].push_back(adjacent_edge);

					Line routing_line = Line(node->center_position(), edge_node->center_position());
					for (auto& path_l : cell->path_line)
					{
						if (is_intersect(path_l.first, routing_line))
						{
							node->adjacent_edges[1].back()->conflict_id.push_back(path_l.second);
							edge_node->adjacent_edges[1].back()->conflict_id.push_back(path_l.second);
						}
					}

					node->adjacent_edges[1].back()->inverse_edge = edge_node->adjacent_edges[1].back();
					edge_node->adjacent_edges[1].back()->inverse_edge = node->adjacent_edges[1].back();
				}
			}
			for (auto& edge_node : hanan_edges[1])
			{
				if (!(node->is_extended_adjvex(edge_node)))
				{
					float routing_weight = diagonal_distance(node->root_node_edge()->center_position(), edge_node->root_node_edge()->center_position());

					RoutingEdge* adjacent_edge = new RoutingEdge(edge_node, routing_weight);
					node->adjacent_edges[1].push_back(adjacent_edge);

					adjacent_edge = new RoutingEdge(node, routing_weight);
					edge_node->adjacent_edges[0].push_back(adjacent_edge);

					Line routing_line = Line(node->center_position(), edge_node->center_position());
					for (auto& path_l : cell->path_line)
					{
						if (is_intersect(path_l.first, routing_line))
						{
							node->adjacent_edges[1].back()->conflict_id.push_back(path_l.second);
							edge_node->adjacent_edges[0].back()->conflict_id.push_back(path_l.second);
						}
					}

					node->adjacent_edges[1].back()->inverse_edge = edge_node->adjacent_edges[0].back();
					edge_node->adjacent_edges[0].back()->inverse_edge = node->adjacent_edges[1].back();
				}
			}
			for (auto& edge_node : hanan_edges[2])
			{
				if (!(node->is_extended_adjvex(edge_node)))
				{
					float routing_weight = diagonal_distance(node->root_node_edge()->center_position(), edge_node->root_node_edge()->center_position());

					RoutingEdge* adjacent_edge = new RoutingEdge(edge_node, routing_weight);
					node->adjacent_edges[1].push_back(adjacent_edge);

					adjacent_edge = new RoutingEdge(node, routing_weight);
					edge_node->adjacent_edges[0].push_back(adjacent_edge);

					Line routing_line = Line(node->center_position(), edge_node->center_position());
					for (auto& path_l : cell->path_line)
					{
						if (is_intersect(path_l.first, routing_line))
						{
							node->adjacent_edges[1].back()->conflict_id.push_back(path_l.second);
							edge_node->adjacent_edges[0].back()->conflict_id.push_back(path_l.second);
						}
					}

					node->adjacent_edges[1].back()->inverse_edge = edge_node->adjacent_edges[0].back();
					edge_node->adjacent_edges[0].back()->inverse_edge = node->adjacent_edges[1].back();
				}
			}
		}
	}
}

void HananRoutingGraph::printHananGrid()
{
	// Print HorizontalEdge
	cout << "----------- Horizontal Edge List -----------" << endl;
	int i = 0;
	vector<float> x(2);
	vector<float> y(2);
	for (auto edge : this->HorizontalEdgeList)
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
	for (auto edge : this->VerticalEdgeList)
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

void HananRoutingGraph::printRoutingGraph()
{
	vector<float> x(2);
	vector<float> y(2);
	// draw Horizontal Edges
	for (auto& edges : this->HorizontalEdgeMap)
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
					for (auto& routing_edge : nd->adjacent_edges[0])
					{
						x[1] = (routing_edge->adjvex->vertexs[0]->position.x + routing_edge->adjvex->vertexs[1]->position.x) / 2.0;
						y[1] = (routing_edge->adjvex->vertexs[0]->position.y + routing_edge->adjvex->vertexs[1]->position.y) / 2.0;
						if (routing_edge->conflict_id.empty())
						{
							plot::plot(x, y, "g");
						}
						else
						{
							plot::plot(x, y, { {"color", "g"}, {"linestyle", ":"} });
						}
					}
					for (auto& routing_edge : nd->adjacent_edges[1])
					{
						x[1] = (routing_edge->adjvex->vertexs[0]->position.x + routing_edge->adjvex->vertexs[1]->position.x) / 2.0;
						y[1] = (routing_edge->adjvex->vertexs[0]->position.y + routing_edge->adjvex->vertexs[1]->position.y) / 2.0;
						if (routing_edge->conflict_id.empty())
						{
							plot::plot(x, y, "g");
						}
						else
						{
							plot::plot(x, y, { {"color", "g"}, {"linestyle", ":"} });
						}
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
	for (auto& edges : this->VerticalEdgeMap)
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
					for (auto& routing_edge : nd->adjacent_edges[0])
					{
						x[1] = (routing_edge->adjvex->vertexs[0]->position.x + routing_edge->adjvex->vertexs[1]->position.x) / 2.0;
						y[1] = (routing_edge->adjvex->vertexs[0]->position.y + routing_edge->adjvex->vertexs[1]->position.y) / 2.0;
						if (routing_edge->conflict_id.empty())
						{
							plot::plot(x, y, "g");
						}
						else
						{
							plot::plot(x, y, { {"color", "g"}, {"linestyle", ":"} });
						}
					}
					for (auto& routing_edge : nd->adjacent_edges[1])
					{
						x[1] = (routing_edge->adjvex->vertexs[0]->position.x + routing_edge->adjvex->vertexs[1]->position.x) / 2.0;
						y[1] = (routing_edge->adjvex->vertexs[0]->position.y + routing_edge->adjvex->vertexs[1]->position.y) / 2.0;
						if (routing_edge->conflict_id.empty())
						{
							plot::plot(x, y, "g");
						}
						else
						{
							plot::plot(x, y, { {"color", "g"}, {"linestyle", ":"} });
						}
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
	plot::show();
}

void HananRoutingGraph::printRoutingPath()
{
	vector<float> x(2);
	vector<float> y(2);
	// draw Horizontal Edges
	for (auto& edges : this->HorizontalEdgeMap)
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
					for (auto& routing_edge : nd->adjacent_edges[0])
					{
						x[1] = (routing_edge->adjvex->vertexs[0]->position.x + routing_edge->adjvex->vertexs[1]->position.x) / 2.0;
						y[1] = (routing_edge->adjvex->vertexs[0]->position.y + routing_edge->adjvex->vertexs[1]->position.y) / 2.0;
						if (routing_edge->conflict_id.empty())
						{
							plot::plot(x, y, "g");
						}
						else
						{
							plot::plot(x, y, { {"color", "g"}, {"linestyle", ":"} });
						}
					}
					for (auto& routing_edge : nd->adjacent_edges[1])
					{
						x[1] = (routing_edge->adjvex->vertexs[0]->position.x + routing_edge->adjvex->vertexs[1]->position.x) / 2.0;
						y[1] = (routing_edge->adjvex->vertexs[0]->position.y + routing_edge->adjvex->vertexs[1]->position.y) / 2.0;
						if (routing_edge->conflict_id.empty())
						{
							plot::plot(x, y, "g");
						}
						else
						{
							plot::plot(x, y, { {"color", "g"}, {"linestyle", ":"} });
						}
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
	for (auto& edges : this->VerticalEdgeMap)
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
					for (auto& routing_edge : nd->adjacent_edges[0])
					{
						x[1] = (routing_edge->adjvex->vertexs[0]->position.x + routing_edge->adjvex->vertexs[1]->position.x) / 2.0;
						y[1] = (routing_edge->adjvex->vertexs[0]->position.y + routing_edge->adjvex->vertexs[1]->position.y) / 2.0;
						if (routing_edge->conflict_id.empty())
						{
							plot::plot(x, y, "g");
						}
						else
						{
							plot::plot(x, y, { {"color", "g"}, {"linestyle", ":"} });
						}
					}
					for (auto& routing_edge : nd->adjacent_edges[1])
					{
						x[1] = (routing_edge->adjvex->vertexs[0]->position.x + routing_edge->adjvex->vertexs[1]->position.x) / 2.0;
						y[1] = (routing_edge->adjvex->vertexs[0]->position.y + routing_edge->adjvex->vertexs[1]->position.y) / 2.0;
						if (routing_edge->conflict_id.empty())
						{
							plot::plot(x, y, "g");
						}
						else
						{
							plot::plot(x, y, { {"color", "g"}, {"linestyle", ":"} });
						}
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

	// draw the Routing path
	for (auto& path: this->pathList)
	{
		for (auto& path_node : path)
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
	}
	plot::show();
}
