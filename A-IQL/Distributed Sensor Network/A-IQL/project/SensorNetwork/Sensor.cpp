//Sensor.cpp
#include "Sensor.h"
#include "SensorActionSet.h"
#include <fstream>
using std::ofstream;
using std::ifstream;
#include <iostream>
using std::cout;
using std::cin;
using std::ios;
using std::endl;
using std::cerr;

short Sensor::count = 0;
double Sensor::all_ins_reward = 0.0;
double Sensor::all_acc_reward = 0.0;
Sensor::Sensor( short formal_neighborCount, short formal_action, double formal_reword )
    :neighborCount(formal_neighborCount), index(count)
{
	for(int actionID = 0; actionID < MAX_SENSORACTIONCOUNT; actionID++){
		actionSet.action[actionID] = -1;
	}
	SetAction(formal_action);
	SetInsReward(0.0);
	SetAccReward(0.0);
	count++;
}
/*-------------------get the number of neighbor sensors of the sensor-------------------*/
short Sensor::GetNeighborCount() const
{
	return neighborCount;
}
/*-------------------get the number of sensors-------------------*/
short Sensor::GetCount()
{
	return count;
}
/*-------------------get the action of the sensor-------------------*/
short Sensor::GetAction() const
{
	return action;
}
/*-------------------set the action of the sensor-------------------*/
void Sensor::SetAction(short formal_action)
{
	action = formal_action;
}
/*-------------------get the immediate reward-------------------*/
double Sensor::GetInsReward() const
{
	return ins_reward;
}
/*-------------------set the immediate reward------------------*/
void Sensor::SetInsReward(double formal_ins_reward)
{
	ins_reward = formal_ins_reward;
}
/*------------------get the cumulative reward-------------------*/
double Sensor::GetAccReward() const
{
	return acc_reward;
}
/*-------------------set the cumulative reward-------------------*/
void Sensor::SetAccReward(double formal_acc_reward)
{
	acc_reward = formal_acc_reward;
}
/*-------------------get the immediate reward of all sensors-------------------*/
double Sensor::GetAllInsReward()
{
	return all_ins_reward;
}
/*-------------------set the immediate reward of all sensors-------------------*/
void Sensor::SetAllInsReward(double formal_all_ins_reward)
{
	all_ins_reward = formal_all_ins_reward;
}
/*-------------------get the cumulative reward of all sensors-------------------*/
double Sensor::GetAllAccReward()
{
	return all_acc_reward;
}
/*-------------------set the cumulative reward of all sensors-------------------*/
void Sensor::SetAllAccReward(double formal_all_acc_reward)
{
	all_acc_reward = formal_all_acc_reward;
}
/*-------------------get the action set for the sensor-------------------*/
SensorActionSet Sensor::GetActionSet() const
{
	return actionSet;
}
/*-------------------set the action set for the sensor-------------------*/
void Sensor::SetActionSet(SensorActionSet formal_actionSet)
{
	actionSet = formal_actionSet;
}
/*-------------------print information for debug-------------------*/
void Sensor::Print() const
{
	cout<<"Sensor"<<" "<<index<<" "<<"action"<<" "<<action<<" "<<"neighborCount"<<" "<<neighborCount\
		 <<" "<<"ins_reward"<<" "<<ins_reward<<" "<<"acc_reward"<<" "<<acc_reward<<" "<<"all_ins_reward"<<" "<<all_ins_reward\
		 <<" "<<"all_acc_reward"<<" "<<all_acc_reward<<endl;
}
/*-------------------print information at each step in each episode-------------------*/
void Sensor::Savedata() const
{
	ofstream sensorNetworkFile("SensorRecord.dat",ios::app);
	if(!sensorNetworkFile){
		cerr<<"File Record.dat could not be opened"<<endl;
		exit(1);
	}
	sensorNetworkFile<<"Sensor"<<" "<<index<<" "<<"action"<<" "<<action<<" "\
		 <<" "<<"ins_reward"<<" "<<ins_reward<<" "<<"acc_reward"<<" "<<acc_reward<<" "<<"all_ins_reward"<<" "<<all_ins_reward\
		 <<" "<<"all_acc_reward"<<" "<<all_acc_reward<<endl;
	sensorNetworkFile.close();
}