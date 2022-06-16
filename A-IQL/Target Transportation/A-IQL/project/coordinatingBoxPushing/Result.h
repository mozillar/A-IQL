#ifndef RESULT_H
#define RESULT_H
#include <string>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include "Game.h"
using namespace std;
class Result{ 

public:
	//function members
	Result();
	void SaveData(double a, const string& str_file, const string& op);
    void SaveData(double my_array[], int cout, const string& str_file, const string& op);
	void UpdateAllWinRate();
	void UpdateAllWinStep();
	void UpdateAllStatistics();
	void SaveResult();

private:
	//reserve
	void Initialize();
	void CreateFolder(string folder_name);
	void SaveData(const string& sr_folder, const string& str_mode, const string& str_team, const string& str_file, double my_array[][MAX_TEAM][MAX_RUN_COUNT], const int dim1, const int dim2, const int cout, const string& op);
	void SaveData(const string& sr_folder, const string& str_mode, const string& str_team, const string& str_file, double my_array[][MAX_TEAM], const int dim1, const int dim2, const string& op);


	//data member
	double win_count[MODE_COUNT][MAX_TEAM][MAX_RUN_COUNT];
	double draw_count[MODE_COUNT][MAX_TEAM][MAX_RUN_COUNT];
	double lose_count[MODE_COUNT][MAX_TEAM][MAX_RUN_COUNT];
	double win_step[MODE_COUNT][MAX_TEAM][MAX_RUN_COUNT];
	double win_rate[MODE_COUNT][MAX_TEAM][MAX_RUN_COUNT];
	double all_win_rate[MODE_COUNT][MAX_TEAM];
	double all_win_step[MODE_COUNT][MAX_TEAM];

};
#endif