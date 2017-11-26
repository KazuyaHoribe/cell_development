#pragma once
#include "data_SN.cpp"

class AtD:public DaD
{
private:
	int connections_num;
	
public:
	double r[3];
	double mr[3];
	
	int status;//0:膜、1:プレート、2:頂上系、3:肛門
	
	////////////////////////////////////Construct
	AtD();
	~AtD();
	AtD(const AtD& atd);
	AtD& operator=(const AtD& atd);
	////////////////////////////////////Get
	void Where(double nx,double ny,double nz);
	void Which_Status(int n);
	void Get_Mr(double mrx,double mry,double mrz);
	////////////////////////////////////Show
	DV Show_2D_Coordinate();
	int Show_Status();
	////////////////////////////////////Simulation
	void Move_Atom();
	void Reset_Mr();
};

AtD::AtD()
{
	for(i=0;i<3;i++)
	{
		r[i]=0.0;
		mr[i]=0.0;
	}
	status=1;
}

AtD::~AtD()
{
}

AtD::AtD(const AtD& atd)
{
	//cout<<"atom_copy"<<endl;
	for(i=0;i<3;i++)
	{
		r[i]=0.0;
		mr[i]=0.0;
	}
	status=atd.status;
}

AtD& AtD::operator=(const AtD& atd)
{
	//cout<<"atom_operator="<<endl;
	for(i=0;i<3;i++)
	{
		r[i]=0.0;
		mr[i]=0.0;
	}
	status=atd.status;
}
////////////////////////////////////////////////////////////////////////////////

//Get

////////////////////////////////////////////////////////////////////////////////
void AtD::Where(double nx,double ny,double nz)
{
	r[0]=nx;
	r[1]=ny;
	r[2]=nz;
}

void AtD::Which_Status(int n)
{
	status=n;
}

void AtD::Get_Mr(double mrx,double mry,double mrz)
{
	mr[0]+=mrx;
	mr[1]+=mry;
	mr[2]+=mrz;
}
////////////////////////////////////////////////////////////////////////////////

//Show

////////////////////////////////////////////////////////////////////////////////
DV AtD::Show_2D_Coordinate()
{
	DV ans{r[0],r[1]};
	
	return ans;
}

int AtD::Show_Status()
{
	return status;
}
////////////////////////////////////////////////////////////////////////////////

//Simulation

////////////////////////////////////////////////////////////////////////////////
void AtD::Move_Atom()
{
	for(i=0;i<3;i++)
	r[i]+=dt*mr[i];
}

void AtD::Reset_Mr()
{
	for(i=0;i<3;i++)
	mr[i]=0.0;
}
