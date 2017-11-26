#pragma once
#include "data_SN.cpp"
#include "atom.cpp"

class NoD:public DaD
{
private:
	
public:
	int ati[4][2];//三角格子の4点のインデックス、N字型に点を取る、(i,j)
	double atr[4][3],atd[2][3][3];
	double nd[2][3];//01ベクトルと、02ベクトルの外積ベクトルと、それを規格化した法線ベクトル
	double D[2][3][3][3];//i番目のatomからj番目のatomへのベクトルのk番目の成分
	double L[2];//外積ベクトルの長さ
	
	int id;
	double kappa;
	
	NoD();
	~NoD();
	NoD(const NoD& nod);
	NoD& operator=(const NoD& nod);
	////////////////////////////////////Show
	int Show_Atom(int ni,int nl);
	double Show_Mr(int ni,int nk);
	////////////////////////////////////Initialize
	void Norm(int n00,int n01,
			  int n10,int n11,
			  int n20,int n21,
			  int n30,int n31,
			  int bo0,int bo1);
	void Which_Character(bool bo);
	////////////////////////////////////Calculate
	void Parallel(double d00,double d01,double d02,
				  double d10,double d11,double d12,
				  double d20,double d21,double d22,
				  double d30,double d31,double d32);
	void Calculate();
	double Dif(int nH,int nh,int nl,int ni, int nk);
};


NoD::NoD()
{
	for(i=0;i<4;i++)
	{
		for(j=0;j<2;j++)
		ati[i][j]=0;
		for(k=0;k<3;k++)
		atr[i][k]=0.0;
	}
	for(h=0;h<2;h++)
	{
		for(i=0;i<3;i++)
		{
			nd[h][i]=0.0;
			for(j=0;j<3;j++)
			{
				atd[h][i][j]=0.0;
				for(k=0;k<3;k++)
				D[h][i][j][k]=0.0;
			}
		}
	}
	kappa=kappa_m;
}

NoD::~NoD()
{
}

NoD::NoD(const NoD& nod)
{
	//cout<<"norm class copy constructa"<<endl;
	for(i=0;i<4;i++)
	{
		for(j=0;j<2;j++)
		ati[i][j]=nod.ati[i][j];
		for(j=0;j<3;j++)
		atr[i][j]=nod.atr[i][j];
	}
	for(h=0;h<2;h++)
	{
		for(i=0;i<3;i++)
		{
			nd[h][i]=nod.nd[h][i];
			for(j=0;j<3;j++)
			{
				atd[h][i][j]=nod.atd[h][i][j];
				for(k=0;k<3;k++)
				D[h][i][j][k]=nod.D[h][i][j][k];
			}
		}
		L[h]=nod.L[h];
	}
	
	id=nod.id;
	kappa=nod.kappa;
}

NoD& NoD::operator=(const NoD& nod)
{
	//cout<<"norm class operator="<<endl;
	for(i=0;i<4;i++)
	{
		for(j=0;j<2;j++)
		ati[i][j]=nod.ati[i][j];
		for(j=0;j<3;j++)
		atr[i][j]=nod.atr[i][j];
	}
	for(h=0;h<2;h++)
	{
		for(i=0;i<3;i++)
		{
			nd[h][i]=nod.nd[h][i];
			for(j=0;j<3;j++)
			{
				atd[h][i][j]=nod.atd[h][i][j];
				for(k=0;k<3;k++)
				D[h][i][j][k]=nod.D[h][i][j][k];
			}
		}
		L[h]=nod.L[h];
	}
	
	id=nod.id;
	kappa=nod.kappa;
}

int NoD::Show_Atom(int ni, int nl)
{
	if(!(ni>=0 && ni<4))
	cout<<"Caution!! In NoD class, at Show_Atom(int nh,int ni,int nl), ni is out of range"<<endl;
	
	if(!(nl>=0 && nl<2))
	cout<<"Caution!! In NoD class, at Show_Atom(int nh,int ni,int nl), nl is out of range"<<endl;
	return ati[ni][nl];
}

double NoD::Show_Mr(int ni,int nk)
{
	return atr[ni][nk];
}
////////////////////////////////////////////////////////////////////////////////

//Initialize

////////////////////////////////////////////////////////////////////////////////
void NoD::Norm(int n00,int n01,
			   int n10,int n11,
			   int n20,int n21,
			   int n30,int n31,
			   int bo0,int bo1)
{
	ati[0][0]=n00;
	ati[0][1]=n01;
	ati[1][0]=n10;
	ati[1][1]=n11;
	ati[2][0]=n20;
	ati[2][1]=n21;
	ati[3][0]=n30;
	ati[3][1]=n31;
	
	if(bo0!=bo1)
	kappa=kappa_b;
	else if(bo0)
	kappa=kappa_p;
	else
	kappa=kappa_m;
}
////////////////////////////////////////////////////////////////////////////////

//Calculate

////////////////////////////////////////////////////////////////////////////////
void NoD::Parallel(double d00,double d01,double d02,
				   double d10,double d11,double d12,
				   double d20,double d21,double d22,
				   double d30,double d31,double d32)
{
	for(i=0;i<4;i++)
	{
		for(k=0;k<3;k++)
		atr[i][k]=0.0;
	}
	atd[0][0][0]=d00;
	atd[0][0][1]=d01;
	atd[0][0][2]=d02;
	atd[0][1][0]=d10;
	atd[0][1][1]=d11;
	atd[0][1][2]=d12;
	atd[0][2][0]=d20;
	atd[0][2][1]=d21;
	atd[0][2][2]=d22;
	atd[1][0][0]=d30;
	atd[1][0][1]=d31;
	atd[1][0][2]=d32;
	atd[1][1][0]=d20;
	atd[1][1][1]=d21;
	atd[1][1][2]=d22;
	atd[1][2][0]=d10;
	atd[1][2][1]=d11;
	atd[1][2][2]=d12;
	
	Calculate();
	
	double S1=0.0,S2=1.0/(L[0]*L[1]);
	for(l=0;l<3;l++)
	S1+=nd[0][l]*nd[1][l];
	
	for(h=0;h<2;h++)
	{
		for(i=0;i<3;i++)
		{
			for(k=0;k<3;k++)
			{
				double B=0.0,C=0.0;
				for(H=0;H<2;H++)
				{
					double A=0.0;
					for(l=0;l<3;l++)
					{
						A+=nd[H][l]*Dif(H,h,l,i,k);
						B+=nd[(H+1)%2][l]*Dif(H,h,l,i,k);
					}
					C+=A/(L[H]*L[H]);
				}
				if(!h)
				atr[i][k]+=kappa*S2*(B-S1*C);
				else
				atr[-i+3][k]+=kappa*S2*(B-S1*C);
			}
		}
	}
}

void NoD::Calculate()
{
	for(h=0;h<2;h++)
	{
		for(i=0;i<3;i++)
		{
			for(j=0;j<3;j++)
			{
				for(k=0;k<3;k++)
				D[h][i][j][k]=atd[h][j][k]-atd[h][i][k];
			}
		}
		for(i=0;i<3;i++)
		nd[h][i]=D[h][0][1][(i+1)%3]*D[h][0][2][(i+2)%3]-D[h][0][2][(i+1)%3]*D[h][0][1][(i+2)%3];
		L[h]=0.0;
		for(i=0;i<3;i++)
		L[h]+=nd[h][i]*nd[h][i];
		L[h]=sqrt(L[h]);
	}
}

double NoD::Dif(int nH,int nh,int nl,int ni, int nk)
{
	double ans=0.0;
	if(nh==nH)
	ans=D[nH][(ni+nl-nk+3)%3][(ni-nl+nk+3)%3][(-nl-nk+6)%3];
	else if(ni)
	ans=D[nH][(-ni+nl-nk+6)%3][(-ni-nl+nk+6)%3][(-nl-nk+6)%3];
	
	return ans;
}
