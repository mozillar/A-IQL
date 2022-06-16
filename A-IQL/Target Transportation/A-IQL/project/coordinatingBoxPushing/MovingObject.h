#ifndef MOVING_OBJECT_H
#define MOVING_OBJECT_H
#include <string>
#include "Point.h"
#include "GlobalVariable.h"
using namespace std;

class MovingObject {

public:
	//function members
	MovingObject(double formal_x_left,double formal_x_right,double formal_y_up,double formal_y_down);
	bool GetIsActive();
	void SetIsActive(bool formal_is_active);
	double GetSize();
	void SetSize(double formal_size);
	void Reset();
	void SetPos(double formal_x_left,double formal_x_right,double formal_y_up,double formal_y_down);
	//data member
	Point point[POINT_COUNT]; // 5 pionts:  four corner and the center
	const double ini_x_left;  //initial coordiate
	const double ini_x_right; //initial coordiate
	const double ini_y_up;    //initial coordiate
	const double ini_y_down;  //initial coordiate

	double x_left;  //initial coordiate
	double x_right; //initial coordiate
	double y_up;    //initial coordiate
	double y_down;  //initial coordiate
private:
	bool is_active;
    double size;  

};
#endif