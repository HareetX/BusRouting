#pragma once
#include <iostream>
#include <vector>
#include <unordered_map>
#include <map>
#include <stack>
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
	Cell* cells[2];  // 0 -> 1 : from small to large

private:
};


class Cell
{
public:
	Cell(Edge* edge_1, Edge* edge_2, Edge* edge_3, Edge* edge_4) { edges[0] = edge_1; edges[1] = edge_2; edges[2] = edge_3; edges[3] = edge_4; };
	~Cell() {};
	
	Edge* edges[4];  // 0 - up ; 1 - right ; 2 - down ; 3 - left

private:
};


class Line
{
public:
	float xa;
	float ya;
	float xb;
	float yb;
	Line(float xa, float ya, float xb, float yb) {
		this->xa = xa;
		this->ya = ya;
		this->xb = xb;
		this->yb = yb;
	};
	Line(Position a, Position b) {
		this->xa = a.x;
		this->ya = a.y;
		this->xb = b.x;
		this->yb = b.y;
	};
	float get_max_x() {
		return xa > xb ? xa : xb;
	};
	float get_min_x() {
		return xa > xb ? xb : xa;
	};
	float get_max_y() {
		return ya > yb ? ya : yb;
	};
	float get_min_y() {
		return ya > yb ? yb : ya;
	};
};

bool is_intersect(Line myline1, Line myline2);

float diagonal_distance(Position point1, Position point2);