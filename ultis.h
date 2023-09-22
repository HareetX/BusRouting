#pragma once
#include <iostream>
#include <vector>
//#include <unordered_map>
#include <map>
using namespace std;

typedef struct
{
	float x;
	float y;
} Position;

class Edge;

class Vertex
{
public:
	Vertex(float x, float y) { position = Position{ x, y }; edges[0] = nullptr; edges[1] = nullptr; edges[2] = nullptr; edges[3] = nullptr; };
	~Vertex() {};
	
	Position position;

	Edge* edges[4];  // 0 - up ; 1 - right ; 2 - down ; 3 - left

	void PrintVertex();

private:
};


class Cell;


class Edge
{
public:
	Edge(Vertex* vertex_1, Vertex* vertex_2) { vertexs[0] = vertex_1; vertexs[1] = vertex_2; cells[0] = nullptr; cells[1] = nullptr; };
	~Edge() {};
	
	Vertex* vertexs[2];
	Cell* cells[2];

private:
};


class Cell
{
public:
	Cell(Edge* edge_1, Edge* edge_2, Edge* edge_3, Edge* edge_4) { edges[0] = edge_1; edges[1] = edge_2; edges[2] = edge_3; edges[3] = edge_4; };
	~Cell() {};
	
	Edge* edges[4];

private:
};