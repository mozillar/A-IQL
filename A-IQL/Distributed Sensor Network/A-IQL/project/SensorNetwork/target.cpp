//target.cpp
#include "target.h"
#include <math.h>
#include <fstream>
#include <string> 
#include <iostream>

using namespace std;

short Target::count = 0;


Target::Target(short formal_energy, short formal_pos):index(count)
{
	SetEnergy(formal_energy);
	SetPosition(formal_pos);
	count++; 
}
short Target::GetEnergy() const
{
	return energy;
}
void Target::SetEnergy(short formal_energy)
{
	energy = formal_energy;
	if(energy <= 0){
		energy = 0;
	}
	else if(energy >=3 ){
		energy = 3;
	}
	else{
		;
	}
}
void Target::DecEnergy()
{
	energy--;
	SetEnergy(energy);
}
/*-------------------get the number of targets-------------------*/
short Target::GetCount()
{
	return count;
}
/*-------------------set the number of targets-------------------*/
void Target::SetCount(short formal_count)
{
	count = formal_count;
	if(count <= 0){
		count = 0;
	}
}
void Target::DecCount()
{
	count--;
	SetCount(count);
}
/*------------------get the position of the target-------------------*/
short Target::GetPosition() const
{
	return pos;
}
/*------------------set the position of the target-------------------*/
void Target::SetPosition(short formal_pos)
{
	pos = formal_pos;
	if(pos == 0){
		x = 1;
		y = 1;
	}
	else if(pos == 1){
		x = 2;
		y = 1;
	}
	else if(pos == 2){
		x = 3;
		y = 1;
	}
	else if(pos == 3){
		x = 1;
		y = 2;
	}
	else if(pos == 4){
		x = 2;
		y = 2;
	}
	else if(pos == 5){
		x = 3;
		y = 2;
	}
	else{
		x = -1;
		y = -1;
		;
	}
}
/*------------------set the position of the target-------------------*/
void Target::SetPosition(const int formal_x, const int formal_y)   
{
	x = formal_x;
	y = formal_y;
	if(x == 1 && y == 1){
		pos = 0;
	}
	else if(x == 2 && y == 1){
		pos = 1;
	}
	else if(x == 3 && y == 1){
		pos = 2;
	}
	else if(x == 1 && y == 2){
		pos = 3;
	}
	else if(x == 2 && y == 2){
		pos = 4;
	}
	else if(x == 3 && y == 2){
		pos = 5;
	}
	else{
		pos = OUT;
	}
}
/*------------------print information for debug-------------------*/
void Target::Print() const
{
	cout<<"Target"<<" "<<index<<" "<<"energy"<<" "<<energy<<" "<<"action"<<" "<<action<<" "<<"position"<<" "<<pos<<" "<<"totalCount"<<" "<<count<<endl;
}
void Target::SaveData() const
{
	ofstream sensorNetworkFile("TargetRecord.dat",ios::app);
	if(!sensorNetworkFile){
		cerr<<"File completeRecord.dat could not be opened"<<endl;
		exit(1);
	}
	string actionName;
	if(action == UP){
		actionName = "up";
	}
	else if(action == DOWN){
		actionName = "down";
	}
	else if(action == LEFT){
		actionName = "left";
	}
	else if(action == RIGHT){
		actionName = "right";
	}
	else{
		;
	}
	sensorNetworkFile<<"Target"<<" "<<index<<" "<<"energy"<<" "<<energy<<" "<<"actionName"<<" "<<actionName<<" "<<"position"<<" "<<pos<<" "<<"totalCount"<<" "<<count<<endl;
	sensorNetworkFile.close();
}
/*-------------------get the action of the target-------------------*/
short Target::GetAction() const
{
	return action;
}
/*-------------------set the action of the target-------------------*/
void Target::SetAction(short formal_action)
{
	action = formal_action;
}
