#pragma once
#include "ultis.h"

class HananCell : public Cell
{
public:
	HananCell(Edge* edge_1, Edge* edge_2, Edge* edge_3, Edge* edge_4) : Cell(edge_1, edge_2, edge_3, edge_4) {};
	~HananCell() {};
};

