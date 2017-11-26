#include "data_PF.cpp"
using namespace std;

class Ran
{
	double mean,standard_deviation;
	double U1,U2;
	
public:
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