//Worker.cpp
#include "Worker.h"
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <math.h>
using namespace std;
Worker::Worker()
{
	SetInsReward(0.0);
	SetAccReward(0.0);
	SetGrasp(FREE);
	SetAction(-1);
}
int Worker::GetAction() const
{
	return action;
}
void Worker::SetAction(int formal_action)
{
	action = formal_action;
}
double Worker::GetInsReward() const
{
	return ins_reward;
}
void Worker::SetInsReward(double formal_ins_reward)
{
	ins_reward = formal_ins_reward;
}
double Worker::GetAccReward() const
{
	return acc_reward;
}
void Worker::SetAccReward(double formal_acc_reward)
{
	acc_reward = formal_acc_reward;
}
void Worker::Reset()
{
	SetInsReward(0.0);
	SetAccReward(0.0);
	SetGrasp(FREE);
	SetAction(-1);
}
/*-----------------Return a random number that obeys U[0,1].-----------------*/
double Worker::GetRand()
{
	double temp_randMin,temp_randMax,temp_randNumber;
    temp_randMin = 0.0;
    temp_randMax = 1.0;
    temp_randNumber = (double)rand()/RAND_MAX * (temp_randMax-temp_randMin) + temp_randMin;
	return temp_randNumber;
}
void Worker::GetState(State & formal_state)
{
	formal_state.x1 = state.x1;
	formal_state.y1 = state.y1;
	formal_state.x2 = state.x2;
	formal_state.y2 = state.y2;
	formal_state.grasp1 = state.grasp1;
	formal_state.grasp2 = state.grasp2;
}
void Worker::SetState(State formal_state)
{
	state.x1 = formal_state.x1;
	state.y1 = formal_state.y1;
	state.x2 = formal_state.x2;
	state.y2 = formal_state.y2;
	state.grasp1 = formal_state.grasp1;
	state.grasp2 = formal_state.grasp2;
}
int Worker::GetStateIndex() const
{
	return state_index;
}
void Worker::SetStateIndex(int formal_state_index)
{
	state_index = formal_state_index;
}
int Worker::GetPreStateIndex() const
{
	return pre_state_index;
}
void Worker::SetPreStateIndex(int formal_pre_state_index)
{
	pre_state_index = formal_pre_state_index;
}
int Worker::GetCurStateIndex() const
{
	return cur_state_index;
}
void Worker::SetCurStateIndex(int formal_cur_state_index)
{
	cur_state_index = formal_cur_state_index;
}
double Worker::GetX() const
{
	return x;
}
void Worker::SetX(double formal_x)
{
	x = formal_x;
}
double Worker::GetY() const
{
	return y;
}
void Worker::SetY(double formal_y)
{
	y = formal_y;
}
double Worker::GetPreX() const
{
	return pre_x;
}
void Worker::SetPreX(double formal_pre_x)
{
	pre_x = formal_pre_x;
}
double Worker::GetPreY() const
{
	return pre_y;
}
void Worker::SetPreY(double formal_pre_y)
{
	pre_y = formal_pre_y;
}
double Worker::GetCurX() const
{
	return cur_x;
}
void Worker::SetCurX(double formal_cur_x)
{
	cur_x = formal_cur_x;
}
double Worker::GetCurY() const
{
	return cur_y;
}
void Worker::SetCurY(double formal_cur_y)
{
	cur_y = formal_cur_y;
}
int  Worker::GetGrasp() const
{
	return grasp;
}
void Worker::SetGrasp(int formal_grasp)
{
	grasp = formal_grasp;
}
int Worker::GetPreGrasp() const
{
	return pre_grasp;
}
void Worker::SetPreGrasp(int formal_pre_grasp)
{
	pre_grasp = formal_pre_grasp;
}
int Worker::GetCurGrasp() const
{
	return cur_grasp;
}
void Worker::SetCurGrasp(int formal_cur_grasp)
{
	cur_grasp = formal_cur_grasp;
}


