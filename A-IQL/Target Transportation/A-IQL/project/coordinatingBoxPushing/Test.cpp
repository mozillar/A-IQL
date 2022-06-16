#include "Test.h"
#include <math.h>
/*Print the trajectory of the robots in the map for one episode.*/
Test::Test()
{
}
void Test::PrintTestInformationMap(Game* p_game, Team* p_team[], Box* p_box, MovingObject* p_movingObject[], const string& str_file, const string& op, const int step)
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
	PrintInformation(file, p_game, p_team, p_movingObject, step);
	PrintMap(file, p_game, p_movingObject, p_box);
	
	file.close();
}
void Test::PrintInformation(ofstream& file, Game* p_game, Team* p_team[], MovingObject* p_movingObject[], const int step)
{
	file<<"step: "<<step<<endl;
	file<<"state_index: "<<p_game->GetStateIndexForOneTeam(p_team, TEAM1, p_movingObject)<<endl;
	file<<endl;
	for(int objectID = 0; objectID < MAX_MO_COUNT; objectID++){ 
		file<<"movingObject "<<objectID;
		if(p_movingObject[objectID]->GetIsActive() == true){
			file<<"  Active"<<endl;
		}
		else{
			file<<"  Inactive"<<endl;
		}
	}
	//if(step != 1){
		string str_action[MAX_ACTION_COUNT] = {"STAY","UP","DOWN","LEFT","RIGHT"};
		for(int teamID = 0; teamID < MAX_TEAM;teamID++){
			for(int teammateID = 0; teammateID < MAX_TEAMMATE; teammateID++){
				int action = (p_team[teamID]->p_worker[teammateID]).GetAction();
				if(action < MAX_ACTION_COUNT && action >= 0){
					file<<str_action[action]<<"  "<<endl;
				}
				else{
					file<<action<<"  "<<endl;
				}			
				//file<<endl;
			}
		}
		string str_grasp[3] ={"free", "left_grasp","right_grasp"}; 
		for(int teamID = 0; teamID < MAX_TEAM;teamID++){
			for(int teammateID = 0; teammateID < MAX_TEAMMATE; teammateID++){
				file<<str_grasp[(p_team[teamID]->p_worker[teammateID]).GetGrasp()]<<endl;			
			}
		}
	//}
	for(int teamID = 0; teamID < MAX_TEAM;teamID++){
		for(int teammateID = 0; teammateID < MAX_TEAMMATE; teammateID++){
			file<<"InsReward: "<<(p_team[teamID]->p_worker[teammateID]).GetInsReward()<<"  "<<endl;
			file<<"AccReward: "<<(p_team[teamID]->p_worker[teammateID]).GetAccReward()<<"  "<<endl;
			//file<<endl;
		}
	}
	//file<<endl;
	file<<"AllInsReward: "<<p_team[TEAM1]->GetAllInsReward()<<"  ";
	file<<"AllAccReward: "<<p_team[TEAM1]->GetAllAccReward()<<"  "<<endl;
	//file<<endl;
}
void Test::PrintMap(ofstream& file, Game* p_game, MovingObject* p_movingObject[], Box* p_box)
{
	const int int_null = 0;
	const int int_home = 1;
	const int int_barrier = 2;
	const int int_box = 3;
	const int int_worker1 = 4;
	const int int_worker2 = 5;
	int map[WIDTH][HEIGHT];
	// blank area
	for(int x = 0; x < WIDTH; x++){
		for(int y = 0; y < HEIGHT; y++){
			map[x][y] = int_null;
		}
	}
	// home zone
	map[3][0] = int_home;
	map[4][0] = int_home;
	map[5][0] = int_home;
	map[3][1] = int_home;
	map[4][1] = int_home;
	map[5][1] = int_home;
	//barrier
	map[4][2] = int_barrier;

	map[0][6] = int_barrier;
	map[1][6] = int_barrier;

	map[3][6] = int_barrier;
	map[4][6] = int_barrier;
	map[5][6] = int_barrier;
	map[6][6] = int_barrier;
	map[7][6] = int_barrier;
	map[8][6] = int_barrier;

	map[8][7] = int_barrier;
	map[8][8] = int_barrier;

	int x, y;
	x = floor(p_movingObject[WORKER1]->point[CENTER].x);
	y = floor(p_movingObject[WORKER1]->point[CENTER].y);
	map[x][y] = int_worker1;
	x = floor(p_movingObject[WORKER2]->point[CENTER].x);
	y = floor(p_movingObject[WORKER2]->point[CENTER].y);
	map[x][y] = int_worker2;
	x = (int)(floor(p_box->point[CENTER].x));
	y = (int)(floor(p_box->point[CENTER].y));
	map[x][y] = int_box;

	string str_map[7] = {"-","H","X","B","1","2"};

	for(int y = 0; y < HEIGHT; y++){
		for(int x = 0; x < WIDTH; x++){
			file<<str_map[map[x][y]]<<" ";
		}
		file<<endl;
	}
	file<<endl;
}

