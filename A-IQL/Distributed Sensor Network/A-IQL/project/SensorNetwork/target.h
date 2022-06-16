//target.h
//prevent multiple inclusions of header file

#ifndef  TARGET_H
#define TARGET_H
//The action of each target
const int UP    = 0;
const int DOWN  = 1;
const int LEFT  = 2;
const int RIGHT = 3;
const int OUT = -1;
//extern Position posDefault;
class Target {

public:
	//function members
	Target(short formal_energy = 3, short formal_pos = 0);
	short GetEnergy() const;
	void SetEnergy(short formal_energy);
	void DecEnergy();
	static short GetCount();
	static void SetCount(short formal_count);
	static void DecCount();
	short GetPosition() const;
	void SetPosition(short formal_pos);
	void SetPosition(const int formal_x, const int formal_y);
    void Print() const;
	void SaveData() const;
	short GetAction() const;
	void SetAction(short formal_action);

	//data members
	const short index;      //the index of a target
	int x;                  // the x of the target 
	int y;                  // the y of the target 
	
private:
	short energy;           // the initial energy value is 3
	short pos;              // the position of the target
	static short count;     // the number of targets
	short action;           // the action of the target

};
#endif