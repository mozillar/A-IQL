#ifndef SAVEDATA_H
#define SAVEDATA_H
#include <string>
using namespace std;
void SaveData(int a, const string& str_file, const string& op = "app");
void SaveData(double a, const string& str_file, const string& op = "app");
void SaveData(int my_array[], int cout, const string& str_file, const string& op = "app");
void SaveData(double my_array[], int cout, const string& str_file, const string& op = "app");
#endif
