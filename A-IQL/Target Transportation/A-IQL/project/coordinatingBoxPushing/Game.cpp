//Soldier.cpp
#include "Game.h"

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <math.h>
using namespace std;
Game::Game()
{
	;
}
void Game::GetActionSet(Team* p_team[], const int teamID, int action_set[], int & action_count)
{
	action_count = MAX_ACTION_COUNT;
	action_set[STAY] = STAY;
	action_set[UP] = UP;
	action_set[DOWN] = DOWN;
	action_set[LEFT] = LEFT;
	action_set[RIGHT] = RIGHT;
}
bool Game::GetIsGoing()
{
	return is_going;
}
void Game::SetIsGoing(bool formal_is_going)
{
	is_going = formal_is_going;
}
/*-----------Restart an episode-----------*/
void Game::Reset(Team* p_team[], MovingObject* p_movingObject[], Box* p_box)
{
	for(int teamID = 0; teamID < MAX_TEAM; teamID++){
		p_team[teamID]->Reset();
	}
	for(int objectID = 0; objectID < MAX_MO_COUNT; objectID++){
		p_movingObject[objectID]->Reset();
	}
	p_movingObject[COMBINATION]->SetIsActive(false); 
	p_box->Reset();
	SetIsGoing(true);
}
/*-----------Update the status of each unit and return the reward-----------*/
void Game::Update(Team* p_team[], MovingObject* p_movingObject[], Box* p_box, Barrier* p_barrier[], Home* p_home, Boundary* p_boundary, const int step, const int episode_id, bool& is_success)
{
	for(int teamID = 0; teamID < MAX_TEAM; teamID++){
		MovingObject* p_temp_movingObject[MAX_MO_COUNT];
		MovingObject temp_object0(-1,-1,-1,-1); 
		MovingObject temp_object1(-1,-1,-1,-1);
		MovingObject temp_object2(-1,-1,-1,-1);
	   
		p_temp_movingObject[0] = &temp_object0;
		p_temp_movingObject[1] = &temp_object1;
		p_temp_movingObject[2] = &temp_object2;

		for(int objectID = 0; objectID < MAX_MO_COUNT; objectID++){
			CopyMovingObject(p_movingObject[objectID], p_temp_movingObject[objectID]); 
		}

		Box temp_box(4,5,3.33,3.66);
		Box* p_temp_box = &temp_box;
		CopyBox(p_box, p_temp_box); 

		
		if((p_temp_movingObject[WORKER1]->GetIsActive() == true || p_temp_movingObject[WORKER2]->GetIsActive() == true) && p_temp_movingObject[COMBINATION]->GetIsActive() == false){// Only one robot has grasped the box. 
			for(int objectID = 0; objectID < MAX_MO_COUNT-1; objectID++){ 
				if(p_temp_movingObject[objectID]->GetIsActive() == true){
					UpdatePos(p_temp_movingObject[objectID], (p_team[teamID]->p_worker[objectID]).GetAction());
				}
			}
			for(int objectID = 0; objectID < MAX_MO_COUNT-1; objectID++){ 
				if(p_temp_movingObject[objectID]->GetIsActive() == true){
					bool isCollided = GetIsWorkerCollided(p_temp_movingObject, objectID, p_box, p_barrier, p_boundary);
					if(isCollided){
						CopyMovingObject(p_movingObject[objectID], p_temp_movingObject[objectID]);
					}	
				}
			}
			bool isCollided[MAX_MO_COUNT-1];
			GetAllCollided(p_movingObject, p_temp_movingObject, isCollided);
			for(int objectID = 0; objectID < MAX_MO_COUNT-1; objectID++){ 
				if(p_temp_movingObject[objectID]->GetIsActive() == true){  // The robot has grasped the box.
					if(!isCollided[objectID]){  // no collision (The robot moves successfully. )
						bool isGrasp = UpdateGrasp(p_temp_movingObject[objectID], p_box, p_team[teamID], objectID); // Update grasp information
						if(isGrasp){
							(p_team[teamID]->p_worker[objectID]).SetInsReward(REWARD_GRASP);
							p_temp_movingObject[objectID]->SetIsActive(false); 
						}
						else{
							(p_team[teamID]->p_worker[objectID]).SetInsReward(REWARD_STEP);
						}
					}
					else{
						(p_team[teamID]->p_worker[objectID]).SetInsReward(REWARD_STEP); 
					}			
				}
				else{ // The robot has not grasped the box.
					(p_team[teamID]->p_worker[objectID]).SetInsReward(REWARD_STEP); 
				}
			}
			for(int objectID = 0; objectID < MAX_MO_COUNT-1; objectID++){
				CopyMovingObject(p_temp_movingObject[objectID], p_movingObject[objectID]); 
			}
			//If both robots have grasped the box, activate the combination.
			if(p_movingObject[WORKER1]->GetIsActive() == false && p_movingObject[WORKER2]->GetIsActive() == false){
				p_movingObject[COMBINATION]->SetIsActive(true);
			}
		}
		else if(p_movingObject[WORKER1]->GetIsActive() == false && p_movingObject[WORKER2]->GetIsActive() == false && p_movingObject[COMBINATION]->GetIsActive() == true){// Both robots have grasped the box.
			bool is_pos_changed = UpdatePos(p_temp_movingObject, p_temp_box, p_team[teamID]);//Update the position of all Movingobjects and the position of the box, if agent is moving in the same direction, seto is_pos_changed to true .
			if(is_pos_changed){
				bool isCollided = false;
				for(int objectID = 0; objectID < MAX_MO_COUNT; objectID++){
					isCollided = GetIsCollided(p_temp_movingObject[objectID], p_barrier, p_boundary); 
					if(isCollided){   // As soon as one part of the combination collides, the action fails.
						break;
					}
				}		
				if(!isCollided){ 
					// Update the position of all Movingobjects.
					for(int objectID = 0; objectID < MAX_MO_COUNT; objectID++){ 
						CopyMovingObject(p_temp_movingObject[objectID], p_movingObject[objectID]);
					}	
					CopyBox(p_temp_box, p_box);
					bool isAtHome = GetIsAtHome(p_box, p_home); 
					if(isAtHome){
						SetIsGoing(false);
						float reward_home = -100;
						if(episode_id < MAX_LEARNING_COUNT/2){ 
							if(p_box->point[CENTER].x <= 4.0 && p_box->point[CENTER].x >= 3.0){         // When episode_id < L/2, entering the home zone from the left is rewarded with 100.
								reward_home = REWARD_HOME_BIG;
							}
							else if(p_box->point[CENTER].x <= 6.0 && p_box->point[CENTER].x >= 5.0){    // When episode_id < L/2, entering the home zone from the right is rewarded with 50.
								reward_home = REWARD_HOME_SMALL;
							}
							else{
								;
							}
						}
						else{  
							if(p_box->point[CENTER].x <= 4.0 && p_box->point[CENTER].x >= 3.0){         // When episode_id >= L/2, entering the home zone from the left is rewarded with 50.
								reward_home = REWARD_HOME_SMALL;
							}
							else if(p_box->point[CENTER].x <= 6.0 && p_box->point[CENTER].x >= 5.0){    // When episode_id >= L/2, entering the home zone from the right is rewarded with 100.
								reward_home = REWARD_HOME_BIG;
								is_success = true;
							}
							else{
								;
							}
						}
						// Movement succeeded and reached home.
						for(int teammateID = 0; teammateID < MAX_TEAMMATE; teammateID++){
							(p_team[teamID]->p_worker[teammateID]).SetInsReward(reward_home);
						}
					}
					else{ // Movement succeeded but did not reach home.
						SetIsGoing(true);
						for(int teammateID = 0; teammateID < MAX_TEAMMATE; teammateID++){
							(p_team[teamID]->p_worker[teammateID]).SetInsReward(REWARD_STEP);
						}
					}
				}
				else{  // Movement failed and did not reach home.
					for(int teammateID = 0; teammateID < MAX_TEAMMATE; teammateID++){
						(p_team[teamID]->p_worker[teammateID]).SetInsReward(REWARD_STEP);
					}
				}	
			}
			else{  //If there is no change in position, an immediate reward of -1 is given.   (Movement failed and did not reach home)
				for(int teammateID = 0; teammateID < MAX_TEAMMATE; teammateID++){
					(p_team[teamID]->p_worker[teammateID]).SetInsReward(REWARD_STEP);
				}
			}
		}
		else{
			cout<<"error: Game::Update";
		}		
		/*Update reward for each agent/robot.*/
		for(int teammateID = 0; teammateID < MAX_TEAMMATE; teammateID++){
			(p_team[teamID]->p_worker[teammateID]).SetAccReward((p_team[teamID]->p_worker[teammateID]).GetInsReward() + (p_team[teamID]->p_worker[teammateID]).GetAccReward());
		}
		p_team[teamID]->SetAllInsReward(p_team[teamID]->GetAllInsReward());
		p_team[teamID]->SetAllAccReward(p_team[teamID]->GetAllAccReward());
	}
}
/*-----------Return true if the box is in the home zone.-----------*/
bool Game::GetIsAtHome(Box* p_box, Home* p_home)
{
	bool is_home = true;
	for(int pointID = 0; pointID < POINT_COUNT; pointID++){
		if(p_box->point[pointID].x <= p_home->x_right && p_box->point[pointID].x >= p_home->x_left && p_box->point[pointID].y <= p_home->y_down && p_box->point[pointID].y >= p_home->y_up){
			is_home = true;	
		}
		else{
			is_home = false;
			return is_home;
		}
	}
	return is_home;
}
void Game::UpdatePos(MovingObject* p_movingObject, int action)
{
	if(action != STAY){
		double s = UNIT_DIS + GetRand()*2*RAND_DIS - RAND_DIS;
		double x_left = p_movingObject->x_left;
		double x_right = p_movingObject->x_right;
		double y_up = p_movingObject->y_up;
		double y_down = p_movingObject->y_down;
		if(action == LEFT){
			x_left -= s;
			x_right -= s;
		}
		else if(action == RIGHT){
			x_left += s;
			x_right += s;
		}
		else if(action == UP){
			y_up -= s;
			y_down -= s;
		}
		else if(action == DOWN){
			y_up += s;
			y_down += s;
		}
		else{
			;
		}
		p_movingObject->SetPos(x_left, x_right, y_up, y_down);
	}
}
bool Game::GetIsWorkerCollided(MovingObject* p_movingObject[], int objectID, Box* p_box, Barrier* p_barrier[], Boundary* p_boundary)
{
	bool isCollided;
	for(int barID = 0; barID < MAX_BAR_COUNT; barID++){
		isCollided = GetTwoCollided(p_movingObject[objectID], p_barrier[barID]);
		if(isCollided){
			return isCollided;
		}
	}
    isCollided = GetOutBoundary(p_movingObject[objectID], p_boundary);
	if(isCollided){ 
		return isCollided;
	}
	isCollided = GetTwoCollided(p_movingObject[objectID], p_box);
	if(isCollided){
		return isCollided;
	}
	return isCollided;    
}
bool Game::GetIsWorkerCollided(MovingObject* p_movingObject[], int objectID)
{
	bool isCollided;
	for(int t_objectID = 0; t_objectID < MAX_MO_COUNT-1; t_objectID++){
		if(t_objectID != objectID){
			isCollided = GetTwoCollided(p_movingObject[objectID], p_movingObject[t_objectID]);
			if(isCollided){ 
				return isCollided;
			}
		}
	}
	return isCollided;    
}
bool Game::GetTwoCollided(MovingObject* p_movingObject, Barrier* p_barrier)
{
	bool is_collided = false;
	if(p_movingObject->GetSize() <= p_barrier->size){
		for(int pointID = 0; pointID < POINT_COUNT; pointID++){
			if(p_movingObject->point[pointID].x < p_barrier->x_right && p_movingObject->point[pointID].x > p_barrier->x_left && p_movingObject->point[pointID].y < p_barrier->y_down && p_movingObject->point[pointID].y > p_barrier->y_up){
				is_collided = true;
				return is_collided;
			}
		}
	}
	else{
		for(int pointID = 0; pointID < POINT_COUNT; pointID++){
			if(p_barrier->point[pointID].x < p_movingObject->x_right && p_barrier->point[pointID].x > p_movingObject->x_left && p_barrier->point[pointID].y < p_movingObject->y_down && p_barrier->point[pointID].y > p_movingObject->y_up){
				is_collided = true;
				return is_collided;
			}
		}
	}
	return is_collided;
}
bool Game::GetTwoCollided(MovingObject* p_movingObject1, MovingObject* p_movingObject2)
{
	bool is_collided = false;
	if(p_movingObject1->GetSize() <= p_movingObject2->GetSize()){ 
		for(int pointID = 0; pointID < POINT_COUNT; pointID++){
			if(p_movingObject1->point[pointID].x < p_movingObject2->x_right && p_movingObject1->point[pointID].x > p_movingObject2->x_left && p_movingObject1->point[pointID].y < p_movingObject2->y_down && p_movingObject1->point[pointID].y > p_movingObject2->y_up){
				is_collided = true;
				return is_collided;
			}
		}
	}
	else{
		for(int pointID = 0; pointID < POINT_COUNT; pointID++){
			if(p_movingObject2->point[pointID].x < p_movingObject1->x_right && p_movingObject2->point[pointID].x > p_movingObject1->x_left && p_movingObject2->point[pointID].y < p_movingObject1->y_down && p_movingObject2->point[pointID].y > p_movingObject1->y_up){
				is_collided = true;
				return is_collided;
			}
		}
	}
	return is_collided;
}
bool Game::GetOutBoundary(MovingObject* p_movingObject, Boundary* p_boundary)
{
	bool is_collided = false;
	for(int pointID = 0; pointID < POINT_COUNT; pointID++){
		if(p_movingObject->point[pointID].x > p_boundary->x_right || p_movingObject->point[pointID].x < p_boundary->x_left || p_movingObject->point[pointID].y > p_boundary->y_down || p_movingObject->point[pointID].y < p_boundary->y_up){
			is_collided = true;
			return is_collided;
		}
	}
	return is_collided;
}
bool Game::GetTwoCollided(MovingObject* p_movingObject, Box* p_box)
{
	bool is_collided = false;
	if(p_box->size <= p_movingObject->GetSize()){ 
		for(int pointID = 0; pointID < POINT_COUNT; pointID++){
			if(p_box->point[pointID].x < p_movingObject->x_right && p_box->point[pointID].x > p_movingObject->x_left && p_box->point[pointID].y < p_movingObject->y_down && p_box->point[pointID].y > p_movingObject->y_up){
				is_collided = true;
				return is_collided;
			}
		}
	}
	else{
		for(int pointID = 0; pointID < POINT_COUNT; pointID++){
			if(p_movingObject->point[pointID].x < p_box->x_right && p_movingObject->point[pointID].x > p_box->x_left && p_movingObject->point[pointID].y < p_box->y_down && p_movingObject->point[pointID].y > p_box->y_up){
				is_collided = true;
				return is_collided;
			}
		}
		cout<<"The box is larger than the worker"<<endl;
	}
	return is_collided;
}
bool Game::UpdateGrasp(MovingObject* p_movingObject, Box* p_box, Team* p_team, const int teammateID)
{
	bool is_grasp;
	if((p_box->x_left - p_movingObject->x_right) <= 0.5 && (p_box->x_left - p_movingObject->x_right) >= 0.0 && abs(p_movingObject->point[CENTER].y - p_box->point[CENTER].y) <= 0.5){
		(p_team->p_worker[teammateID]).SetGrasp(Worker::LEFT_GRASP);
		p_movingObject->SetPos(3,4,3,4); 
		is_grasp = true;
	}
	else if((p_movingObject->x_left - p_box->x_right) <= 0.5 && (p_movingObject->x_left - p_box->x_right) >= 0.0 && abs(p_movingObject->point[CENTER].y - p_box->point[CENTER].y) <= 0.5){
		(p_team->p_worker[teammateID]).SetGrasp(Worker::RIGHT_GRASP);
		p_movingObject->SetPos(5,6,3,4);
		is_grasp = true;
	}
	else{
		(p_team->p_worker[teammateID]).SetGrasp(Worker::FREE);
		is_grasp = false;
	}
	return is_grasp;
}
bool Game::UpdatePos(MovingObject* p_movingObject[], Box* p_box, Team* p_team)
{
	bool isChanged = false;
	if((p_team->p_worker[WORKER1]).GetAction() == (p_team->p_worker[WORKER2]).GetAction() && (p_team->p_worker[WORKER1]).GetAction() != STAY){
		isChanged = true;
		double s = UNIT_DIS + GetRand()*2*RAND_DIS - RAND_DIS;//每次移动距离等于1+[-0.1,0.1]
		int action = (p_team->p_worker[WORKER1]).GetAction();
		for(int objectID = 0; objectID < MAX_MO_COUNT; objectID++){ 	
			double x_left = p_movingObject[objectID]->x_left;
			double x_right = p_movingObject[objectID]->x_right;
			double y_up = p_movingObject[objectID]->y_up;
			double y_down = p_movingObject[objectID]->y_down;
			if(action == LEFT){
				x_left -= s;
				x_right -= s;
			}
			else if(action == RIGHT){
				x_left += s;
				x_right += s;
			}
			else if(action == UP){
				y_up -= s;
				y_down -= s;
			}
			else if(action == DOWN){
				y_up += s;
				y_down += s;
			}
			else{
				;
			}
			p_movingObject[objectID]->SetPos(x_left, x_right, y_up, y_down);
		}
		double x_left = p_box->x_left;
		double x_right = p_box->x_right;
		double y_up = p_box->y_up;
		double y_down = p_box->y_down;
		if(action == LEFT){
			x_left -= s;
			x_right -= s;
		}
		else if(action == RIGHT){
			x_left += s;
			x_right += s;
		}
		else if(action == UP){
			y_up -= s;
			y_down -= s;
		}
		else if(action == DOWN){
			y_up += s;
			y_down += s;
		}
		else{
			;
		}
		p_box->SetPos(x_left, x_right, y_up, y_down);
	}
	return isChanged;
}
/*----------- Return true if the MovingObject collides with the obstacle and goes out of boundary.-----------*/
bool Game::GetIsCollided(MovingObject* p_movingObject, Barrier* p_barrier[], Boundary* p_boundary)
{
	bool isCollided;
	// return true if the MovingObjectcollides with the obstacle/barrier?
	for(int barID = 0; barID < MAX_BAR_COUNT; barID++){
		isCollided = GetTwoCollided(p_movingObject, p_barrier[barID]);
		if(isCollided){ 
			return isCollided;
		}
	}
	//return true if the MovingObject goes out of boundary?
    isCollided = GetOutBoundary(p_movingObject, p_boundary);
	if(isCollided){ 
		return isCollided;
	}
	return isCollided;    
}
int Game::GetStateIndexForOneTeam(Team* p_team[], const int teamID, MovingObject* p_movingObject[])
{
	int state_index;
	state_index = 0;
	double base_num;
	base_num = pow(WIDTH/UNIT_DISCRETE * HEIGHT/UNIT_DISCRETE * Worker::GRASP_COUNT,MAX_TEAMMATE+0.0);
	for(int teammateID = 0; teammateID < MAX_TEAMMATE; teammateID++){
		base_num = base_num / (WIDTH/UNIT_DISCRETE);
		state_index += floor(p_movingObject[teammateID]->point[CENTER].x/UNIT_DISCRETE) * base_num;
		base_num = base_num / (HEIGHT/UNIT_DISCRETE);
		state_index += floor(p_movingObject[teammateID]->point[CENTER].y/UNIT_DISCRETE) * base_num;
		base_num = base_num / Worker::GRASP_COUNT;
		state_index += (p_team[teamID]->p_worker[teammateID]).GetGrasp() * base_num;
	}
	return (int)state_index;
}
void Game::SetStateIndexForOneTeam(Team* p_team[], const int teamID, int state_index)
{
	for(int teammateID = 0; teammateID < MAX_TEAMMATE; teammateID++){
		(p_team[teamID]->p_worker[teammateID]).SetStateIndex(state_index);
	}
}
/*-----------Return true if an episode has not ended-----------*/
bool Game::IsGoing(Team* p_team[], const int step)
{
	bool is_going;
	if(step < MAX_STEP_COUNT-1){   
		is_going = GetIsGoing();
	}
	else{      
		is_going = false;
	}
	SetIsGoing(is_going);
	return is_going;
}
void Game::InitializeLearning(Team* p_team[])
{
	for(int teamID = 0; teamID < MAX_TEAM; teamID++){
		int action_set[MAX_ACTION_COUNT];
		int action_count;
		GetActionSet(p_team, teamID, action_set, action_count);
		for(int teammateID = 0; teammateID < MAX_TEAMMATE; teammateID++){
			(p_team[teamID]->p_worker[teammateID]).p_strategy->InitializeLearning(action_set, action_count);
		}
	}
}
void Game::UpdateStrategyAfterEachStep(Team* p_team[], int pre_action_set[][MAX_ACTION_COUNT], int pre_action_count[], int cur_action_set[][MAX_ACTION_COUNT], int cur_action_count[], unsigned episode_id, const int step)

{
	for(int teamID = 0; teamID < MAX_TEAM; teamID++){
		int one_team_pre_action_set[MAX_ACTION_COUNT], one_team_pre_action_count, one_team_cur_action_set[MAX_ACTION_COUNT], one_team_cur_action_count;
		for(int actionID = 0; actionID < MAX_ACTION_COUNT; actionID++){
			one_team_pre_action_set[actionID] = pre_action_set[teamID][actionID] ;
			one_team_cur_action_set[actionID] = cur_action_set[teamID][actionID] ;
		}
		one_team_pre_action_count = pre_action_count[teamID];
		one_team_cur_action_count = cur_action_count[teamID];
		for(int teammateID = 0; teammateID < MAX_TEAMMATE; teammateID++){
			((p_team[teamID]->p_worker[teammateID]).p_strategy)->UpdateStrategyAfterEachStep((p_team[teamID]->p_worker[teammateID]).GetPreStateIndex(), \
			(p_team[teamID]->p_worker[teammateID]).GetCurStateIndex(), (p_team[teamID]->p_worker[teammateID]).GetStateIndex(), \
			(p_team[teamID]->p_worker[teammateID]).GetAction(), p_team[teamID]->GetAllInsReward(), one_team_pre_action_set, one_team_pre_action_count, \
			one_team_cur_action_set, one_team_cur_action_count, episode_id, step);  
		}
	}
}
void Game::UpdateStrategyAfterEachEpisode(Team* p_team[], const int step)
{
	for(int teamID = 0; teamID < MAX_TEAM; teamID++){
		int action_set[MAX_ACTION_COUNT];
	    int action_count;
		GetActionSet(p_team, teamID, action_set, action_count);
		for(int teammateID = 0; teammateID < MAX_TEAMMATE; teammateID++){
			(p_team[teamID]->p_worker[teammateID]).p_strategy->UpdateStrategyAfterEachEpisode(step, action_set, action_count);  
		}
	}
}
void Game::UpdateAlgorithmParameter(Team* p_team[], const unsigned episode_id)
{
	for(int teamID = 0; teamID < MAX_TEAM; teamID++){
		for(int teammateID = 0; teammateID < MAX_TEAMMATE; teammateID++){
			(p_team[teamID]->p_worker[teammateID]).p_strategy->UpdateAlgorithmParameter(episode_id);  //null operation
		}
	}
}
/*-----------Select an action for each agent.-----------*/
void Game::PerformAction(Team* p_team[], int mode, unsigned episode_id, const int step)
{
	for(int teamID = 0; teamID < MAX_TEAM; teamID++){
		int action_set[MAX_ACTION_COUNT];
		int action_count;
		GetActionSet(p_team, teamID, action_set, action_count);
		p_team[teamID]->PerformAction(mode, action_set, action_count, episode_id, step);
	}
}
void Game::SetPreStateIndex(Team* p_team[])
{
	for(int teamID = 0; teamID < MAX_TEAM; teamID++){
		for(int teammateID = 0; teammateID < MAX_TEAMMATE; teammateID++){
			(p_team[teamID]->p_worker[teammateID]).SetPreStateIndex((p_team[teamID]->p_worker[teammateID]).GetStateIndex());
		}
	}
}
void Game::SetCurStateIndex(Team* p_team[], MovingObject* p_movingObject[])
{
	for(int teamID = 0; teamID < MAX_TEAM; teamID++){
		for(int teammateID = 0; teammateID < MAX_TEAMMATE; teammateID++){
			int stateIndex;
		    stateIndex = GetStateIndexForOneTeam(p_team, teamID, p_movingObject);
			(p_team[teamID]->p_worker[teammateID]).SetCurStateIndex(stateIndex);
		}
	}
}
void Game::CopyMovingObject(MovingObject* p_originMovingObject, MovingObject* p_copyMovingObject) 
{
	p_copyMovingObject->SetPos(p_originMovingObject->x_left, p_originMovingObject->x_right, p_originMovingObject->y_up, p_originMovingObject->y_down);
	p_copyMovingObject->SetIsActive(p_originMovingObject->GetIsActive());
	p_copyMovingObject->SetSize(p_originMovingObject->GetSize());
}
void Game::CopyBox(Box* p_originBox, Box* p_copyBox)  
{
	p_copyBox->SetPos(p_originBox->x_left, p_originBox->x_right, p_originBox->y_up, p_originBox->y_down);
}
double Game::GetRand()
{
	double temp_randMin,temp_randMax,temp_randNumber;
    temp_randMin = 0.0;
    temp_randMax = 1.0;
    temp_randNumber = (double)rand()/RAND_MAX * (temp_randMax-temp_randMin) + temp_randMin;
	return temp_randNumber;
}
void Game::GetAllCollided(MovingObject* p_movingObject[], MovingObject* p_temp_movingObject[], bool isCollided[])
{
	bool is_one_collided;
	is_one_collided = true;
	while(is_one_collided){
		is_one_collided = false;
		for(int objectID1 = 0; objectID1 < MAX_MO_COUNT-1; objectID1++){
			isCollided[objectID1] = false;
			bool temp_collided[MAX_MO_COUNT-1] = {false, false};
			for(int objectID2 = 0; objectID2 < MAX_MO_COUNT-1; objectID2++){
				if(objectID2 != objectID1){
					bool collided;
					collided = GetTwoCollided(p_temp_movingObject[objectID1], p_temp_movingObject[objectID2]);
					if(collided){
						temp_collided[objectID1] = true;
						temp_collided[objectID2] = true;
						is_one_collided = true;
						isCollided[objectID1] = true;
					}
				}
			}
			for(int objectID = 0; objectID < MAX_MO_COUNT-1; objectID++){
				if(temp_collided[objectID]){
					CopyMovingObject(p_movingObject[objectID], p_temp_movingObject[objectID]);
				}
			}
		}
	}
}
