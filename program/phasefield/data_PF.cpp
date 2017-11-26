#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>
#include <stdio.h>
#include <sys/stat.h>
//#include <direct.h>
#include <sys/stat.h>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <numeric>
#include <memory.h>
#include "ygnuplot.cpp"
#define Pi 3.14159265358979323
#define sina sin(2.0/5.0*Pi)
#define cosa cos(2.0/5.0*Pi)
#define sin2a sin(4.0/5.0*Pi)
#define cos2a cos(4.0/5.0*Pi)
#define sin3a sin(6.0/5.0*Pi)
#define cos3a cos(6.0/5.0*Pi)
#define sin4a sin(8.0/5.0*Pi)
#define cos4a cos(8.0/5.0*Pi)
using namespace std;


class DaD
{
protected:
	////////////////////////////////////
	int h,i,j,k,l,m,n;
	bool display_PF,data_input_PF,plate_plot,around;
	double t,dt,tmax;
	int ft,ftmax;
	int plot_plate_id;
	double dr,dA;
	double Field_Xrange,Field_Yrange;
	double Field_Centerx,Field_Centery;
	////////////////////////////////////
	int field_xrange,field_yrange;
	int field_centerx,field_centery;
	double Initial_Plate_Area;
	double Initial_Boundary_Radius;
	double Field_Growth,dr_boundary;
	double Anus_Radius;
	double Plate_Create_Radius;
	double Initial_Plate_Radius;
	int initial_plate_radius;
	int plate_create_radius;
	int plate_create_donut_range;
	double initial_plate_random_theta;
	double feedback_co;
	////////////////////////////////////
	double tau;
	double e;
	double alfa;
	double beta;
	double gamma;
	double eta;
	double Plate_Growth;
	double dV;
	double a_peri;
	double threshold_plate;
	double threshold_plate_boundary;
	double min_u_threshold;
	double D0,D1,D2;
	////////////////////////////////////Calculate
	double Theta(double x,double y);
	double Angle(double x,double y);
	double Theta_two_vector(double x1,double y1,double x2,double y2);
	double Angle_two_vector(double x1,double y1,double x2,double y2);
	
public:
	////////////////////////////////////Constructa
	DaD();
	~DaD();
	DaD(const DaD& dad);
	DaD& operator=(const DaD& dad);
	////////////////////////////////////Judge Parameter
	void Judge_Parameter();
};

////////////////////////////////////////////////////////////////////////////////

//Constructa

////////////////////////////////////////////////////////////////////////////////
DaD::DaD()
{
	//display_PF=true;
	display_PF=false;
	data_input_PF=true;
	//data_input_PF=false;
	//plate_plot=true;
	plate_plot=false;
	around=true;
	//around=false;
	
	dt=0.005;
	ftmax=100;
	dr=0.02;
	dA=dr*dr;
	
	plot_plate_id=1;
	
	Plate_Growth=0.036;
	Field_Growth=0.1;
	Initial_Plate_Radius=0.5;
	Anus_Radius=1.5;
	feedback_co=0.00001;
	//tmax=100.0;
	tmax=Initial_Plate_Radius/Field_Growth*20.0;
	//cout<<"tmax="<<tmax<<endl;
	initial_plate_random_theta=5.0*Pi/180.0;
	alfa=5.0;
	beta=40.0;
	e=0.11;
	threshold_plate=0.3;
	
	
	//Fie class
	dr_boundary=dt*Field_Growth;
	Initial_Plate_Area=Initial_Plate_Radius*Initial_Plate_Radius*Pi;
	initial_plate_radius=(int)(Initial_Plate_Radius/dr);
	Initial_Boundary_Radius=Anus_Radius+2.0*Initial_Plate_Radius-0.1;
	Field_Xrange=(Field_Growth*tmax+Initial_Boundary_Radius)*2.0+4.0;
	if(!around)
	Field_Xrange+=sqrt(Plate_Growth*tmax);
	Field_Yrange=Field_Xrange;
	Field_Centerx=Field_Xrange*0.5;
	Field_Centery=Field_Yrange*0.5;
	field_xrange=(int)(Field_Xrange/dr);
	field_yrange=(int)(Field_Yrange/dr);
	field_centerx=(int)(Field_Centerx/dr);
	field_centery=(int)(Field_Centery/dr);
	Plate_Create_Radius=Anus_Radius+Initial_Plate_Radius+0.05;
	plate_create_radius=(int)(Plate_Create_Radius/dr);
	plate_create_donut_range=2;
	
	
	
	//Pla class
	tau=0.8;
	gamma=5.0;
	eta=4.0;
	a_peri=0.0025;
	threshold_plate_boundary=(0.5-1.0/(4.0*2.0*e))*(0.5-1.0/(4.0*2.0*e));
	min_u_threshold=0.0002;
	D0=dt/dA/tau*e*e;
	D1=dt/dr/tau;
	D2=2.0*Plate_Growth*sqrt(Pi);
	
	Judge_Parameter();
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
}

DaD& DaD::operator=(const DaD& dad)
{
	i=dad.i;
	j=dad.j;
	t=dad.t;
	dt=dad.dt;

	return *this;
}
////////////////////////////////////////////////////////////////////////////////

//Judge Parameter

////////////////////////////////////////////////////////////////////////////////
void DaD::Judge_Parameter()
{
	if(Field_Xrange<2.0*(tmax*Field_Growth+Initial_Boundary_Radius))
	{
		cout<<"Warning field range is not sufficient."<<endl;
		cout<<"Please add field range"<<2.0*(tmax*Field_Growth+Initial_Boundary_Radius)-Field_Xrange<<endl;
	}
	if(dt*e*e/dA>=0.5)
	{
		cout<<"Parammeter is not reasonable"<<endl;
		cout<<"d=dt*e*e/dA="<<dt*e*e/dA<<">=0.5"<<endl;
	}
	else{}
	return;
}
////////////////////////////////////////////////////////////////////////////////

//Calculate

////////////////////////////////////////////////////////////////////////////////
double DaD::Theta(double x,double y)
{
	double ans_theta;
	double d=sqrt(x*x+y*y);
	if(y>0.0)
	ans_theta=acos(x/d);
	else
	ans_theta=-acos(x/d);
	
	return ans_theta;
}

double DaD::Angle(double x,double y)
{
	return Theta(x,y)*180.0/Pi;
}

double DaD::Theta_two_vector(double x1,double y1,double x2,double y2)
{
	double ans_theta;
	double theta1=Theta(x1,y1);
	double theta2=Theta(x2,y2);
	if(theta1<0.0 && theta2>0.0)
	ans_theta=theta1-theta2+2.0*Pi;
	else
	ans_theta=theta1-theta2;
	
	return ans_theta;
}

double DaD::Angle_two_vector(double x1,double y1,double x2,double y2)
{
	return Theta_two_vector(x1,y1,x2,y2)*180.0/Pi;
}