#pragma once
#include "ultis.h"

enum VertexType
{
	BoardVertex_,
	ComponentVertex_,
	LineVertex_
};

class HananVertex : public Vertex
{
public:
	HananVertex(float x, float y) : Vertex(x, y) {};
	~HananVertex() {};
};


enum ComponentVertexType
{
	upleft,
	upright,
	downleft,
	downright
};


class ComponentVertex : public HananVertex
{
public:
	ComponentVertex(float x, float y, ComponentVertexType type) : HananVertex(x, y) { vertex_type = type; is_extended[0] = false; is_extended[1] = false; };
	~ComponentVertex() {};

	ComponentVertexType vertex_type;
	bool is_extended[2];

private:

};

