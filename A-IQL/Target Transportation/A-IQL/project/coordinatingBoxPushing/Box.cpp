#include "Box.h"
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <math.h>
using namespace std;
Box::Box(double formal_x_left,double formal_x_right,double formal_y_up,double formal_y_down):ini_x_left(formal_x_left),ini_x_right(formal_x_right),ini_y_up(formal_y_up),ini_y_down(formal_y_down),size((formal_x_right - formal_x_left)*(formal_y_down-formal_y_up))
{
	point[LEFT_UP].x = formal_x_left;
	point[LEFT_UP].y = formal_y_up;
	point[RIGHT_UP].x = formal_x_right;
	point[RIGHT_UP].y = formal_y_up;
	point[LEFT_DOWN].x = formal_x_left;
	point[LEFT_DOWN].y = formal_y_down;
	point[RIGHT_DOWN].x = formal_x_right;
	point[RIGHT_DOWN].y = formal_y_down;
	point[CENTER].x = (point[LEFT_UP].x + point[RIGHT_UP].x + point[LEFT_DOWN].x + point[RIGHT_DOWN].x)/4;
	point[CENTER].y = (point[LEFT_UP].y + point[RIGHT_UP].y + point[LEFT_DOWN].y + point[RIGHT_DOWN].y)/4;
	point[MIDDLE_LEFT].x = formal_x_left;
	point[MIDDLE_LEFT].y = (formal_y_up + formal_y_down)/2;
	point[MIDDLE_RIGHT].x = formal_x_right;
	point[MIDDLE_RIGHT].y = (formal_y_up + formal_y_down)/2;
	point[MIDDLE_UP].x = (formal_x_left + formal_x_right)/2;
	point[MIDDLE_UP].y = formal_y_up;
	point[MIDDLE_DOWN].x = (formal_x_left + formal_x_right)/2;
	point[MIDDLE_DOWN].y = formal_y_down;
	point[LEFT_CENTER].x = (point[LEFT_UP].x + point[MIDDLE_UP].x + point[LEFT_DOWN].x + point[MIDDLE_DOWN].x)/4;
	point[LEFT_CENTER].y = (point[LEFT_UP].y + point[MIDDLE_UP].y + point[LEFT_DOWN].y + point[MIDDLE_DOWN].y)/4;
	point[RIGHT_CENTER].x = (point[RIGHT_UP].x + point[MIDDLE_UP].x + point[RIGHT_DOWN].x + point[MIDDLE_DOWN].x)/4;
	point[RIGHT_CENTER].y = (point[RIGHT_UP].y + point[MIDDLE_UP].y + point[RIGHT_DOWN].y + point[MIDDLE_DOWN].y)/4;
}
void Box::Reset()
{
	point[LEFT_UP].x = ini_x_left;
	point[LEFT_UP].y = ini_y_up;
	point[RIGHT_UP].x = ini_x_right;
	point[RIGHT_UP].y = ini_y_up;
	point[LEFT_DOWN].x = ini_x_left;
	point[LEFT_DOWN].y = ini_y_down;
	point[RIGHT_DOWN].x = ini_x_right;
	point[RIGHT_DOWN].y = ini_y_down;
	point[CENTER].x = (point[LEFT_UP].x + point[RIGHT_UP].x + point[LEFT_DOWN].x + point[RIGHT_DOWN].x)/4;
	point[CENTER].y = (point[LEFT_UP].y + point[RIGHT_UP].y + point[LEFT_DOWN].y + point[RIGHT_DOWN].y)/4;
	point[MIDDLE_LEFT].x = ini_x_left;
	point[MIDDLE_LEFT].y = (ini_y_up + ini_y_down)/2;
	point[MIDDLE_RIGHT].x = ini_x_right;
	point[MIDDLE_RIGHT].y = (ini_y_up + ini_y_down)/2;
	point[MIDDLE_UP].x = (ini_x_left + ini_x_right)/2;
	point[MIDDLE_UP].y = ini_y_up;
	point[MIDDLE_DOWN].x = (ini_x_left + ini_x_right)/2;
	point[MIDDLE_DOWN].y = ini_y_down;
	point[LEFT_CENTER].x = (point[LEFT_UP].x + point[MIDDLE_UP].x + point[LEFT_DOWN].x + point[MIDDLE_DOWN].x)/4;
	point[LEFT_CENTER].y = (point[LEFT_UP].y + point[MIDDLE_UP].y + point[LEFT_DOWN].y + point[MIDDLE_DOWN].y)/4;
	point[RIGHT_CENTER].x = (point[RIGHT_UP].x + point[MIDDLE_UP].x + point[RIGHT_DOWN].x + point[MIDDLE_DOWN].x)/4;
	point[RIGHT_CENTER].y = (point[RIGHT_UP].y + point[MIDDLE_UP].y + point[RIGHT_DOWN].y + point[MIDDLE_DOWN].y)/4;

	x_left = ini_x_left;
	x_right = ini_x_right;
	y_up = ini_y_up;
	y_down = ini_y_down;
}
void Box::SetPos(double formal_x_left,double formal_x_right,double formal_y_up,double formal_y_down)
{
	point[LEFT_UP].x = formal_x_left;
	point[LEFT_UP].y = formal_y_up;
	point[RIGHT_UP].x = formal_x_right;
	point[RIGHT_UP].y = formal_y_up;
	point[LEFT_DOWN].x = formal_x_left;
	point[LEFT_DOWN].y = formal_y_down;
	point[RIGHT_DOWN].x = formal_x_right;
	point[RIGHT_DOWN].y = formal_y_down;
	point[CENTER].x = (point[LEFT_UP].x + point[RIGHT_UP].x + point[LEFT_DOWN].x + point[RIGHT_DOWN].x)/4;
	point[CENTER].y = (point[LEFT_UP].y + point[RIGHT_UP].y + point[LEFT_DOWN].y + point[RIGHT_DOWN].y)/4;
	point[MIDDLE_LEFT].x = formal_x_left;
	point[MIDDLE_LEFT].y = (formal_y_up + formal_y_down)/2;
	point[MIDDLE_RIGHT].x = formal_x_right;
	point[MIDDLE_RIGHT].y = (formal_y_up + formal_y_down)/2;
	point[MIDDLE_UP].x = (formal_x_left + formal_x_right)/2;
	point[MIDDLE_UP].y = formal_y_up;
	point[MIDDLE_DOWN].x = (formal_x_left + formal_x_right)/2;
	point[MIDDLE_DOWN].y = formal_y_down;
	point[LEFT_CENTER].x = (point[LEFT_UP].x + point[MIDDLE_UP].x + point[LEFT_DOWN].x + point[MIDDLE_DOWN].x)/4;
	point[LEFT_CENTER].y = (point[LEFT_UP].y + point[MIDDLE_UP].y + point[LEFT_DOWN].y + point[MIDDLE_DOWN].y)/4;
	point[RIGHT_CENTER].x = (point[RIGHT_UP].x + point[MIDDLE_UP].x + point[RIGHT_DOWN].x + point[MIDDLE_DOWN].x)/4;
	point[RIGHT_CENTER].y = (point[RIGHT_UP].y + point[MIDDLE_UP].y + point[RIGHT_DOWN].y + point[MIDDLE_DOWN].y)/4;

	x_left = formal_x_left;
	x_right = formal_x_right;
	y_up = formal_y_up;
	y_down = formal_y_down;
}


