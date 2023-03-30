#include <iostream>
#include <random>
#include <fstream>
#include "ran_class.h"
using namespace std;
#define PI 3.14159265
#define N 10000


int main()
{
	ran_num Entity;
	Entity.Guassian();//正态分布
	Entity.exp_distribution();//指数分布
	Entity.GGD_c_is_one();
	Entity.GGD_c_is_halfone();
	Entity.GGD_distribution(); 
	Entity.parameter_estimation();
	Entity.Theotical_CDF();
	
	return 0;
}