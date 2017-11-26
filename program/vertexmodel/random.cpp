#include "data_PF.cpp"
/*
#include <iostream>  // for cout
#include <math.h>    // for cos, sin
#include <cstdlib>   // for rand()
#include <stdio.h>   // for printf()
#include <time.h>
#define Pi 3.14159265358979323
*/
using namespace std;

class Ran
{
	// 各種定数
	double mean,standard_deviation;
	double U1,U2;// 一様乱数
	
public:
	// コンストラクタ
	Ran();
	double Gaussian_Distribution();
	double Gaussian_Distribution(double n_mean,double n_standard_deviation);
	
};

Ran::Ran()
{
	srand((unsigned int)time(NULL));
	mean=0;
	standard_deviation=1.0;
}

double Ran::Gaussian_Distribution()
{
	U1=rand()/RAND_MAX;
	U2=rand()/RAND_MAX;
	return standard_deviation*sqrt(-2.0*log(U1))*cos(2.0*Pi*U2)+mean;
}

double Ran::Gaussian_Distribution(double n_mean,double n_standard_deviation)
{
	U1=((double)rand())/((double)RAND_MAX);
	U2=((double)rand())/((double)RAND_MAX);
	
	return n_standard_deviation*sqrt(-2.0*log(U1))*cos(2.0*Pi*U2)+n_mean;
}
/*
int main()
{
	Ran ran;
	cout<<"Gaussian_Distribution1="<<ran.Gaussian_Distribution(3.0,2.0)<<endl;
	cout<<"Gaussian_Distribution2="<<ran.Gaussian_Distribution(3.0,2.0)<<endl;
	cout<<"Gaussian_Distribution3="<<ran.Gaussian_Distribution(3.0,2.0)<<endl;
	cout<<"Gaussian_Distribution4="<<ran.Gaussian_Distribution(3.0,2.0)<<endl;
	cout<<"Gaussian_Distribution5="<<ran.Gaussian_Distribution(3.0,2.0)<<endl;
	cout<<"Gaussian_Distribution6="<<ran.Gaussian_Distribution(3.0,2.0)<<endl;
	cout<<"Gaussian_Distribution7="<<ran.Gaussian_Distribution(3.0,2.0)<<endl;
	cout<<"Gaussian_Distribution8="<<ran.Gaussian_Distribution(3.0,2.0)<<endl;
	cout<<"Gaussian_Distribution9="<<ran.Gaussian_Distribution(3.0,2.0)<<endl;
	cout<<"Gaussian_Distribution10="<<ran.Gaussian_Distribution(3.0,2.0)<<endl;
	
	Ran ran1;
	cout<<"Gaussian_Distribution1="<<ran.Gaussian_Distribution(3.0,2.0)<<endl;
	cout<<"Gaussian_Distribution2="<<ran.Gaussian_Distribution(3.0,2.0)<<endl;
	cout<<"Gaussian_Distribution3="<<ran.Gaussian_Distribution(3.0,2.0)<<endl;
	cout<<"Gaussian_Distribution4="<<ran.Gaussian_Distribution(3.0,2.0)<<endl;
	cout<<"Gaussian_Distribution5="<<ran.Gaussian_Distribution(3.0,2.0)<<endl;
	cout<<"Gaussian_Distribution6="<<ran.Gaussian_Distribution(3.0,2.0)<<endl;
	cout<<"Gaussian_Distribution7="<<ran.Gaussian_Distribution(3.0,2.0)<<endl;
	cout<<"Gaussian_Distribution8="<<ran.Gaussian_Distribution(3.0,2.0)<<endl;
	cout<<"Gaussian_Distribution9="<<ran.Gaussian_Distribution(3.0,2.0)<<endl;
	cout<<"Gaussian_Distribution10="<<ran.Gaussian_Distribution(3.0,2.0)<<endl;
	
	return 0;
}
*/