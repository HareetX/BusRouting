#pragma once
#include "ultis.h"

class Bus
{
public:
	Bus(Position start, Position end, float width, int id)
	{
		this->start = start;
		this->end = end;
		this->width = width;
		this->id = id;
	};
	~Bus() {};

	Position start; // start width ?
	Position end;   // end width ?

	float width;

	int id;
private:

};

