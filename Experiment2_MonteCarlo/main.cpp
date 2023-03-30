#include "MonteCarlo_simulation.h"
#include<fstream>
#include<iostream>
using namespace std;
#define __NO_ATTACK__ 0
#define __ATTACK__ 1
int main()
{
	MC M;
	ofstream outfile("res.txt", ios::out);
	cout << "将已生成的文件存放在res.txt中，可直接查看\n";
	cout << "debug,pfa_theoretical:\n";
	for (int i = 0; i < BIN; i++) {
		cout << M.pfa_theoretical[i] << endl;
	}
	outfile << "pfa_theoretical:" << "\n";
	
	for (int i = 0; i < BIN; i++) {
		outfile<<M.pfa_theoretical[i]<<"\n";
	}
	//计算检测阈值 根据公式φ=[σ/sqrt(N)]*Q-1pfa
	outfile << "\ndetection threshold(Φ):\n";
	for (int i = 0; i < BIN; i++) {
		M.phi[i] = M.cal_phi(M.pfa_theoretical[i]);
		outfile << M.phi[i] << " ";
	}
	//计算理论的pm:
	//pm理论值计算公式：课件SS-ch-doc.pdf p8
	/************************无攻击情况下：**********************************/
	if (__NO_ATTACK__) {
		outfile << "\npm_theoretical:\n";
		for (int i = 0; i < BIN; i++) {
			M.pm_theoretical[i] = M.cal_pmtheory(M.pfa_theoretical[i]);
			outfile << M.pm_theoretical[i] << "\n";
		}
		outfile << "\npfa_experimantal:\n";
		//测试无攻击情况：c=2.0 a=1.8
		//c=1.0 a=1.8
		M.test_no_attack();
		for (int i = 0; i < BIN; i++) {
			outfile << M.pfa_experimental[i] << "\n";
		}
		outfile << "\npm_experimantal:\n";
		for (int i = 0; i < BIN; i++) {
			outfile << M.pm_experimental[i] << "\n";
		}
		outfile << endl;
		cout << "Debug:\n";
		for (int i = 0; i < N; i++) {

			cout << M.u_S[i] << endl;
		}
	}
	/*****************************************有攻击情况下*********************************/
	if (__ATTACK__) {//STDV=2.5
		outfile << "\npm_Attack_theoretical:\n";
		for (int i = 0; i < BIN; i++) {
			M.pm_theoretical[i] = M.cal_attack_pmtheory(M.pfa_theoretical[i],1.8,5.0);
			outfile << M.pm_theoretical[i] << "\n";
		}
		outfile << "\npfa_Attack_experimantal:\n";
		M.test_attack(1.8,2.5);
		for (int i = 0; i < BIN; i++) {
			outfile << M.pfa_experimental[i] << "\n";
		}
		outfile << "\npm_Attack_experimantal:\n";
		for (int i = 0; i < BIN; i++) {
			outfile << M.pm_experimental[i] << "\n";
		}
		outfile << endl;
		cout << "Debug:\n";
		for (int i = 0; i < N; i++) {

			cout << M.u_S[i] << endl;
		}
	}
	return 0;
}
//>result.txt