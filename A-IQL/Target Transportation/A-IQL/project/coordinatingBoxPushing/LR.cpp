//main.app A-IQL
//2022-06-04
//by Zhen Zhang 
#include"stdio.h"
#include <ctime>
#include<fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <math.h>
#include "Game.h"
#include "Result.h"
#include "GlobalVariable.h"
#include "A_IQL.h"
#include "Barrier.h"
#include "Boundary.h"
#include "Box.h"
#include "Home.h"
#include "MovingObject.h"
#include "Test.h"

using namespace std;


unsigned episode_id;                         
unsigned episodeCount;
const int TEST = 1;  // For debug. Print the map in one evaluation episode.
int main()
{	
	srand(1); /* the seed must be set to the same value for reproduction*/
    Game* p_game = new Game;  
	Team* p_team[MAX_TEAM];
	Team* p_temp_team = new Team[MAX_TEAM];  
	for(int teamID = 0;teamID < MAX_TEAM;teamID++){
		p_team[teamID] = &(p_temp_team[teamID]);
	}
	//Set the learning method adopted by the worker.
	for(int teammateID = 0; teammateID < MAX_TEAMMATE; teammateID++){  
		(p_team[TEAM1]->p_worker[teammateID]).p_strategy = new A_IQL;    
	}
	Home home(3,6,0,2);
	Home* p_home = &home;
	Barrier bar0(4,5,2,3);
	Barrier bar1(0,2,6,7);
	Barrier bar2(3.0,9,6,7);
	/*if(TEST == 1){
		Barrier bar2(3,9,6,7);
	}*/
	Barrier bar3(8,9,7,9);
	Barrier* p_barrier[MAX_BAR_COUNT];
	p_barrier[0] = &bar0;
	p_barrier[1] = &bar1;
	p_barrier[2] = &bar2;
	p_barrier[3] = &bar3;

	Boundary boundary(0,9,0,9);
	Boundary* p_boundary = &boundary;
	Box box(4,5,3.33,3.66);
	Box* p_box = &box;
	MovingObject mo0(0,1,7,8);                   // Set the initial position of agent1.
	MovingObject mo1(4,5,7,8);                   // Set the initial position of agent2.
	MovingObject mo2(3,6,3,4);                   // Set the initial position of the combination.
	MovingObject* p_movingObject[MAX_MO_COUNT];
	p_movingObject[WORKER1] = &mo0;
	p_movingObject[WORKER2] = &mo1;
	p_movingObject[COMBINATION] = &mo2;

	Test* p_test = new Test;

	Result* p_result = new Result; 
	double averageReward[MAX_EPISODE_COUNT / episodePerBlock];  // Sliding average reward per episode during training
	double averageStep[MAX_EPISODE_COUNT / episodePerBlock];    // Sliding average step per episode during training 

	double avr_reward[MAX_RUN_COUNT];                         // average reward per episode during evaluation in each run 
	double avr_step[MAX_RUN_COUNT];                           // average step per episode during evaluation in each run 
	double successTimes[MAX_RUN_COUNT];                       // the number of successful episode during evaluation in each run 
	double successRate[MAX_RUN_COUNT];                        // average success rate during evaluation in each run 
	double all_avr_reward;                                    // average reward per episode during evaluation (averaged on each run)
	double all_avr_step;                                      // average step per episode during evaluation (averaged on each run)
	double all_avr_successRate;                               // average success rate during evaluation (averaged on each run)
	int run_id;
	for(run_id = 0; run_id < MAX_RUN_COUNT; run_id++){
		srand(run_id); 
		p_game->InitializeLearning(p_team);                   
		if (run_id == 0){
			//SaveIniP(ptr_agent);
		}
		ofstream agentFile("completeRecord.dat",ios::app);
		if(!agentFile){
			cerr<<"File completeRecord.dat could not be opened"<<endl;
			exit(1);
		}
		agentFile<<"Run"<<" "<<run_id+1<<endl;
		agentFile.close();

		double accReward50Episode;
		accReward50Episode = 0;
		double accStep50Episode;
		accStep50Episode = 0;
		int blockID;
		blockID = 0;
		int effective_episode = 0;
		int ineffective_episode = 0;
		double success = 0; 
		double temp_reward = 0;
		double temp_step = 0;
		double temp_success = 0;
		for(episode_id = 0;episode_id < MAX_EPISODE_COUNT;episode_id++){
			p_game->Reset(p_team, p_movingObject, p_box);                      //Prepare for the beginning of one episode.
			int min_step = MINIMAL_STEP;                                       //the minimum step in the second half of the training stage and the evaluaton stage
			int step;
			step = 0;
			int mode;
			if(episode_id < MAX_LEARNING_COUNT){
				mode = Strategy::LEARN;
			}
			else{
				mode = Strategy::EVALUATE;
			}
			bool is_success = false; //20220225
			do{	
				for(int teamID = 0; teamID < MAX_TEAM; teamID++){
					int stateIndex;
				    stateIndex = p_game->GetStateIndexForOneTeam(p_team, teamID, p_movingObject);
					p_game->SetStateIndexForOneTeam(p_team, teamID, stateIndex);
				}
				p_game->SetPreStateIndex(p_team);  
				//----------------print the map for one evalatuion episode----------------
				if(TEST == 1 && episode_id == MAX_LEARNING_COUNT && step == 0 && run_id == MAX_RUN_COUNT-1){
					p_test->PrintTestInformationMap(p_game, p_team, p_box, p_movingObject, "TestMap.dat", "app", step);
				}
				//----------------print the map for one evalatuion episode----------------

				int pre_action_set[MAX_TEAM][MAX_ACTION_COUNT];
				int pre_action_count[MAX_TEAM];
				for(int teamID = 0; teamID < MAX_TEAM; teamID++){
					int action_set[MAX_ACTION_COUNT];
				    int action_count;
					p_game->GetActionSet(p_team, teamID, action_set, action_count);
					pre_action_count[teamID] = action_count;
					for(int teammateID = 0; teammateID < MAX_ACTION_COUNT; teammateID++){
						pre_action_set[teamID][teammateID] = action_set[teammateID];
					}
				}

				p_game->PerformAction(p_team, mode, episode_id, step);                                                       // Each agent/robot selects an aciton at state s.
				p_game->Update(p_team, p_movingObject, p_box, p_barrier, p_home, p_boundary, step, episode_id,is_success);   // Update the reward, the position for each agent and the posiiton of the box. Judge the end of an episode. 
				bool is_going = p_game->IsGoing(p_team, step);  	

				int cur_action_set[MAX_TEAM][MAX_ACTION_COUNT];
				int cur_action_count[MAX_TEAM];
				for(int teamID = 0; teamID < MAX_TEAM; teamID++){
					int action_set[MAX_ACTION_COUNT];
				    int action_count;
					p_game->GetActionSet(p_team, teamID, action_set, action_count);
					cur_action_count[teamID] = action_count;
					for(int teammateID = 0; teammateID < MAX_ACTION_COUNT; teammateID++){
						cur_action_set[teamID][teammateID] = action_set[teammateID];
					}
				}

				p_game->SetCurStateIndex(p_team, p_movingObject);                                                              // Get the next state s'.

				//----------------print the map for one evalatuion episode----------------
				if(TEST == 1 && episode_id == MAX_LEARNING_COUNT && run_id == MAX_RUN_COUNT-1){
					p_test->PrintTestInformationMap(p_game, p_team, p_box, p_movingObject, "TestMap.dat", "app", step);
				}
				//----------------print the map for one evalatuion episode----------------

				p_game->UpdateStrategyAfterEachStep(p_team, pre_action_set, pre_action_count, cur_action_set, cur_action_count, episode_id, step);   // Update Q - functin Q(s, ai) and p(ai | s) for each robot i.
				step++;		
			}while(p_game->GetIsGoing()); // One episode ends.
            p_game->UpdateStrategyAfterEachEpisode(p_team, step);  // null operation for A-IQL

			if(episode_id >= MAX_LEARNING_COUNT){
				temp_reward += p_team[TEAM1]->GetAllAccReward();   
				temp_step += step;
				if(step <= min_step && is_success){  // 20220225 If 14 steps are used in an evaluation episode and the target enters the home zone from the right (which means that a cumulative reward of 178 is obtained), one success is obtained. 
					++success;
				}
			}
			
			if(mode == Strategy::LEARN){
				p_game->UpdateAlgorithmParameter(p_team, episode_id);
			}
			/*---------------process the result-------------------*/
			accReward50Episode = accReward50Episode + p_team[TEAM1]->GetAllAccReward();
			accStep50Episode = accStep50Episode + step ; // caution
			if((episode_id + 1)%episodePerBlock == 0 ){
				if(run_id == 0 ){
					averageReward[blockID] = accReward50Episode / episodePerBlock;
					averageStep[blockID] = accStep50Episode / episodePerBlock;
				}
				else{
					averageReward[blockID] = averageReward[blockID] * ((double)(run_id) / (double)(run_id + 1)) + (accReward50Episode / episodePerBlock) / (double)(run_id + 1);
					averageStep[blockID] = averageStep[blockID] * ((double)(run_id) / (double)(run_id + 1)) + (accStep50Episode / episodePerBlock) / (double)(run_id + 1);
				}
				accReward50Episode = 0;
				accStep50Episode = 0;
				blockID++;
			}	
			/*----------------------------------*/
		    //system("pause");
		}
		if (run_id == MAX_RUN_COUNT-1){
			(p_team[TEAM1]->p_worker[WORKER1]).p_strategy->SaveStrategy();
		}
		//process the result
		avr_reward[run_id] = temp_reward / MAX_EVALUATION_COUNT;
		avr_step[run_id] = temp_step / MAX_EVALUATION_COUNT;
		successTimes[run_id] = success;
		successRate[run_id] = success/MAX_EVALUATION_COUNT;
		cout<<"Successes in evaluation: "<<success<<endl;
		cout<<"Success rate: "<<success/(MAX_EPISODE_COUNT-MAX_LEARNING_COUNT)<<endl;

		cout<<"run: "<<run_id+1<<endl;
		cout<<"reward: "<<avr_reward[run_id]<<endl;
		cout<<"step: "<<avr_step[run_id]<<endl;
		cout<<"successRate: "<<successRate[run_id]<<endl;
	}

	//process the result
	double temp_avr_reward = 0;
	double temp_avr_step = 0;
	double temp_avr_successRate = 0;
	for(run_id = 0; run_id < MAX_RUN_COUNT; run_id++){
		temp_avr_reward += avr_reward[run_id];
		temp_avr_step += avr_step[run_id];
		temp_avr_successRate += successRate[run_id];
	}
	all_avr_reward = temp_avr_reward / MAX_RUN_COUNT;
	all_avr_step = temp_avr_step / MAX_RUN_COUNT;
	all_avr_successRate = temp_avr_successRate / MAX_RUN_COUNT;
    
	p_result->SaveData(avr_reward, MAX_RUN_COUNT, "avr_reward.dat","app");
	p_result->SaveData(avr_step, MAX_RUN_COUNT, "avr_step.dat","app");
	p_result->SaveData(successTimes, MAX_RUN_COUNT, "successTimes.dat","app");
	p_result->SaveData(successRate, MAX_RUN_COUNT, "successRate.dat","app");

	p_result->SaveData(averageReward, MAX_EPISODE_COUNT/episodePerBlock,"averageReward_slide.dat","app");
    p_result->SaveData(averageStep, MAX_EPISODE_COUNT/episodePerBlock, "averageStep_slide.dat","app");

	p_result->SaveData(all_avr_reward, "all_avr_reward.dat","app");
	p_result->SaveData(all_avr_step, "all_avr_step.dat","app");
	p_result->SaveData(all_avr_successRate, "all_avr_successRate.dat","app");


	//Free heap memory
	delete p_result;
	for(int teamID = 0; teamID < MAX_TEAM; teamID++){
		delete p_team[teamID]->p_worker;
	}
	delete p_game;
	delete[] p_temp_team; 
	if(TEST == 1){
		delete p_test;
	}

	/*----------------------------------*/
	system("pause");
	return 0;
}