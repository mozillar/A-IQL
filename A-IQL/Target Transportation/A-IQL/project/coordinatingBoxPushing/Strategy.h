#ifndef  STRATEGY_H
#define STRATEGY_H

class Strategy{

public:
	Strategy();
	int GetStateCount();
	void SetStateCount(int formal_state_count);
	int GetActionCount();
	void SetActionCount(int formal_action_count);       

	virtual int GetAction(int state_index, int mode, int action_set[], const int action_count, unsigned episode_id, const int step) = 0; 
	virtual void UpdateStrategyAfterEachStep(const int pre_state_index, const int cur_state_index, const int state_index, const int action, const double reward, \
		int pre_action_set[], const int & pre_action_count, int cur_action_set[], const int & cur_action_count, unsigned episode_id, const int step) = 0;   
	virtual void UpdateStrategyAfterEachEpisode(const int step, int action_set[], int action_count) = 0;
	virtual void InitializeLearning(int action_set[], const int action_count) = 0; 
	virtual void UpdateAlgorithmParameter(const unsigned episode_id) = 0; 
	virtual void SaveStrategy() = 0;
    
	
	static const int LEARN = 0;
	static const int EVALUATE = 1;
private:
	int state_count;
	int action_count;
};
#endif