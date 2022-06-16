//Team.cpp
#include "Team.h"
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <math.h>
using namespace std;
Team::Team()
{
	p_worker = new Worker[MAX_TEAMMATE];
}
Team::~Team()
{
	//delete[] p_worker;
}
void Team::Reset()
{
	for(int memberID = 0; memberID < MAX_TEAMMATE; memberID++){
		p_worker[memberID].Reset();
	}
}
void Team::PerformAction(int mode, int action_set[], const int action_count, unsigned episode_id, const int step)
{
	for(int memberID = 0; memberID < MAX_TEAMMATE; memberID++){
		int state_index;
		state_index = p_worker[memberID].GetStateIndex();
		int action = (p_worker[memberID].p_strategy)->GetAction(state_index, mode, action_set, action_count, episode_id, step); 
		p_worker[memberID].SetAction(action);
	}
}
double Team::GetAllInsReward()
{
	all_ins_reward = 0.0;
	for(int memberID = 0; memberID < MAX_TEAMMATE; memberID++){
		all_ins_reward += p_worker[memberID].GetInsReward();
	}
	return all_ins_reward;
}
void Team::SetAllInsReward(double formal_all_ins_reward)
{
	all_ins_reward = formal_all_ins_reward;
}
double Team::GetAllAccReward()
{
	all_acc_reward = 0.0;
	for(int memberID = 0; memberID < MAX_TEAMMATE; memberID++){
		all_acc_reward += p_worker[memberID].GetAccReward();
	}
	return all_acc_reward;
}
void Team::SetAllAccReward(double formal_all_acc_reward)
{
	all_acc_reward = formal_all_acc_reward;
}
void Team::Print() const
{
	cout<<"Team->Print"<<endl;
	system("pause");
}
