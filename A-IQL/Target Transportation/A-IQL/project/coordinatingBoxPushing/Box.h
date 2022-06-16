#ifndef BOX_H
#define BOX_H
#include <string>
#include "Point.h"
#include "GlobalVariable.h"
using namespace std;

class Box {

public:
	//function members
	Box(double formal_x_left,double formal_x_right,double formal_y_up,double formal_y_down);
	void SetPos(double formal_x_left,double formal_x_right,double formal_y_up,double formal_y_down);
	void Reset();
	//data members
	Point point[POINT_COUNT];
	const double ini_x_left;  //initial coordiate
	const double ini_x_right; //initial coordiate
	const double ini_y_up;    //initial coordiate
	const double ini_y_down;  //initial coordiate
	const double size;

	double x_left;  //current coordiate
	double x_right; //current coordiate
	double y_up;    //current coordiate
	double y_down;  //current coordiate
	
};
#endif