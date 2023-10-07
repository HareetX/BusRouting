#pragma once
#include <queue>
#include "HananEdge.h"

class AStarNode
{
public:
	AStarNode(HananEdge* edge, float g, float h);
	AStarNode(HananEdge* edge, float g, float h, AStarNode* parent);
	~AStarNode() {};

	HananEdge* edge;

	float g;
	float h;
	float f;

	AStarNode* parent;

private:

};

struct cmp1 {
	bool operator()(const AStarNode* a, const AStarNode* b) {
		return -(a->f) < -(b->f);
	}
};

class AStarRouter
{
public:
	void a_star_router(HananEdge* start_edge, HananEdge* end_edge, vector<HananEdge*>& path);

private:

};

