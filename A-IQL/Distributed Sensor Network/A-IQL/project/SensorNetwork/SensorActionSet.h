//SensorActionSet.h
#ifndef SENSORACTIONSET_H
#define SENSORACTIONSET_H
const short MAX_SENSORACTIONCOUNT = 7;
struct SensorActionSet{
	short action[MAX_SENSORACTIONCOUNT];
	short actionCount;
};
#endif