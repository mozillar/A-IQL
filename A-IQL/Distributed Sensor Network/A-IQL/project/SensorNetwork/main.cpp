//main.app A-IQL
//2022-06-04
//by Zhen Zhang 
#include "target.h"
#include "Sensor.h"
#include "savedata.h"
#include"stdio.h"
#include <ctime>
#include<math.h>
#include<fstream>
#include "deque"
#include<string>
#include <vector>
#include <iostream>
using namespace std;

extern const short FOCUS_CELL1, FOCUS_CELL2, FOCUS_CELL3,FOCUS_CELL4, FOCUS_CELL5, FOCUS_CELL6, NO_FOCUS, FOCUS_OUT;
extern const int MAX_STATE_COUNT;
extern const int MAX_ACTION_COUNT;
extern const int MAX_STEP_COUNT; 
// the actions of the target
extern const int UP;
extern const int DOWN;
extern const int LEFT;
extern const int RIGHT;
// a special state that means the target is eliminated 
extern const int OUT; 


static unsigned seed;                          // seed for generating random numbers
const short MAX_SENSOR_COUNT = 12;             
const short MAX_TARGET_COUNT = 3;             
const short seed_offset_betweenTargets = 1; 
unsigned seed_offset_betweenEpisodes;        
unsigned episode_id;                         
unsigned episodeCount;
const int MAX_RUN_COUNT = 50;                 // The number of times of experiment. One run contains MAX_LEARNING_COUNT episodes for training and MAX_EVALUATION_COUNT episodes for evaluation.
const int MAX_LEARNING_COUNT = 1200000;       // The number of episodes for training in one experiment (one run). L in the paper.
const int MAX_EVALUATION_COUNT = 100000;      // The number of episodes for evaluation in one experiment (one run).
const double NEG_INF = -1E06;
const int MAX_EPISODE_COUNT = MAX_LEARNING_COUNT + MAX_EVALUATION_COUNT;  //DO NOT CHANGE THIS LINE.   The number of total episodes in one experiment (one run).
const int MAX_TARGET_RANDCOUNT = MAX_STEP_COUNT;
const int HIT2 = 2;                 
const int HIT1 = 1;
const int FOCUS_REWARD = -1;
const int NO_FOCUS_REWARD = 0;
const int CAPTURE_REWARD = 10;
const int MAX_CELL_COUNT = 6;
const double BEST_ACC_REWARD = 42;
const int BEST_STEP = 3;
const double MAX_ENERGY = 3.0;
const int MAX_SENSOR_INVOLVED = 2;
const int TARGET_ACTION_COUNT = 4;
const int NOT_INVOLVED = 0;
const int INVOLVED = 1;

/*---parameters for the IQL-SS algorithm---*/
double discountFactor = 0.9;                                            //discount factor                      
const double INI_DISCOUNT_FACTOR = discountFactor;
const double T = 2000;                                                  //The period T in the paper
const int episodePerBlock = 1000;
const double INI_THRESH = 2.0;
double thresh = INI_THRESH;
const double END_THRESH = 0.01;

double Q[MAX_SENSOR_COUNT][MAX_STATE_COUNT][MAX_ACTION_COUNT];             // the Q-function Q(s,ai) for each (s,ai) and each sensor i
int visit[MAX_SENSOR_COUNT][MAX_STATE_COUNT][MAX_ACTION_COUNT];
double max_his[MAX_SENSOR_COUNT][MAX_STATE_COUNT][MAX_ACTION_COUNT];
double pi[MAX_SENSOR_COUNT][MAX_STATE_COUNT][MAX_ACTION_COUNT];            // the policy of each sensor i (the probability of p(ai|s) for each s, ai, and each sensor i)

const double END_EPSILON = 0.1;
double epsilon[MAX_SENSOR_COUNT];                                          // the curent value of epsilon of sensor i
double ini_epsilon[MAX_SENSOR_COUNT];                                      // the initial value of epsilon of sensor i

struct Cell{
	bool occupied;
};

int STATE_INDEX[MAX_STATE_COUNT][MAX_CELL_COUNT];
const int UNOCCUPIED = 0;
const int OCCUPIED = 1;


//global function
void Initialize(Sensor* ptr_sensor[]);
bool IsEpisodeGoing(Target* target[]);
void SetSensorAction(Sensor* ptr_sensor[], const short stateIndex, double Q[][MAX_STATE_COUNT][MAX_ACTION_COUNT], double pi[][MAX_STATE_COUNT][MAX_ACTION_COUNT], const int episodeID);
void SetSensorAction(Sensor* ptr_sensor[]); 
void UpdateState(Sensor* ptr_sensor[], Target* ptr_target[], const int episode_id);
void Reset(Sensor* ptr_sensor[], Target* ptr_target[]);
void SaveData(int stateIndex, Sensor* ptr_sensor[], Target* ptr_target[], unsigned episode_id, int step);
void SaveData(Target* ptr_target[]);
void SaveAverageReward(double averageReward[], int episode_count, int episodePerBlock);
void SaveAverageStep(double averageStep[], int episode_count, int episodePerBlock);
double GetRand(void);
void InitializeRLParameter(Sensor* ptr_sensor[]);
void UpdateRLParameter(double epsilon[],unsigned episode_id);
int GetStateIndex(Target* ptr_target[]);
void GetJointAction(Sensor* ptr_sensor[], int jointAction[]);
void UpdateQ(Sensor* ptr_sensor[], const short pre_stateIndex, const short cur_stateIndex, int pre_jointAction[], int step);
void UpdateTargetPos(Target* ptr_target[]);

//for debug
void SaveIniQ(Sensor* ptr_sensor[]);     // Save the initial Q-function for all agents(sensors).
void SaveQ(Sensor* ptr_sensor[]);        // Save the Q-function for all agents after one run.
void SavePi(Sensor* ptr_sensor[]);       // Save the policy for all agents after one run.


int main()
{
	srand(1);                           /* the seed must be set to the same value for reproduction*/
	// Set the number of neighbor sensor for each sensor(agent).
	Sensor sensor1(3); 
	Sensor sensor2(5);
	Sensor sensor3(5);
	Sensor sensor4(3);
	Sensor sensor5(5);
	Sensor sensor6(8);
	Sensor sensor7(8);
	Sensor sensor8(5);
	Sensor sensor9(3);
	Sensor sensor10(5);
	Sensor sensor11(5);
	Sensor sensor12(3);

    Sensor* ptr_sensor[MAX_SENSOR_COUNT];
	ptr_sensor[0] = &sensor1;
	ptr_sensor[1] = &sensor2;
	ptr_sensor[2] = &sensor3;
	ptr_sensor[3] = &sensor4;
	ptr_sensor[4] = &sensor5;
	ptr_sensor[5] = &sensor6;
	ptr_sensor[6] = &sensor7;
	ptr_sensor[7] = &sensor8;
	ptr_sensor[8] = &sensor9;
	ptr_sensor[9] = &sensor10;
	ptr_sensor[10] = &sensor11;
	ptr_sensor[11] = &sensor12;

	Target target1;
	Target target2;
	Target target3;                           
	Target* ptr_target[MAX_TARGET_COUNT];
    ptr_target[0] = &target1;
	ptr_target[1] = &target2;
	ptr_target[2] = &target3;
	//Initialization for sensors
	Initialize(ptr_sensor);
	int step;
	double averageReward[MAX_EPISODE_COUNT/episodePerBlock];              // Sliding average reward per episode during training
	double averageStep[MAX_EPISODE_COUNT/episodePerBlock];                // Sliding average step per episode during training 
	double avr_reward[MAX_RUN_COUNT];                                     // average reward per episode during evaluation in each run 
	double avr_step[MAX_RUN_COUNT];                                       // average step per episode during evaluation in each run 
	double successTimes[MAX_RUN_COUNT];                                   // the number of successful episode during evaluation in each run 
	double successRate[MAX_RUN_COUNT];                                    // average success rate during evaluation in each run 
	double all_avr_reward;                                                // average reward per episode during evaluation (averaged on each run)
	double all_avr_step;                                                  // average step per episode during evaluation (averaged on each run)
	double all_avr_successRate;                                           // average success rate during evaluation (averaged on each run)
	for(int run_id = 0; run_id < MAX_RUN_COUNT; run_id++){
		srand(run_id);
		InitializeRLParameter(ptr_sensor);                                   // Initialize A-IQL related parameters.
		if (run_id == 0){
			SaveIniQ(ptr_sensor);
		}
		ofstream sensorNetworkFile("completeRecord.dat",ios::app);
		if(!sensorNetworkFile){
			cerr<<"File completeRecord.dat could not be opened"<<endl;
			exit(1);
		}
		sensorNetworkFile<<"Run"<<" "<<run_id+1<<endl;
		sensorNetworkFile.close();
		double accReward50Episode;
		accReward50Episode = 0;
		double accStep50Episode;
		accStep50Episode = 0;
		int blockID;
		blockID = 0;
		double temp_reward = 0;
		double temp_step = 0;
		double temp_success = 0;
		for(episode_id = 0;episode_id < MAX_EPISODE_COUNT;episode_id++){
			Reset(ptr_sensor,ptr_target);                                      //Reset the energy, position of each target and the reward for each sensor
			step = 0;
			int pre_stateIndex;
			pre_stateIndex = GetStateIndex(ptr_target);                        // Observe state s.
			SetSensorAction(ptr_sensor, pre_stateIndex, Q, pi,episode_id);     // Each sensor selects an aciton at state s.
			step++;
			int pre_jointAction[MAX_SENSOR_COUNT];                             // the joint action a executed at state s
			GetJointAction(ptr_sensor, pre_jointAction);
			int cur_jointAction[MAX_SENSOR_COUNT];                             // the joint action a executed at state s'
			int cur_stateIndex;                                                // the next state s' (transition from (s,a))
			while(IsEpisodeGoing(ptr_target)){
				if(step > MAX_STEP_COUNT){
					//cout<<"run outof MAX_STEP_COUNT";
					break;
					//system("pause");
					//exit(0);
				}	
				UpdateState(ptr_sensor, ptr_target, episode_id);	           // Update the reward for each sensor, the energy value and the position for each target.
				cur_stateIndex = GetStateIndex(ptr_target);                    // Get the next state s' (transition from (s,a)) in the form of state index
				if(episode_id >= MAX_LEARNING_COUNT){
					if(cur_stateIndex == 7){
						cout<<"cur_stateIndex "<<cur_stateIndex;
						system("pause");
					}
				}
				if(episode_id < MAX_LEARNING_COUNT){
					UpdateQ(ptr_sensor, pre_stateIndex, cur_stateIndex, pre_jointAction, step); // Update Q-functin Q(s,ai) and p(ai|s) for each agent i.
				}
				SetSensorAction(ptr_sensor, cur_stateIndex, Q, pi, episode_id);                 // Each sensor selects an aciton at state s'.
				GetJointAction(ptr_sensor, cur_jointAction);
				pre_stateIndex = cur_stateIndex;
				for(int sensorID = 0; sensorID <MAX_SENSOR_COUNT; sensorID++){
					pre_jointAction[sensorID] = cur_jointAction[sensorID];
				}
				step++;	
			}
			if(episode_id >= MAX_LEARNING_COUNT){
				temp_reward += Sensor::GetAllAccReward();
				temp_step += (step-1);
				if(Sensor::GetAllAccReward() == BEST_ACC_REWARD && (step-1) == BEST_STEP){
					temp_success++;
				}
			}
			UpdateRLParameter(epsilon, episode_id);                                              // Update related parameters of A-IQL
			accReward50Episode = accReward50Episode + Sensor::GetAllAccReward();
			accStep50Episode = accStep50Episode + (step-1);  // caution
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
		}// One run ends.
		if (run_id == 0){
			SaveQ(ptr_sensor);
			SavePi(ptr_sensor);
		}
		avr_reward[run_id] = temp_reward / MAX_EVALUATION_COUNT;
		avr_step[run_id] = temp_step / MAX_EVALUATION_COUNT;
		successTimes[run_id] = temp_success;
		successRate[run_id] = temp_success / MAX_EVALUATION_COUNT;

		cout<<"run: "<<run_id+1<<endl;
		cout<<"reward: "<<avr_reward[run_id]<<endl;
		cout<<"step: "<<avr_step[run_id]<<endl;
		cout<<"successRate: "<<successRate[run_id]<<endl;
	}//All runs end.
	double temp_avr_reward = 0;
	double temp_avr_step = 0;
	double temp_avr_successRate = 0;
	for(int run_id = 0; run_id < MAX_RUN_COUNT; run_id++){
		temp_avr_reward += avr_reward[run_id];
		temp_avr_step += avr_step[run_id];
		temp_avr_successRate += successRate[run_id];
	}
	all_avr_reward = temp_avr_reward / MAX_RUN_COUNT;
	all_avr_step = temp_avr_step / MAX_RUN_COUNT;
	all_avr_successRate = temp_avr_successRate / MAX_RUN_COUNT;

	//Save results.
	SaveData(avr_reward, MAX_RUN_COUNT, "avr_reward.dat","app");
	SaveData(avr_step, MAX_RUN_COUNT, "avr_step.dat","app");
	SaveData(successTimes, MAX_RUN_COUNT, "successTimes.dat","app");
	SaveData(successRate, MAX_RUN_COUNT, "successRate.dat","app");

	SaveData(all_avr_reward, "all_avr_reward.dat","app");
	SaveData(all_avr_step, "all_avr_step.dat","app");
	SaveData(all_avr_successRate, "all_avr_successRate.dat","app");

	SaveAverageReward(averageReward, MAX_EPISODE_COUNT, episodePerBlock);
	SaveAverageStep(averageStep, MAX_EPISODE_COUNT, episodePerBlock);

	/*sensor1.Print();
	sensor2.Print();
	sensor3.Print();
	sensor4.Print();
	sensor5.Print();
	sensor6.Print();
	sensor7.Print();
	sensor8.Print();

	target1.Print();
	target2.Print();*/

	/*Target ptr_target1;
	ptr_target1.Print();
	Target ptr_target2;
	ptr_target2.Print();
	Target::DecCount();
	ptr_target2.Print();*/
	system("pause");

	return 0;
}
/*-------------------Initialization for sensors(agents)-------------------*/
void Initialize(Sensor* ptr_sensor[])
{   
	seed_offset_betweenEpisodes = 1;
	/*Initialize the neighbor sensors for each sensor*/
	SensorActionSet sensor1ActionSet;
	SensorActionSet sensor2ActionSet;
	SensorActionSet sensor3ActionSet;
	SensorActionSet sensor4ActionSet;
	SensorActionSet sensor5ActionSet;
	SensorActionSet sensor6ActionSet;
	SensorActionSet sensor7ActionSet;
	SensorActionSet sensor8ActionSet;
	SensorActionSet sensor9ActionSet;
	SensorActionSet sensor10ActionSet;
	SensorActionSet sensor11ActionSet;
	SensorActionSet sensor12ActionSet;

	sensor1ActionSet.actionCount = 2;
	sensor1ActionSet.action[0] = NO_FOCUS;
	sensor1ActionSet.action[1] = FOCUS_CELL1;
	ptr_sensor[0]->SetActionSet(sensor1ActionSet);

	sensor2ActionSet.actionCount = 3;
	sensor2ActionSet.action[0] = NO_FOCUS;
	sensor2ActionSet.action[1] = FOCUS_CELL1;
	sensor2ActionSet.action[2] = FOCUS_CELL2;
	ptr_sensor[1]->SetActionSet(sensor2ActionSet);

	sensor3ActionSet.actionCount = 3;
	sensor3ActionSet.action[0] = NO_FOCUS;
	sensor3ActionSet.action[1] = FOCUS_CELL2;
	sensor3ActionSet.action[2] = FOCUS_CELL3;
	ptr_sensor[2]->SetActionSet(sensor3ActionSet);

	sensor4ActionSet.actionCount = 2;
	sensor4ActionSet.action[0] = NO_FOCUS;
	sensor4ActionSet.action[1] = FOCUS_CELL3;
	ptr_sensor[3]->SetActionSet(sensor4ActionSet);

	sensor5ActionSet.actionCount = 3;
	sensor5ActionSet.action[0] = NO_FOCUS;
	sensor5ActionSet.action[1] = FOCUS_CELL1;
	sensor5ActionSet.action[2] = FOCUS_CELL4;
	ptr_sensor[4]->SetActionSet(sensor5ActionSet);

	sensor6ActionSet.actionCount = 5;
	sensor6ActionSet.action[0] = NO_FOCUS;
	sensor6ActionSet.action[1] = FOCUS_CELL1;
	sensor6ActionSet.action[2] = FOCUS_CELL2;
	sensor6ActionSet.action[3] = FOCUS_CELL4;
	sensor6ActionSet.action[4] = FOCUS_CELL5;
	ptr_sensor[5]->SetActionSet(sensor6ActionSet);

	sensor7ActionSet.actionCount = 5;
	sensor7ActionSet.action[0] = NO_FOCUS;
	sensor7ActionSet.action[1] = FOCUS_CELL2;
	sensor7ActionSet.action[2] = FOCUS_CELL3;
	sensor7ActionSet.action[3] = FOCUS_CELL5;
	sensor7ActionSet.action[4] = FOCUS_CELL6;
	ptr_sensor[6]->SetActionSet(sensor7ActionSet);

	sensor8ActionSet.actionCount = 3;
	sensor8ActionSet.action[0] = NO_FOCUS;
	sensor8ActionSet.action[1] = FOCUS_CELL3;
	sensor8ActionSet.action[2] = FOCUS_CELL6;
	ptr_sensor[7]->SetActionSet(sensor8ActionSet);

	sensor9ActionSet.actionCount = 2;
	sensor9ActionSet.action[0] = NO_FOCUS;
	sensor9ActionSet.action[1] = FOCUS_CELL4;
	ptr_sensor[8]->SetActionSet(sensor9ActionSet);

	sensor10ActionSet.actionCount = 3;
	sensor10ActionSet.action[0] = NO_FOCUS;
	sensor10ActionSet.action[1] = FOCUS_CELL4;
	sensor10ActionSet.action[2] = FOCUS_CELL5;
	ptr_sensor[9]->SetActionSet(sensor10ActionSet);

	sensor11ActionSet.actionCount = 3;
	sensor11ActionSet.action[0] = NO_FOCUS;
	sensor11ActionSet.action[1] = FOCUS_CELL5;
	sensor11ActionSet.action[2] = FOCUS_CELL6;
	ptr_sensor[10]->SetActionSet(sensor11ActionSet);

	sensor12ActionSet.actionCount = 2;
	sensor12ActionSet.action[0] = NO_FOCUS;
	sensor12ActionSet.action[1] = FOCUS_CELL6;
	ptr_sensor[11]->SetActionSet(sensor12ActionSet);
}
/*-------------------Return true if an episode has not ended.-------------------*/
bool IsEpisodeGoing(Target* target[]) //2021-02-28
{ 
	for(int targetID = 0;targetID < MAX_TARGET_COUNT; targetID++){
		if(target[targetID]->GetEnergy() != 0){
			return true;
		}
	}
	return false;
}
/*-----------------Each sensor selects an aciton at state stateIndex.-----------------*/
void SetSensorAction(Sensor* ptr_sensor[], const short stateIndex, double Q[][MAX_STATE_COUNT][MAX_ACTION_COUNT],double pi[][MAX_STATE_COUNT][MAX_ACTION_COUNT], const int episodeID)
{
	if(episodeID < MAX_LEARNING_COUNT){
		int sensorID;
		for(sensorID = 0; sensorID < MAX_SENSOR_COUNT; sensorID++){
			double p_agent_action[MAX_ACTION_COUNT] = {0,0,0,0,0,0,0};
			const int ACT_COUNT = ptr_sensor[sensorID]->GetActionSet().actionCount;
			for(int actionID = 0; actionID < ACT_COUNT; actionID++){
				int action;
				action = (ptr_sensor[sensorID]->GetActionSet()).action[actionID];
				p_agent_action[actionID] = pi[sensorID][stateIndex][action];
			}

			//roulette
			double binaryPoint_pro[MAX_ACTION_COUNT+1] = {0,0,0,0,0,0,0,0};  
			for(int binaryPointID = 1; binaryPointID <= ACT_COUNT-1; binaryPointID++){
				binaryPoint_pro[binaryPointID] = p_agent_action[binaryPointID-1] + binaryPoint_pro[binaryPointID -1] ;
			}
			binaryPoint_pro[ACT_COUNT] = 1.0;

			double randNum;
			randNum = GetRand();
			for(int actionID = 0; actionID < ACT_COUNT; actionID++){
				if ((binaryPoint_pro[actionID] <= randNum ) && (randNum <= binaryPoint_pro[actionID+1])){
					int action;
					action = (ptr_sensor[sensorID]->GetActionSet()).action[actionID];
					ptr_sensor[sensorID]->SetAction(action);
					break;
				}			
			}
		}
	}
	else{
		for(int sensorID = 0; sensorID < MAX_SENSOR_COUNT; sensorID++){
			double max_pi = NEG_INF;
			for(int actionID = 0; actionID < (ptr_sensor[sensorID]->GetActionSet()).actionCount; actionID++){
				if(pi[sensorID][stateIndex][(ptr_sensor[sensorID]->GetActionSet()).action[actionID]] > max_pi){
					max_pi = pi[sensorID][stateIndex][(ptr_sensor[sensorID]->GetActionSet()).action[actionID]];
					ptr_sensor[sensorID]->SetAction((ptr_sensor[sensorID]->GetActionSet()).action[actionID]);
				}
			}
		}
	}
}
/*-----------------Update the reward for each sensor, the energy value and the position for each target.-----------------*/
void UpdateState(Sensor* ptr_sensor[], Target* ptr_target[], const int episode_id)  //Update the enegy first and then let the targets move.
{
	if(episode_id < MAX_LEARNING_COUNT/2){   // in the first half of the training stage  One sensor can make a hit.
		//20210228  20220323 
		bool is_involved[MAX_SENSOR_COUNT];
		for(int sensorID = 0;sensorID < MAX_SENSOR_COUNT;sensorID++){
			is_involved[sensorID] = NOT_INVOLVED;
		}
		int receivedFocus;
		for(int targetID = 0;targetID < MAX_TARGET_COUNT; targetID++){
			receivedFocus = 0;
			int sensorsInvolved = 0;
			if(ptr_target[targetID]->GetEnergy() > 0){
				for(int sensorID = 0;sensorID < MAX_SENSOR_COUNT;sensorID++){
					if(ptr_sensor[sensorID]->GetAction() == ptr_target[targetID]->GetPosition()){
						receivedFocus++;
						sensorsInvolved = ptr_sensor[sensorID]->index;
					}
				}
				if(receivedFocus >= HIT1){
					ptr_target[targetID]->DecEnergy();
				}
				if(ptr_target[targetID]->GetEnergy() == 0){
					is_involved[sensorsInvolved] = INVOLVED;
					ptr_sensor[sensorsInvolved]->SetInsReward(CAPTURE_REWARD*2 + FOCUS_REWARD); // maximum cumulative reward 51
					ptr_target[targetID]->SetPosition(OUT);
					Target::DecCount();
				}
			}
		}
		for(int sensorID = 0;sensorID < MAX_SENSOR_COUNT;sensorID++){
			if(is_involved[sensorID] == NOT_INVOLVED){
				if(ptr_sensor[sensorID]->GetAction() != NO_FOCUS){
					ptr_sensor[sensorID]->SetInsReward(FOCUS_REWARD);
				}
				else{
					ptr_sensor[sensorID]->SetInsReward(NO_FOCUS_REWARD);
				}
			}
		}
	}
	else{              // in the second half of the training stage  Two sensors can make a hit.
		//20210228  20220323   
		bool is_involved[MAX_SENSOR_COUNT];
		for(int sensorID = 0;sensorID < MAX_SENSOR_COUNT;sensorID++){
			is_involved[sensorID] = NOT_INVOLVED;
		}
		int receivedFocus;
		for(int targetID = 0;targetID < MAX_TARGET_COUNT; targetID++){
			receivedFocus = 0;
			int sensorsInvolved[MAX_SENSOR_INVOLVED] = {0,0};
			if(ptr_target[targetID]->GetEnergy() > 0){
				for(int sensorID = 0;sensorID < MAX_SENSOR_COUNT;sensorID++){
					if(ptr_sensor[sensorID]->GetAction() == ptr_target[targetID]->GetPosition()){
						receivedFocus++;
						sensorsInvolved[0] = sensorsInvolved[1];
						sensorsInvolved[1] = ptr_sensor[sensorID]->index;
					}
				}
				if(receivedFocus >= HIT2){
					ptr_target[targetID]->DecEnergy();
				}
				if(ptr_target[targetID]->GetEnergy() == 0){
					for(int sensorID = 0;sensorID < HIT2;sensorID++){
						is_involved[sensorsInvolved[sensorID]] = INVOLVED;
						ptr_sensor[sensorsInvolved[sensorID]]->SetInsReward(CAPTURE_REWARD + FOCUS_REWARD);  // maximum cumulative reward 42
					}
					ptr_target[targetID]->SetPosition(OUT);
					Target::DecCount();
				}
			}
		}
		for(int sensorID = 0;sensorID < MAX_SENSOR_COUNT;sensorID++){
			if(is_involved[sensorID] == NOT_INVOLVED){
				if(ptr_sensor[sensorID]->GetAction() != NO_FOCUS){
					ptr_sensor[sensorID]->SetInsReward(FOCUS_REWARD);
				}
				else{
					ptr_sensor[sensorID]->SetInsReward(NO_FOCUS_REWARD);
				}
			}
		}
	}
	/*Update reward for all sensors.*/
	double allInsReward;
	allInsReward = 0;
	for(int sensorNum = 0;sensorNum < MAX_SENSOR_COUNT;sensorNum++){
		ptr_sensor[sensorNum]->SetAccReward(ptr_sensor[sensorNum]->GetAccReward() + ptr_sensor[sensorNum]->GetInsReward());
		allInsReward = allInsReward + ptr_sensor[sensorNum]->GetInsReward();
	}
	Sensor::SetAllInsReward(allInsReward);
	Sensor::SetAllAccReward(Sensor::GetAllAccReward() + Sensor::GetAllInsReward());
	// Update the position of each target.
	UpdateTargetPos(ptr_target);
}
/*-----------------------------------Update the position of each target.-----------------------------------*/
void UpdateTargetPos(Target* ptr_target[])
{
	int target_action[TARGET_ACTION_COUNT] = {UP,DOWN,LEFT,RIGHT};
	for(int targetID = 0;targetID < MAX_TARGET_COUNT; targetID++){
		if(ptr_target[targetID]->GetEnergy() > 0){  
            vector<int> available_action;
			for(int actionID = 0;actionID < TARGET_ACTION_COUNT;actionID++){
				int temp_x, temp_y; 
				temp_x = ptr_target[targetID]->x;
				temp_y = ptr_target[targetID]->y;
				int action = target_action[actionID];
				if(action == UP){
					temp_y--;
				}
				else if(action == DOWN){
					temp_y++;
				}
				else if(action == LEFT){
					temp_x--;
				}
				else if(action == RIGHT){
					temp_x++;
				}
				else{
					;
				}
				bool is_out_bound = true;
				if(temp_x >= 1 && temp_y >= 1  && temp_x <= 3 && temp_y <= 2 ){  //没出界
					is_out_bound = false;
				}
				else{  
					continue;
				}
				if(is_out_bound == false){
					bool is_conflict = false;
					for(int target_id = 0;target_id < MAX_TARGET_COUNT; target_id++){
						if(target_id != targetID){
							if(temp_x == ptr_target[target_id]->x && temp_y == ptr_target[target_id]->y){
								is_conflict = true;
								break;
							}
						}
					}
					if(is_conflict){
						continue;
					}
					else{
						available_action.push_back(action);
					}
				}
			}
			int available_action_count = available_action.size();
			/*string str_target_action[TARGET_ACTION_COUNT] = {"UP","DOWN","LEFT","RIGHT"};
			for(int i = 0; i< available_action_count;i++){
				cout<<"targetID "<<targetID<<" Avaliable actions include:  "<<str_target_action[available_action.at(i)]<<endl;
			}
			system("pause");*/
			if(available_action_count > 0){
				int temp_x, temp_y; 
				temp_x = ptr_target[targetID]->x;
				temp_y = ptr_target[targetID]->y;
				int actionID = rand() % available_action_count;
				int action = available_action.at(actionID);
				if(action == UP){
					temp_y--;
				}
				else if(action == DOWN){
					temp_y++;
				}
				else if(action == LEFT){
					temp_x--;
				}
				else if(action == RIGHT){
					temp_x++;
				}
				else{
					;
				}
				ptr_target[targetID]->SetPosition(temp_x, temp_y); 
			}
		}
	}
}
/*----------------Reset the energy, position of each target and the reward for each sensor.-----------------*/
void Reset(Sensor* ptr_sensor[], Target* ptr_target[])
{
	for(int sensorNum = 0;sensorNum < MAX_SENSOR_COUNT;sensorNum++){
		ptr_sensor[sensorNum]->SetInsReward(0);
		ptr_sensor[sensorNum]->SetAllInsReward(0);
		ptr_sensor[sensorNum]->SetAccReward(0);
		ptr_sensor[sensorNum]->SetAllAccReward(0);
	}
	Target::SetCount(MAX_TARGET_COUNT);
	/*The initial position of each target is a random cell. */
	vector<int> vec_pos;
	for(int pos = 0; pos < MAX_CELL_COUNT; pos++){
		vec_pos.push_back(pos);
	}
	int pos_count = MAX_CELL_COUNT;
	for(int targetID = 0; targetID < MAX_TARGET_COUNT; targetID++){
		ptr_target[targetID]->SetEnergy(MAX_ENERGY);
		int pos;
		pos = rand() % pos_count;
		ptr_target[targetID]->SetPosition(vec_pos.at(pos)); 
		vec_pos.erase(vec_pos.begin()+pos);
		pos_count--;
	}
}
void SaveData(int stateIndex, Sensor* ptr_sensor[], Target* ptr_target[], unsigned episode_id, int step)
{
	ofstream sensorNetworkFile("completeRecord.dat",ios::app);
	if(!sensorNetworkFile){
		cerr<<"File completeRecord.dat could not be opened"<<endl;
		exit(1);
	}
	sensorNetworkFile<<"Episode"<<" "<<episode_id+1<<" "<<"step"<<" "<<step+1<<"  "<<"stateIndex"<<"  "<<stateIndex<<endl;
	sensorNetworkFile.close();

	for(int sensorNum = 0;sensorNum < MAX_SENSOR_COUNT;sensorNum++){
		ptr_sensor[sensorNum]->Savedata();
	}
	for(int targetNum = 0;targetNum < MAX_TARGET_COUNT; targetNum++){
		ptr_target[targetNum]->SaveData();
	}
}
void SaveData(Target* ptr_target[])
{
	ofstream sensorNetworkFile("completeRecord.dat",ios::app);
	if(!sensorNetworkFile){
		cerr<<"File completeRecord.dat could not be opened"<<endl;
		exit(1);
	}
	sensorNetworkFile<<endl;
	sensorNetworkFile<<"Initial target position  "<<"target1 "<<ptr_target[0]->GetPosition()<<"   "<<"target2 "<<ptr_target[1]->GetPosition()<<endl;
	sensorNetworkFile.close();
}

void SaveAverageReward(double averageReward[], int episode_count, int episodePerBlock)
{
	ofstream sensorNetworkFile("averageReward.dat",ios::app);
	if(!sensorNetworkFile){
		cerr<<"File averageReward.dat could not be opened"<<endl;
		exit(1);
	}
	int block_id;
	for(block_id = 0; block_id < episode_count/episodePerBlock; block_id++){
		sensorNetworkFile<<averageReward[block_id]<<endl;
	}
	sensorNetworkFile.close();
}
void SaveAverageStep(double averageStep[], int episode_count, int episodePerBlock)
{
	ofstream sensorNetworkFile("averageStep.dat",ios::app);
	if(!sensorNetworkFile){
		cerr<<"File averageStep.dat could not be opened"<<endl;
		exit(1);
	}
	int block_id;
	for(block_id = 0; block_id < episode_count/episodePerBlock; block_id++){
		sensorNetworkFile<<averageStep[block_id]<<endl;
	}
	sensorNetworkFile.close();
}
/*-----------------Return a random number that obeys U[0,1].-----------------*/
double GetRand()
{
	double temp_randMin,temp_randMax,temp_randNumber;
    temp_randMin = 0.0;
    temp_randMax = 1.0;
    temp_randNumber = (double)rand()/RAND_MAX * (temp_randMax-temp_randMin) + temp_randMin;
	return temp_randNumber;
}
/*-----------------Initialize the related parameters of A-IQL.-----------------*/
void InitializeRLParameter(Sensor* ptr_sensor[])
{
	discountFactor = INI_DISCOUNT_FACTOR;
	thresh = INI_THRESH;
	for(int sensorID = 0; sensorID < MAX_SENSOR_COUNT; sensorID++){
		double ACT_COUNT = (ptr_sensor[sensorID]->GetActionSet()).actionCount;
		ini_epsilon[sensorID] = 1 - 1.0/ACT_COUNT - 0.1;
		epsilon[sensorID] = ini_epsilon[sensorID]; 
		for(int stateID = 0; stateID < MAX_STATE_COUNT; stateID++){
			for(int actionID = 0; actionID < ACT_COUNT; actionID++){
				Q[sensorID][stateID][(ptr_sensor[sensorID]->GetActionSet()).action[actionID]] = 0.0;
				visit[sensorID][stateID][(ptr_sensor[sensorID]->GetActionSet()).action[actionID]] = 0;
				max_his[sensorID][stateID][(ptr_sensor[sensorID]->GetActionSet()).action[actionID]] = NEG_INF;
				pi[sensorID][stateID][(ptr_sensor[sensorID]->GetActionSet()).action[actionID]] = 1.0/ACT_COUNT;	  //如果每个sensor的动作不一样，则动作数不是REAL_ACTION_COUNT，需通过Sensor的成员函数获取真正的动作数			
			}
		}
	}
	for(int stateID = 0; stateID < MAX_STATE_COUNT; stateID++){
		for(int cellID = 0;cellID < MAX_CELL_COUNT;cellID++){
			STATE_INDEX[stateID][cellID] = UNOCCUPIED;
		}
	}
	int state_index = 0;
	for(int pos = 0; pos < MAX_CELL_COUNT; pos++){
		STATE_INDEX[state_index][pos] = OCCUPIED;
		state_index++;
	}
	for(int pos1 = 0; pos1 < MAX_CELL_COUNT-1; pos1++){
		for(int pos2 = pos1+1; pos2 < MAX_CELL_COUNT; pos2++){
			STATE_INDEX[state_index][pos1] = OCCUPIED;
			STATE_INDEX[state_index][pos2] = OCCUPIED;
			state_index++;
		}
	}
	for(int pos1 = 0; pos1 < MAX_CELL_COUNT-2; pos1++){
		for(int pos2 = pos1+1; pos2 < MAX_CELL_COUNT-1; pos2++){
			for(int pos3 = pos2+1; pos3 < MAX_CELL_COUNT; pos3++){
				STATE_INDEX[state_index][pos1] = OCCUPIED;
				STATE_INDEX[state_index][pos2] = OCCUPIED;
				STATE_INDEX[state_index][pos3] = OCCUPIED;
				state_index++;
			}	
		}
	}
}
/*-----------------Update related parameters of A-IQL.-----------------*/
void UpdateRLParameter(double epsilon[], unsigned episode_id)
{
	for(int sensorID = 0; sensorID < MAX_SENSOR_COUNT; sensorID++){
		epsilon[sensorID] = ini_epsilon[sensorID] - (ini_epsilon[sensorID] - END_EPSILON)/MAX_LEARNING_COUNT * episode_id;
	}
	thresh = INI_THRESH - (INI_THRESH - END_THRESH)/MAX_LEARNING_COUNT * episode_id;
}
/*-----------------Get the state index according to the position and the number of the targets.-----------------*/
int GetStateIndex(Target* ptr_target[])
{
	//A total number of C(6,0)+C(6,1)+C(6,2)+C(6,3) = 42 states (The state encode the position and the number of targets.)
	int is_occupied[MAX_CELL_COUNT];
	for(int cellID = 0;cellID < MAX_CELL_COUNT;cellID++){
		is_occupied[cellID] = UNOCCUPIED;
	}
	for(int targetID = 0;targetID < MAX_TARGET_COUNT; targetID++){
		if(ptr_target[targetID]->GetEnergy() > 0){  
			int pos = ptr_target[targetID]->GetPosition();
			is_occupied[pos] = OCCUPIED;
		}
	}
	for(int stateID = 0; stateID < MAX_STATE_COUNT; stateID++){
		bool is_find = true;
		for(int cellID = 0;cellID < MAX_CELL_COUNT;cellID++){
			if(is_occupied[cellID] != STATE_INDEX[stateID][cellID]){
				is_find = false;
				break;
			}
		}
		if(is_find){
			/*for(int targetID = 0; targetID < MAX_TARGET_COUNT; targetID++){
				cout<<"target "<<targetID<<" "<<"pos "<<ptr_target[targetID]->GetPosition()<<endl;
			}
			cout<<"stateID "<<stateID<<endl;
			system("pause");*/
			return stateID;
		}
	}
	return -1;
}
/*-----------------The first argument is the source，and the second argument is the destination.-----------------*/
void GetJointAction(Sensor* ptr_sensor[], int jointAction[])
{
	for(int sensorID = 0; sensorID < MAX_SENSOR_COUNT; sensorID++){
		jointAction[sensorID] = ptr_sensor[sensorID]->GetAction();
	}
}
/*----------------Select an random action for each sensor.-----------------*/
void SetSensorAction(Sensor* ptr_sensor[])
{
	for(int sensorID = 0; sensorID < MAX_SENSOR_COUNT; sensorID++){
		ptr_sensor[sensorID]->SetAction( (ptr_sensor[sensorID]->GetActionSet()).action[rand()%((ptr_sensor[sensorID]->GetActionSet()).actionCount)]);
	}
}
/*-----------------Update Q-functin Q(s,ai) and pi(ai|s) for each agent i.-----------------*/
void UpdateQ(Sensor* ptr_sensor[], const short pre_stateIndex, const short cur_stateIndex, int pre_jointAction[], int step)
{
	for(int sensorID = 0; sensorID <MAX_SENSOR_COUNT; sensorID++){
		double max_Q = NEG_INF;
		for(int actionID = 0; actionID < (ptr_sensor[sensorID]->GetActionSet()).actionCount; actionID++){
			if(Q[sensorID][cur_stateIndex][(ptr_sensor[sensorID]->GetActionSet()).action[actionID]] > max_Q){
				max_Q = Q[sensorID][cur_stateIndex][(ptr_sensor[sensorID]->GetActionSet()).action[actionID]];
			}
		}
		double estimateQ_pre;
		estimateQ_pre = ptr_sensor[sensorID]->GetAllInsReward() + discountFactor * max_Q;
		if(estimateQ_pre > max_his[sensorID][pre_stateIndex][pre_jointAction[sensorID]]){
			max_his[sensorID][pre_stateIndex][pre_jointAction[sensorID]] = estimateQ_pre;
		}
		visit[sensorID][pre_stateIndex][pre_jointAction[sensorID]] = visit[sensorID][pre_stateIndex][pre_jointAction[sensorID]] + 1;
		if(visit[sensorID][pre_stateIndex][pre_jointAction[sensorID]] >= T){
			Q[sensorID][pre_stateIndex][pre_jointAction[sensorID]] = max_his[sensorID][pre_stateIndex][pre_jointAction[sensorID]];
			visit[sensorID][pre_stateIndex][pre_jointAction[sensorID]] = 0;
			max_his[sensorID][pre_stateIndex][pre_jointAction[sensorID]] = NEG_INF;
		}
		
		double pre_max_Q = NEG_INF;
		for(int actionID = 0; actionID < (ptr_sensor[sensorID]->GetActionSet()).actionCount; actionID++){
			if(Q[sensorID][pre_stateIndex][(ptr_sensor[sensorID]->GetActionSet()).action[actionID]] > pre_max_Q){
				pre_max_Q = Q[sensorID][pre_stateIndex][(ptr_sensor[sensorID]->GetActionSet()).action[actionID]];
			}
		}
		//Update pi(ai|s) if the condition in the paper is satisfied. 
		if((abs(pre_max_Q - (ptr_sensor[sensorID]->GetAllInsReward() + discountFactor * max_Q))) <= thresh){
			double ACT_COUNT = (ptr_sensor[sensorID]->GetActionSet()).actionCount;
			for(int actionID = 0; actionID < (ptr_sensor[sensorID]->GetActionSet()).actionCount; actionID++){
				if( (ptr_sensor[sensorID]->GetActionSet()).action[actionID] == pre_jointAction[sensorID]){
					pi[sensorID][pre_stateIndex][pre_jointAction[sensorID]] = 1 - epsilon[sensorID];
				}
				else{
					pi[sensorID][pre_stateIndex][(ptr_sensor[sensorID]->GetActionSet()).action[actionID]] = epsilon[sensorID]/(ACT_COUNT-1.0);
				}
			}
		}
	}
}
void SaveIniQ(Sensor* ptr_sensor[])
{
	string str_action[MAX_ACTION_COUNT] = {"FOCUS_CELL1","FOCUS_CELL2", "FOCUS_CELL3", "FOCUS_CELL4", "FOCUS_CELL5", "FOCUS_CELL6", "NO_FOCUS"};
	ofstream sensorNetworkFile("iniQ.dat",ios::app);
	if(!sensorNetworkFile){
		cerr<<"File iniQ.dat could not be opened"<<endl;
		exit(1);
	}
	for(int stateID = 0; stateID < MAX_STATE_COUNT; stateID++){
		sensorNetworkFile<<"stateID "<<stateID<<endl;
		for(int sensorID = 0; sensorID < MAX_SENSOR_COUNT; sensorID ++){
			sensorNetworkFile<<"sensor "<<sensorID<<endl;
			sensorNetworkFile<<endl;
			int ACT_COUNT = (ptr_sensor[sensorID]->GetActionSet()).actionCount;
			for(int actionID = 0;actionID < ACT_COUNT; actionID++){
				int action = (ptr_sensor[sensorID]->GetActionSet()).action[actionID];
				sensorNetworkFile<<str_action[action]<<":  "<<Q[sensorID][stateID][action]<<endl;
			}
			sensorNetworkFile<<endl;
		}
	}
	sensorNetworkFile.close();
}
void SaveQ(Sensor* ptr_sensor[])
{
	string str_action[MAX_ACTION_COUNT] = {"FOCUS_CELL1","FOCUS_CELL2", "FOCUS_CELL3", "FOCUS_CELL4", "FOCUS_CELL5", "FOCUS_CELL6", "NO_FOCUS"};
	ofstream sensorNetworkFile("Q.dat",ios::app);
	if(!sensorNetworkFile){
		cerr<<"File Q.dat could not be opened"<<endl;
		exit(1);
	}
	for(int stateID = 0; stateID < MAX_STATE_COUNT; stateID++){
		sensorNetworkFile<<"stateID "<<stateID<<endl;
		for(int sensorID = 0; sensorID < MAX_SENSOR_COUNT; sensorID ++){
			sensorNetworkFile<<"sensor "<<sensorID<<endl;
			sensorNetworkFile<<endl;
			int ACT_COUNT = (ptr_sensor[sensorID]->GetActionSet()).actionCount;
			for(int actionID = 0;actionID < ACT_COUNT; actionID++){
				int action = (ptr_sensor[sensorID]->GetActionSet()).action[actionID];
				sensorNetworkFile<<str_action[action]<<":  "<<Q[sensorID][stateID][action]<<endl;
			}
			sensorNetworkFile<<endl;
		}
	}
	sensorNetworkFile.close();
}
void SavePi(Sensor* ptr_sensor[])
{
	string str_action[MAX_ACTION_COUNT] = {"FOCUS_CELL1","FOCUS_CELL2", "FOCUS_CELL3", "FOCUS_CELL4", "FOCUS_CELL5", "FOCUS_CELL6", "NO_FOCUS"};
	ofstream sensorNetworkFile("Pi.dat",ios::app);
	if(!sensorNetworkFile){
		cerr<<"File Pi.dat could not be opened"<<endl;
		exit(1);
	}
	for(int stateID = 0; stateID < MAX_STATE_COUNT; stateID++){
		sensorNetworkFile<<"stateID "<<stateID<<endl;
		for(int sensorID = 0; sensorID < MAX_SENSOR_COUNT; sensorID ++){
			sensorNetworkFile<<"sensor "<<sensorID<<endl;
			sensorNetworkFile<<endl;
			int ACT_COUNT = (ptr_sensor[sensorID]->GetActionSet()).actionCount;
			for(int actionID = 0;actionID < ACT_COUNT; actionID++){
				int action = (ptr_sensor[sensorID]->GetActionSet()).action[actionID];
				sensorNetworkFile<<str_action[action]<<":  "<<pi[sensorID][stateID][action]<<endl;
			}
			sensorNetworkFile<<endl;
		}
	}
	sensorNetworkFile.close();
}
