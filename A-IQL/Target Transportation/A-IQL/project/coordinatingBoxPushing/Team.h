#ifndef TEAM_H
#define TEAM_H
#include <string>
#include "Worker.h"
#include "MovingObject.h"
#include "GlobalVariable.h"
using namespace std;

class Team {

public:
	//function members
	Team();
	~Team();
	
	double GetAllInsReward();
	void SetAllInsReward(double formal_all_ins_reward);
	double GetAllAccReward();
    void SetAllAccReward(double formal_all_acc_reward);
	void Reset();
	void PerformAction(int mode, int action_set[], const int action_count, unsigned episode_id, const int step); 
    void Print() const;

	//data_member
	Worker* p_worker;

private:
	//data members
	string team_name;                   
	double all_ins_reward;      
	double all_acc_reward;		
};
#endif