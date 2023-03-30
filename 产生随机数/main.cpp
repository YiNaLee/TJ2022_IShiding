#include <iostream>
#include <random>
#include <fstream>
using namespace std;
//template <class RealType = double> class uniform_real_distribution;
int main()
{
	fstream Ramdomoutfile("Random_.txt", ios::out);
	random_device rd;
	mt19937_64 gen(rd());
	uniform_real_distribution<double> dis(0, 1);  
	double x = dis(gen);
	double y = dis(gen);
	/*for (int i = 0; i < 10000; i++) {
		double a = dis(gen);
		
		//double res = log(1 - a);
		Ramdomoutfile << a << endl;
	}*/
	cout << x << endl;
	cout << y << endl;
	
	return 0;
}