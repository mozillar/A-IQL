#ifndef STATE_H
#define STATE_H
struct State{  //public inherit: Subclasses cannot inherit private members of their parent class, but they can access private members of their parent class through public functions
	double x1; // the x of the center point of robot 1
	double y1; // the y of the center point of robot 1
	double x2; // the x of the center point of robot 2
	double y2; // the y of the center point of robot 2
	int grasp1;// the grasp information of robot 1
	int grasp2;// the grasp information of robot 2
};
#endif