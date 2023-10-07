#include "ultis.h"

void Vertex::PrintVertex()
{
	cout << "[ " << this->position.x << ", " << this->position.y << " ]";
}

bool is_intersect(Line myline1, Line myline2)
{
	if (myline1.get_max_x() < myline2.get_min_x() ||
		myline2.get_max_x() < myline1.get_min_x() ||
		myline1.get_max_y() < myline2.get_min_y() ||
		myline2.get_max_y() < myline1.get_min_y())   return false;
	float res1 = (myline1.xa - myline1.xb) * (myline2.ya - myline1.yb) - (myline1.ya - myline1.yb) * (myline2.xa - myline1.xb);
	float res2 = (myline1.xa - myline1.xb) * (myline2.yb - myline1.yb) - (myline1.ya - myline1.yb) * (myline2.xb - myline1.xb);

	float res3 = (myline2.xa - myline2.xb) * (myline1.ya - myline2.yb) - (myline2.ya - myline2.yb) * (myline1.xa - myline2.xb);
	float res4 = (myline2.xa - myline2.xb) * (myline1.yb - myline2.yb) - (myline2.ya - myline2.yb) * (myline1.xb - myline2.xb);
	if (res1 * res2 <= 0 && res3 * res4 <= 0) return true;
	else return false;
}

float diagonal_distance(Position point1, Position point2)
{
	float dx = abs(point1.x - point2.x);
	float dy = abs(point1.y - point2.y);
	float max_d = dx;
	float min_d = dy;
	if (dx < dy)
	{
		max_d = dy;
		min_d = dx;
	}
	return max_d - min_d + 1.414f * min_d;
}

