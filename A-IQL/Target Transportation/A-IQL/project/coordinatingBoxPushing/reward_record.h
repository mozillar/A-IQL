#ifndef RERWARD_RECORD_H
#define RERWARD_RECORD_H

//#include <stdio.h>
#include <vector>
using namespace std;

const int MAX_ACTION_COUNT = 3;
class RewardRecord {
public:
	RewardRecord();
	int stateIndex;
	vector<double> action[MAX_ACTION_COUNT];

	


};
#endif