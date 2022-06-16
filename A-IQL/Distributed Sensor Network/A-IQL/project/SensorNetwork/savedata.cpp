#include "savedata.h"
#include <fstream>
#include <iostream>
using namespace std;

void SaveData(int a, const string& str_file, const string& op)
{
	const char* c_file = str_file.c_str();
	ofstream file;
	if(op == "app"){
		file.open(c_file, ios::app);
	}
	else if(op == "out"){
		file.open(c_file, ios::out);
	}
	else{
		;
	}
	if(!file){
		cerr<<"SaveData error File could not be opened"<<endl;
	}
	file<<a<<endl;
	file.close();
}
void SaveData(double a, const string& str_file, const string& op)
{
	const char * c_file = str_file.c_str();
	ofstream file;
	if(op == "app"){
		file.open(c_file, ios::app);
	}
	else if(op == "out"){
		file.open(c_file, ios::out);
	}
	else{
		;
	}
	if(!file){
		cerr<<"SaveData error File could not be opened"<<endl;
	}
	file<<a<<endl;
	file.close();
}
void SaveData(int my_array[], int cout, const string& str_file, const string& op)
{
	const char * c_file = str_file.c_str();
	ofstream file;
	if(op == "app"){
		file.open(c_file, ios::app);
	}
	else if(op == "out"){
		file.open(c_file, ios::out);
	}
	else{
		;
	}
	if(!file){
		cerr<<"SaveData error File could not be opened"<<endl;
	}
	int id;
	for(id = 0; id < cout; id++){
		file<<my_array[id]<<endl;
	}
	file.close();
}
void SaveData(double my_array[], int cout, const string& str_file, const string& op)
{
	const char * c_file = str_file.c_str();
	ofstream file;
	if(op == "app"){
		file.open(c_file, ios::app);
	}
	else if(op == "out"){
		file.open(c_file, ios::out);
	}
	else{
		;
	}
	if(!file){
		cerr<<"SaveData error File could not be opened"<<endl;
	}
	int id;
	for(id = 0; id < cout; id++){
		file<<my_array[id]<<endl;
	}
	file.close();
}