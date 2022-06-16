#ifndef HOME_H
#define HOME_H
#include <string>
#include "Point.h"
#include "GlobalVariable.h"
using namespace std;

class Home {

public:
	//function members
	Home(double formal_x_left,double formal_x_right,double formal_y_up,double formal_y_down);
	const double x_left;
	const double x_right;
	const double y_up;
	const double y_down;
};
#endif