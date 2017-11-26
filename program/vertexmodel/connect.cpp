#pragma once
#include "data_SN.cpp"
#include "atom.cpp"

class CoD:public DaD
{
private:
	int ati[2][2];
	double atr[2][3],atd[2][3];
	double spring[3];
	
	int id;
	double epsilon;
	
public:
	CoD();
	~CoD();
	CoD(const CoD& cod);
	CoD& operator=(const CoD& cod);
	////////////////////////////////////Show
	int Show_Atom(int ni,int nl);
	double Show_Mr(int ni,int nk);
	////////////////////////////////////Initialize
	void Connect(int n00,int n01,
				 int n10,int n11,
				 int bo1,int bo2);
	////////////////////////////////////Simulate
	void Spring(double d00,double d01,double d02,
				double d10,double d11,double d12);
};


CoD::CoD()
{
	for(i=0;i<2;i++)
	{
		for(k=0;k<3;k++)
		atr[i][k]=0.0;
	}
	epsilon=epsilon_m;
}

CoD::~CoD()
{
}

CoD::CoD(const CoD& cod)//すべてのコピーコンストラクタを調査
{
	for(h=0;h<2;h++)
	{
		for(i=0;i<2;i++)
		ati[h][i]=cod.ati[h][i];
		for(k=0;k<3;k++)
		{
			atr[h][k]=cod.atr[h][k];
			atd[h][k]=cod.atd[h][k];
		}
	}
	for(k=0;k<3;k++)
	spring[k]=cod.spring[k];
	
	id=cod.id;
	epsilon=cod.epsilon;
}

CoD& CoD::operator=(const CoD& cod)
{
	for(h=0;h<2;h++)
	{
		for(i=0;i<2;i++)
		ati[h][i]=cod.ati[h][i];
		for(k=0;k<3;k++)
		{
			atr[h][k]=cod.atr[h][k];
			atd[h][k]=cod.atd[h][k];
		}
	}
	for(k=0;k<3;k++)
	spring[k]=cod.spring[k];
	
	id=cod.id;
	epsilon=cod.epsilon;
}

int CoD::Show_Atom(int ni,int nl)
{
	if(!(ni>=0 && ni<2))
	cout<<"Caution!! In CoD class, at Show_Atom(int ni,int nl), ni is out of range"<<endl;
	
	if(!(nl>=0 && nl<2))
	cout<<"Caution!! In CoD class, at Show_Atom(int ni,int nl), nl is out of range"<<endl;
	return ati[ni][nl];
}

double CoD::Show_Mr(int ni,int nk)
{
	/*
	double ans;
	if(ni==0)
	ans=spring[nk];
	else if(ni==1)
	ans=-spring[nk];
	return ans;
	*/
	return atr[ni][nk];
}
////////////////////////////////////////////////////////////////////////////////

//Initialize

////////////////////////////////////////////////////////////////////////////////
void CoD::Connect(int n00,int n01,
				  int n10,int n11,
				  int bo0,int bo1)
{
	ati[0][0]=n00;
	ati[0][1]=n01;
	ati[1][0]=n10;
	ati[1][1]=n11;
	
	if(bo0!=bo1)
	epsilon=epsilon_b;
	else if(bo0)
	epsilon=epsilon_p;
	else
	epsilon=epsilon_m;
}
////////////////////////////////////////////////////////////////////////////////

//Simulate

////////////////////////////////////////////////////////////////////////////////
void CoD::Spring(double d00,double d01,double d02,
				 double d10,double d11,double d12)
{
	atd[0][0]=d00;
	atd[0][1]=d01;
	atd[0][2]=d02;
	atd[1][0]=d10;
	atd[1][1]=d11;
	atd[1][2]=d12;
	
	for(k=0;k<3;k++)
	{
		atr[0][k]=0.0;
		atr[1][k]=0.0;
	}
	
	double distance_between_atoms=sqrt((atd[0][0]-atd[1][0])*(atd[0][0]-atd[1][0])
									  +(atd[0][1]-atd[1][1])*(atd[0][1]-atd[1][1])
									  +(atd[0][2]-atd[1][2])*(atd[0][2]-atd[1][2]));
	for(k=0;k<3;k++)
	spring[k]=epsilon*(sen_L/distance_between_atoms-1.0)*(atd[0][k]-atd[1][k]);
	
	for(k=0;k<3;k++)
	{
		atr[0][k]+=spring[k];
		atr[1][k]-=spring[k];
	}
}
