#pragma once
#include<math.h>
#include<iostream>
#define N 8
#define PI 3.1415926

class DCT {
public:
	double input[N][N];//原始
	double dctres[N][N];//正变换结果
	double idctres[N][N];//逆变换结果

	void DCT_transform();//正变换
	//DCT_inverse函数功能：把类变量DCT.dctres通过DCT逆变换转到DCT.idctres
	void DCT_inverse();//逆变换
	int around(double a);//修正
};