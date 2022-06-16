#ifndef A_IQL_H
#define A_IQL_H
#include "Strategy.h"
#include "GlobalVariable.h"
#include <math.h>
class A_IQL : public Strategy{

public:
	A_IQL();
	~A_IQL();
	virtual void InitializeLearning(int action_set[], const int action_count);
	virtual int GetAction(int state_index, int mode, int action_set[], const int action_count, unsigned episode_id, const int step); 
	virtual void UpdateAlgorithmParameter(const unsigned episode_id);
	virtual void UpdateStrategyAfterEachStep(const int pre_state_index, const int cur_state_index, const int state_index, const int action, const double reward,\
		int pre_action_set[], const int & pre_action_count, int cur_action_set[], const int & cur_action_count, unsigned episode_id, const int step);   
	virtual void UpdateStrategyAfterEachEpisode(const int step, int action_set[], int action_count);
	virtual void SaveStrategy();
	double GetEpsilon() const;
	void SetEpsilon(double formal_epsilon);
	
private:
	/*---强化学习参数---*/
	const int T;                                                // T in the paper
	const double INI_EPSILON;                                   // the initial value of epsilon 
	const double END_EPSILON;                                   // the final value of epsilon 
	const double INI_THRESH;
	const double END_THRESH;
	const double discount_factor;                               // discount factor
	double epsilon;
	double thresh;
	double Q[MAX_STATE_COUNT][MAX_ACTION_COUNT];                // the Q-funciton Q(s|ai) for each agent ai
	int visit[MAX_STATE_COUNT][MAX_ACTION_COUNT];
	double maxQ[MAX_STATE_COUNT][MAX_ACTION_COUNT];
	double pi[MAX_STATE_COUNT][MAX_ACTION_COUNT];               // the strategy pi(s|ai) for each agent ai
	double GetRand();
};
#endif