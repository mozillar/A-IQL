#include "A_IQL.h"
#include<stdlib.h>
#include<fstream>
#include <iostream>
using namespace std;
A_IQL::A_IQL():T(8), 
		 discount_factor(0.9),
		 INI_EPSILON(0.6), // 0.6
		 END_EPSILON(0.2), // 0.2
		 INI_THRESH(2.0),
		 END_THRESH(0.01)
{
	;
}
A_IQL::~A_IQL()
{
	
}
/*-----------------Each agent selects an aciton at state state_index.-----------------*/
int A_IQL::GetAction(int state_index, int mode, int action_set[], const int action_count, unsigned episode_id, const int step)
{
	int selected_action;
	if(mode == Strategy::LEARN){ 
		double p_agent_action[MAX_ACTION_COUNT] = {0,0,0,0,0}; 
		for(int actionID = 0; actionID < action_count; actionID++){		
			int action = action_set[actionID];
			p_agent_action[actionID] = pi[state_index][action];
		}
		double binaryPoint_pro[MAX_ACTION_COUNT+1] = {0,0,0,0,0,0};  
		for(int binaryPointID = 1; binaryPointID <= action_count-1; binaryPointID++){
			binaryPoint_pro[binaryPointID] = p_agent_action[binaryPointID-1] + binaryPoint_pro[binaryPointID -1] ;
		}
		binaryPoint_pro[action_count] = 1.0;
		double randNum;
		randNum = GetRand();  
		for(int actionID = 0; actionID < action_count; actionID++){
			if ((binaryPoint_pro[actionID] <= randNum ) && (randNum <= binaryPoint_pro[actionID+1])){
				selected_action = action_set[actionID];
				break;
			}			
		}
		visit[state_index][selected_action] = visit[state_index][selected_action] + 1; 
	}
	else{
		// Select the action with the highest probability during the evaluation stage.
		double max_pi = NEG_INF;
		for(int actionID = 0; actionID < action_count; actionID++){
			if(pi[state_index][action_set[actionID]] > max_pi){
				max_pi = pi[state_index][action_set[actionID]];
				selected_action = action_set[actionID];
			}
		}
	}
	return  selected_action;
}
/*------------Initialization for A-IQL------------*/
void A_IQL::InitializeLearning(int action_set[], const int action_count)
{
	SetEpsilon(INI_EPSILON);
	double ACT_COUNT = action_count;
	thresh = INI_THRESH;
	for(int stateID = 0; stateID < MAX_STATE_COUNT; stateID++){
		for(int actionID = 0; actionID < ACT_COUNT; actionID++){
			Q[stateID][action_set[actionID]] = 0.0;
			visit[stateID][action_set[actionID]] = 0;
			maxQ[stateID][action_set[actionID]] = NEG_INF;
			pi[stateID][action_set[actionID]] = 1.0/ACT_COUNT;	  			
		}
	}
}
double A_IQL::GetEpsilon() const
{
	return epsilon;
}
void A_IQL::SetEpsilon(double formal_epsilon)
{
	epsilon = formal_epsilon;
}
/*-----------------Update related parameters of A-IQL.-----------------*/
void A_IQL::UpdateAlgorithmParameter(const unsigned episode_id)
{
	epsilon = INI_EPSILON - (INI_EPSILON - END_EPSILON)/MAX_LEARNING_COUNT * episode_id;
	thresh = INI_THRESH - (INI_THRESH - END_THRESH)/MAX_LEARNING_COUNT * episode_id;
}
/*-----------------Update Q-functin Q(s,ai) and pi(ai|s)for each agent i.-----------------*/
void A_IQL::UpdateStrategyAfterEachStep(const int pre_state_index, const int cur_state_index, const int state_index, const int action, const double reward, \
										 int pre_action_set[], const int & pre_action_count, int cur_action_set[], const int & cur_action_count, unsigned episode_id, const int step)   //¸üÐÂ²ßÂÔ
{
	int v;
	v = visit[pre_state_index][action];
	double max_Q = NEG_INF;
	for(int actionID = 0; actionID < cur_action_count; actionID++){
		if(Q[cur_state_index][cur_action_set[actionID]] > max_Q){
			max_Q = Q[cur_state_index][cur_action_set[actionID]];
		}
	}
	double sample;
	sample = reward + discount_factor * max_Q;
	if(maxQ[pre_state_index][action] < sample){
		maxQ[pre_state_index][action] = sample;
	}

	if(v >= T){
		Q[pre_state_index][action] = maxQ[pre_state_index][action];
		visit[pre_state_index][action] = 0; 
		maxQ[pre_state_index][action] = NEG_INF; 
	} 

	double pre_max_Q = NEG_INF;
	for(int actionID = 0; actionID < pre_action_count; actionID++){
		if(Q[pre_state_index][pre_action_set[actionID]] > pre_max_Q){
			pre_max_Q = Q[pre_state_index][pre_action_set[actionID]];
		}
	}

	if(abs(sample - pre_max_Q) <= thresh){
		double ACT_COUNT = pre_action_count;
		for(int actionID = 0; actionID < pre_action_count; actionID++){
			if( pre_action_set[actionID] == action){
				pi[pre_state_index][pre_action_set[actionID]] = 1 - epsilon;
			}
			else{
				pi[pre_state_index][pre_action_set[actionID]] = epsilon/(ACT_COUNT-1.0);
			}
		}
	}
}
/*------------null operation for A-IQL------------*/
void A_IQL::UpdateStrategyAfterEachEpisode(const int step, int action_set[], int action_count)
{
}
/*-----------------Return a random number that obeys U[0,1].-----------------*/
double A_IQL::GetRand()
{
	double temp_randMin,temp_randMax,temp_randNumber;
    temp_randMin = 0.0;
    temp_randMax = 1.0;
    temp_randNumber = (double)rand()/RAND_MAX * (temp_randMax-temp_randMin) + temp_randMin;
	return temp_randNumber;
}
void A_IQL::SaveStrategy()
{
}