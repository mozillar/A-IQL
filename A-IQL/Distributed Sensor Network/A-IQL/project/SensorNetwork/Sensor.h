//Sensor.h
#ifndef SENSOR_H
#define SENSOR_H
#include "SensorActionSet.h"
const int MAX_STATE_COUNT = 42;
const int MAX_ACTION_COUNT = 7;
const int MAX_STEP_COUNT = 15;

// the action of all sensors
const short FOCUS_CELL1 = 0;
const short FOCUS_CELL2 = 1;
const short FOCUS_CELL3 = 2;
const short FOCUS_CELL4 = 3;
const short FOCUS_CELL5 = 4;
const short FOCUS_CELL6 = 5;
const short NO_FOCUS    = 6;
const short FOCUS_OUT = 7;   // not used

const short MAX_NEIGHBOR = 5;



class Sensor {

public:
	//function members
	Sensor( short formal_neighborCount, short formal_action = NO_FOCUS, double formal_reword = 0.0 );
	short GetNeighborCount() const;
	static short GetCount();
	//static void SetCount(short formal_count);
	short GetAction() const;
	void SetAction(short formal_action);
	double GetInsReward() const;
	void SetInsReward(double formal_ins_reward);
	double GetAccReward() const;
    void SetAccReward(double formal_acc_reward);
	static double GetAllInsReward();
	static void SetAllInsReward(double formal_all_ins_reward);
	static double GetAllAccReward();
	static void SetAllAccReward(double formal_all_acc_reward);
	SensorActionSet GetActionSet() const;
	void SetActionSet(SensorActionSet formal_actionSet);
	void Print() const;
	void Savedata() const;
	//data menbers
	const short index;
	Sensor* ptr_neighbor[MAX_NEIGHBOR];
	
private:
	//data members
	short action;                   // the action set of the sensor 
	const short neighborCount;      // the number of neighbor sensors of the sensor 
	static short count;             // the number of sensors
	double ins_reward;              // immediate reward
	double acc_reward;				// accumulated reward (cumulative reward )
	static double all_ins_reward;   // immediate reward of all sensors
	static double all_acc_reward;   // accumulated reward of all sensors
	SensorActionSet actionSet;      // the action set of the sensor 
};

#endif