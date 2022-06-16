#ifndef GAME_H
#define GAME_H
#include "Team.h"
#include "MovingObject.h"
#include "Box.h"
#include "Barrier.h"
#include "Boundary.h"
#include "Home.h"
#include <string>
using namespace std;
// Game defines the rules of the task. 
const int MAX_TEAM = 1;
class Game {

public:
	//function members
	Game();
	void InitializeLearning(Team* p_team[]);
	void GetActionSet(Team* p_team[], const int teamID, int action_set[], int & action_count);
	void Reset(Team* p_team[], MovingObject* p_movingObject[], Box* p_box);
	void Update(Team* p_team[], MovingObject* p_movingObject[], Box* p_box, Barrier* p_barrier[], Home* p_home, Boundary* p_boundary, const int step, const int episode_id, bool& is_success);
	//bool GetIsWorkerCollided(MovingObject* p_movingObject[], int objectID, Box* p_box, Barrier* p_barrier, Boundary* p_boundary);
	int GetStateIndexForOneTeam(Team* p_team[], const int teamID, MovingObject* p_movingObject[]);
	void SetStateIndexForOneTeam(Team* p_team[], const int teamID, int state_index);
	bool IsGoing(Team* p_team[], const int step);
    void UpdateStrategyAfterEachStep(Team* p_team[], int pre_action_set[][MAX_ACTION_COUNT], int pre_action_count[], int cur_action_set[][MAX_ACTION_COUNT], int cur_action_count[], unsigned episode_id, const int step);
	void UpdateStrategyAfterEachEpisode(Team* p_team[], const int step);
	void UpdateAlgorithmParameter(Team* p_team[], unsigned episode_id);
	void PerformAction(Team* p_team[], int mode, unsigned episode_id, const int step); 
	bool GetIsGoing();
	void SetIsGoing(bool formal_is_going);
	void SetPreStateIndex(Team* p_team[]);
	void SetCurStateIndex(Team* p_team[], MovingObject* p_movingObject[]);

	double GetRand(); 
	
    static const int REWARD_STEP = -1;         // immediate reward after each step
	static const int REWARD_HOME = 100;        // not used
	static const int REWARD_HOME_BIG = 100;    // When episode_id < L/2, entering the home zone from the left is rewarded with 100, and entering the home zone from the right is rewarded with 50.
	static const int REWARD_HOME_SMALL = 50;   // When episode_id < L/2, entering the home zone from the right is rewarded with 100, and entering the home zone from the left is rewarded with 50.
    static const int REWARD_GRASP = 1 ;        // Grasping the box is rewarded with 1.
	//data_member
private:
	//data members
	bool is_going;
	void CopyMovingObject(MovingObject* p_originMovingObject, MovingObject* p_copyMovingObject); 
	void CopyBox(Box* p_originBox, Box* p_copyBox);  
	bool GetIsAtHome(Box* p_box, Home* p_home);
	void UpdatePos(MovingObject* p_movingObject, int action);
	bool GetIsWorkerCollided(MovingObject* p_movingObject[], int objectID, Box* p_box, Barrier* p_barrier[], Boundary* p_boundary);
	bool GetTwoCollided(MovingObject* p_movingObject, Barrier* p_barrier);
	bool GetTwoCollided(MovingObject* p_movingObject1, MovingObject* p_movingObject2);
	bool GetOutBoundary(MovingObject* p_movingObject, Boundary* p_boundary);
	bool GetTwoCollided(MovingObject* p_movingObject, Box* p_box);
	bool UpdateGrasp(MovingObject* p_movingObject, Box* p_box, Team* p_team, const int teammateID);
	bool UpdatePos(MovingObject* p_movingObject[], Box* p_box, Team* p_team);
	bool GetIsCollided(MovingObject* p_movingObject, Barrier* p_barrier[], Boundary* p_boundary);
	bool Game::GetIsWorkerCollided(MovingObject* p_movingObject[], int objectID);
	void Game::GetAllCollided(MovingObject* p_movingObject[], MovingObject* p_temp_movingObject[], bool isCollided[]);
};
#endif