#include "Result.h"
Result::Result()
{
	Initialize();
}
void Result::SaveData(double a, const string& str_file, const string& op)
{
	const char * c_file = str_file.c_str();
	ofstream file;
	if(op == "app"){
		file.open(c_file, ios::app);
	}
	else if(op == "out"){
		file.open(c_file, ios::out);
	}
	else{
		;
	}
	if(!file){
		cerr<<"SaveData error File could not be opened"<<endl;
	}
	file<<a<<endl;
	file.close();
}
void Result::SaveData(double my_array[], int cout, const string& str_file, const string& op)
{
	const char * c_file = str_file.c_str();
	ofstream file;
	if(op == "app"){
		file.open(c_file, ios::app);
	}
	else if(op == "out"){
		file.open(c_file, ios::out);
	}
	else{
		;
	}
	if(!file){
		cerr<<"SaveData error File could not be opened"<<endl;
	}
	int id;
	for(id = 0; id < cout; id++){
		file<<my_array[id]<<endl;
	}
	file.close();
}
void Result::Initialize()
{
	for(int mode = 0; mode < MODE_COUNT; mode++){
		for(int teamID = 0; teamID < MAX_TEAM; teamID++){
			all_win_rate[mode][teamID] = 0.0;
			all_win_step[mode][teamID] = 0.0;
			for(int runID = 0; runID < MAX_RUN_COUNT; runID++){
				win_count[mode][teamID][runID] = 0.0;
				draw_count[mode][teamID][runID] = 0.0;
				lose_count[mode][teamID][runID] = 0.0;
				win_step[mode][teamID][runID] = 0.0;
				win_rate[mode][teamID][runID] = 0.0;
			}
		}
	}
	
}
void Result::SaveResult()
{
	string str_folder = "Result";
	CreateFolder(str_folder);
    string str_mode;
	string str_team;
	for(int mode = 0; mode < MODE_COUNT; mode++){
		if(mode == Strategy::LEARN){
			str_mode = "/learn_";
		}
		else{
			str_mode = "/evaluate_";
		}
		for(int teamID = 0; teamID < MAX_TEAM; teamID++){
			if(teamID == TEAM1){
				str_team = "team1_";
			}
			else{
				str_team = "team2_";
			}
			SaveData(str_folder, str_mode, str_team, "win_count.dat",    win_count,    mode, teamID, MAX_RUN_COUNT,"app" );
			SaveData(str_folder, str_mode, str_team, "draw_count.dat",   draw_count,   mode, teamID, MAX_RUN_COUNT,"app" );
			SaveData(str_folder, str_mode, str_team, "lose_count.dat",   lose_count,   mode, teamID, MAX_RUN_COUNT,"app" );
			SaveData(str_folder, str_mode, str_team, "win_step.dat",     win_step,     mode, teamID, MAX_RUN_COUNT,"app" );
			SaveData(str_folder, str_mode, str_team, "win_rate.dat", win_rate, mode, teamID, MAX_RUN_COUNT,"app" );
			SaveData(str_folder, str_mode, str_team, "all_win_rate.dat", all_win_rate, mode, teamID, "out" );
			SaveData(str_folder, str_mode, str_team, "all_win_step.dat", all_win_step, mode, teamID, "out" );
		}
	}
}
void Result::SaveData(const string& sr_folder, const string& str_mode, const string& str_team, const string& str_file, double my_array[][MAX_TEAM][MAX_RUN_COUNT], const int dim1, const int dim2, const int cout, const string& op)
{
	string str_path = sr_folder + str_mode + str_team + str_file;
	double temp[MAX_RUN_COUNT];
	for(int runID = 0; runID < MAX_RUN_COUNT; runID++){
		temp[runID] = my_array[dim1][dim2][runID];
	}
	SaveData(temp, MAX_RUN_COUNT, str_path, op);
}
void Result::SaveData(const string& sr_folder, const string& str_mode, const string& str_team, const string& str_file, double my_array[][MAX_TEAM], const int dim1, const int dim2, const string& op)
{
	string str_path = sr_folder + str_mode + str_team + str_file;
	double temp = my_array[dim1][dim2];
	SaveData(temp, str_path, op);
}
void Result::CreateFolder(string folder_name)
{
    string command;
    command = "mkdir " + folder_name;  
    system(command.c_str());
}
void Result::UpdateAllStatistics()
{
	UpdateAllWinRate();
	UpdateAllWinStep();
}
void Result::UpdateAllWinRate()  
{
	for(int mode = 0; mode < MODE_COUNT; mode++){
		for(int teamID = 0; teamID < MAX_TEAM; teamID++){
			all_win_rate[mode][teamID] = 0.0;
			for(int runID = 0; runID < MAX_RUN_COUNT; runID++){
				all_win_rate[mode][teamID] += win_rate[mode][teamID][runID];
			}
			all_win_rate[mode][teamID] = all_win_rate[mode][teamID] / MAX_RUN_COUNT;
		}
	}
}
void Result::UpdateAllWinStep()  
{
	for(int mode = 0; mode < MODE_COUNT; mode++){
		for(int teamID = 0; teamID < MAX_TEAM; teamID++){
			all_win_step[mode][teamID] = 0.0;
			double all_win_count = 0.0;
			for(int runID = 0; runID < MAX_RUN_COUNT; runID++){
				all_win_step[mode][teamID] += win_step[mode][teamID][runID] * win_count[mode][teamID][runID];
				all_win_count += win_count[mode][teamID][runID];
			}
			if(all_win_count == 0){
				cout<<"warning: void Result::UpdateAllWinStep() all_win_count = 0"<<endl;
				system("pause");
			}
			all_win_step[mode][teamID] = all_win_step[mode][teamID] / all_win_count;
		}
	}
}
