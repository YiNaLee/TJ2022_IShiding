#pragma once
#include<math.h>
#include<iostream>
#define N 8
#define PI 3.1415926

class DCT {
public:
	double input[N][N];//ԭʼ
	double dctres[N][N];//���任���
	double idctres[N][N];//��任���

	void DCT_transform();//���任
	//DCT_inverse�������ܣ��������DCT.dctresͨ��DCT��任ת��DCT.idctres
	void DCT_inverse();//��任
	int around(double a);//����
};