#ifndef TEST_H
#define TEST_H
#include <string>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include "Game.h"

using namespace std;
class Test{ 

public:
	//function members
	Test();
	void PrintTestInformationMap(Game* p_game, Team* p_team[], Box* p_box, MovingObject* p_movingObject[], const string& str_file, const string& op, const int step);
private:
	void PrintInformation(ofstream& file, Game* p_game, Team* p_team[], MovingObject* p_movingObject[], const int step); //ofstream 
	void PrintMap(ofstream& file, Game* p_game, MovingObject* p_movingObject[], Box* p_box); 
};
#endif