#ifndef BARRIER_H
#define BARRIER_H
#include <string>
#include "Point.h"
#include "GlobalVariable.h"
using namespace std;

class Barrier {

public:
	//function members
	Barrier(double formal_x_left,double formal_x_right,double formal_y_up,double formal_y_down);
	Point point[POINT_COUNT];
	const double x_left;
	const double x_right;
	const double y_up;
	const double y_down;
	const double size;
};
#endif