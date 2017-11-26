#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <numeric>
#include <memory.h>
#include <string.h>
#include "judgeIO.cpp"
#include "ygnuplot.cpp"
#define Pi 3.14159265358979323
using namespace std;
typedef vector<double> DV;
typedef vector<vector<double> > DVV;
typedef vector<vector<vector<double> > > DVVV;

class DaD
{
protected:
	////////////////////////////////////一般変数
	bool Periproct_Form,display_SN,Cods_Plot,Nods_Plot,Center_Norm_Plot,Vector_Plot;
	int H,h,i,j,k,l,m,n;
	int ft,ftmax;
	int height_num,range_num,sen_num,cods_num,nods_num;
	double t,dt,tmax;
	double dr,dA;
	double Field_Xrange,Field_Yrange;
	double Field_Centerx,Field_Centery;
	double boundary_radius,anus_radius;
	double sen_L,sen_l;
	double epsilon_p,epsilon_b,epsilon_m,kappa_p,kappa_b,kappa_m;
	
	double force;
	
public:
	////////////////////////////////////Constructa
	DaD();
	~DaD();
	DaD(const DaD& dad);
	DaD& operator=(const DaD& dad);
	////////////////////////////////////Data_Split
	DVVV CDSD(ifstream& ifs);
};

////////////////////////////////////////////////////////////////////////////////

//Constructa

////////////////////////////////////////////////////////////////////////////////
DaD::DaD()
{
	Periproct_Form=true;
	//Periproct_Form=false;
	display_SN=true;
	//display_SN=false;
	Cods_Plot=true;
	//Cods_Plot=false;
	//Nods_Plot=true;
	Nods_Plot=false;
	//Center_Norm_Plot=true;
	Center_Norm_Plot=false;
	//Vector_Plot=true;
	Vector_Plot=false;
	
	dt=0.0005;
	tmax=10000.0;
	ftmax=100;
	dr=0.02;
	dA=dr*dr;
	
	sen_L=dr*5.0;
	sen_l=sen_L*0.5*sqrt(3.0);
	Field_Xrange=5.0;
	Field_Yrange=5.0;
	Field_Centerx=Field_Xrange*0.5;
	Field_Centery=Field_Yrange*0.5;
	boundary_radius=Field_Xrange*0.5;
	anus_radius=0.1;
	height_num=(int)(Field_Yrange/sen_l)+1;
	range_num=(int)(Field_Xrange/sen_L)+1;
	//height_num=2;
	//range_num=2;
	sen_num=height_num*range_num;
	cods_num=3*height_num*range_num-2*height_num-2*range_num+1;
	nods_num=3*height_num*range_num-4*height_num-4*range_num+5;
	epsilon_p=1.0;
	epsilon_b=0.05;
	epsilon_m=0.05;
	kappa_p=0.01;
	kappa_b=0.005;
	kappa_m=0.005;
	
	
	force=0.3;
}

DaD::~DaD()
{
}

DaD::DaD(const DaD& dad)
{
	i=dad.i;
	j=dad.j;
	t=dad.t;
	dt=dad.dt;
	display_SN=dad.display_SN;
	dt=dad.dt;
	tmax=dad.tmax;
	ftmax=dad.ftmax;
	dr=dad.dr;
	dA=dad.dA;
	sen_L=dad.sen_L;
	sen_l=dad.sen_l;
	Field_Xrange=dad.Field_Xrange;
	Field_Yrange=dad.Field_Yrange;
	boundary_radius=dad.boundary_radius;
	height_num=dad.height_num;
	range_num=dad.range_num;
	sen_num=dad.sen_num;
	cods_num=dad.cods_num;
	nods_num=dad.nods_num;
	epsilon_p=dad.epsilon_p;
	epsilon_b=dad.epsilon_b;
	epsilon_m=dad.epsilon_m;
	kappa_p=dad.kappa_p;
	kappa_b=dad.kappa_b;
	kappa_m=dad.kappa_m;
	//cout<<"Data_SN copy constructa"<<endl;
}

DaD& DaD::operator=(const DaD& dad)
{
	i=dad.i;
	j=dad.j;
	t=dad.t;
	dt=dad.dt;
	display_SN=dad.display_SN;
	dt=dad.dt;
	tmax=dad.tmax;
	ftmax=dad.ftmax;
	dr=dad.dr;
	dA=dad.dA;
	sen_L=dad.sen_L;
	sen_l=dad.sen_l;
	Field_Xrange=dad.Field_Xrange;
	Field_Yrange=dad.Field_Yrange;
	boundary_radius=dad.boundary_radius;
	height_num=dad.height_num;
	range_num=dad.range_num;
	sen_num=dad.sen_num;
	cods_num=dad.cods_num;
	nods_num=dad.nods_num;
	epsilon_p=dad.epsilon_p;
	epsilon_b=dad.epsilon_b;
	epsilon_m=dad.epsilon_m;
	kappa_p=dad.kappa_p;
	kappa_b=dad.kappa_b;
	kappa_m=dad.kappa_m;
	//cout<<"Data_SN operator="<<endl;
}

////////////////////////////////////////CSVファイルのDataをScanし、DVVV型のコンテナとして出力
DVVV DaD::CDSD(ifstream &ifs)
{
	DVVV ans;
	char str[256];
	if(ifs.fail())
	{
		cerr<<"CDSD fail"<<endl;
	}
	
	int ii,count0=0,count1=0;
	double x0,y0;//始点
	DVV v_pair;
	while(ifs.getline(str,256-1))
	{
		if(count0>0)
		{
			ii=0;
			char *s;
			s=strtok(str,",");
			ii++;
			
			double x1,y1;//各点座標
			while(s!=NULL)
			{
				s=strtok(NULL,",");
				if(ii==2)
				{
					char *ends;
					x1=strtod(s,&ends);
					if(count1==0)
					x0=x1;
				}
				if(ii==3)
				{
					char *ends;
					y1=strtod(s,&ends);
					if(count1==0)
					y0=y1;
				}
				ii++;
			}
			
			if(count1==0)
			{
				DV pair1{x1,y1};
				v_pair.push_back(pair1);
				count1++;
			}
			else
			{
				if(x0==x1 && y0==y1)
				{
					ans.push_back(v_pair);
					v_pair.clear();
					count1=0;
				}
				else
				{
					DV pair1{x1,y1};
					v_pair.push_back(pair1);
				}
			}
		}
		else
		count0++;
	}
	/*
	for(i=0;i<ans.size();i++)
	{
		for(j=0;j<ans[i].size();j++)
		cout<<"ans["<<i<<","<<j<<"][0] = "<<ans[i][j][0]
			<<",ans["<<i<<","<<j<<"][0] = "<<ans[i][j][1]<<endl;
	}*/
	
	return ans;
}