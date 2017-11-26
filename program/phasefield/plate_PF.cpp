#pragma once
#include "data_PF.cpp"
typedef vector<vector<int> > IVV;
typedef vector<vector<double> > DVV;
typedef vector<double> DV;

class PlD: public DaD
{
private:
	int plate_id;
	int plate_xrange,plate_yrange;
	int Fiex,Fiey;
	int plate_centerx,plate_centery;
	double create_x,create_y;
	double Plate_Centerx,Plate_Centery;
	double plate_create_time;
	double plate_time;
	double Area;
	double Periphery;
	double Peri_fai;
	double V;
	double ax,ay;
	double plate_first_angle,plate_angle;
	
	DVV u,nu;
	
public:
	//CGnuplot gnu;
	//fstream file;
	
	////////////////////////////////////Constructa
	PlD();
	~PlD();
	PlD(const PlD& pld);
	PlD& operator=(const PlD& pld);
	////////////////////////////////////Get
	void Get(int Id,double time,double x,double y);
	////////////////////////////////////Show
	int Show_Plate_Id();
	int Show_Fiex();
	int Show_Fiey();
	double Show_Create_X();
	double Show_Create_Y();
	double Show_Create_Time();
	double Show_Distance();
	double Show_Area();
	double Show_V();
	double Show_Angle_First();
	double Show_Angle();
	double Show_u(int m, int n);
	double Show_Ax();
	double Show_Ay();
	double Show_Centerx();
	double Show_Centery();
	////////////////////////////////////First simulation
	void First_initial_calculation();
	void First_variable_calculation();
	void First_calculation();
	////////////////////////////////////Initial Calculation
	void Velocity_calculation();
	////////////////////////////////////Calculation
	double h(double u);
	void Variable_calculation(const DVV& fai,const DVV& around_plate,const DVV& anus,double t);
	void Area_Center_calculation();
	void Peri_fai_calculation(const DVV& fai,const DVV& around_plate,const DVV& anus);
	void Periphery_calculation();
	void Timestep_calculation();
	int Add(double boundary);
	void Extension();
	int Fai_calculation(IVV& divide_field,DVV& fai);
	int Fai_calculation(IVV& divide_field,DVV& fai,DVV& fai1);
	void Calculation(const DVV& fai,const DVV& around_plate,const DVV& anus,double t,int ft);
};

////////////////////////////////////////////////////////////////////////////////

//Constructa

////////////////////////////////////////////////////////////////////////////////
PlD::PlD()
{
	First_initial_calculation();
}

PlD::~PlD()
{
}

PlD::PlD(const PlD& pld)
{
	plate_id=pld.plate_id;
	plate_create_time=pld.plate_create_time;
	create_x=pld.create_x;
	create_y=pld.create_y;
	Fiex=pld.Fiex;
	Fiey=pld.Fiey;
	plate_xrange=pld.plate_xrange;
	plate_yrange=pld.plate_yrange;
	plate_centerx=pld.plate_centerx;
	plate_centery=pld.plate_centery;
	Plate_Centerx=pld.Plate_Centerx;
	Plate_Centery=pld.Plate_Centery;
	plate_time=pld.plate_time;
	Area=pld.Area;
	V=pld.V;
	u=pld.u;
	nu=pld.nu;
	ax=pld.ax;
	ay=pld.ay;
	plate_first_angle=pld.plate_first_angle;
	plate_angle=pld.plate_angle;
	//cout<<"V="<<V<<endl;
	//CGnuplot gnu_new;
	//gnu=gnu_new;
}

PlD& PlD::operator=(const PlD& pld)
{
	plate_id=pld.plate_id;
	plate_create_time=pld.plate_create_time;
	create_x=pld.create_x;
	create_y=pld.create_y;
	Fiex=pld.Fiex;
	Fiey=pld.Fiey;
	plate_xrange=pld.plate_xrange;
	plate_yrange=pld.plate_yrange;
	plate_centerx=pld.plate_centerx;
	plate_centery=pld.plate_centery;
	Plate_Centerx=pld.Plate_Centerx;
	Plate_Centery=pld.Plate_Centery;
	plate_time=pld.plate_time;
	Area=pld.Area;
	V=pld.V;
	u=pld.u;
	nu=pld.nu;
	ax=pld.ax;
	ay=pld.ay;
	plate_first_angle=pld.plate_first_angle;
	plate_angle=pld.plate_angle;
	//cout<<"V="<<V<<endl;
	//CGnuplot gnu_new;
	//gnu=gnu_new;
	
	return *this;
}
////////////////////////////////////////////////////////////////////////////////

//Get

////////////////////////////////////////////////////////////////////////////////
void PlD::Get(int id,double time,double x,double y)
{
	plate_id=id;
	plate_create_time=time;
	create_x=x;
	create_y=y;
	Fiex=(int)(x/dr)-plate_centerx;
	Fiey=(int)(y/dr)-plate_centery;
	plate_time=0.0;
	Area_Center_calculation();
	Velocity_calculation();
	plate_first_angle=plate_angle;
	
	/*
	stringstream foldername,filename;
	foldername<<"../anime_plot/plate_"<<plate_id;
	filename<<"../plot/plate_"<<plate_id<<".dat";
	if(_mkdir(foldername.str().c_str())==0)
	cout<<"success plate_"<<plate_id<<" folder"<<endl;
	else
	cout<<"false plate_"<<plate_id<<" folder"<<endl;
	gnu.Gcom("set pm3d map interpolate 0.0,0.0\n");
	gnu.Gcom("set size square\n");
	gnu.Gcom("set cbrange [0:1]\n");
	gnu.Gcom("set palette defined (0 'white',1 'red')\n");
	gnu.Gcom("set xrange [%f:%f]\n",Field_Centerx-Field_Xrange*0.5-1.0,Field_Centerx+Field_Xrange*0.5+1.0);
	gnu.Gcom("set yrange [%f:%f]\n",Field_Centery-Field_Yrange*0.5-1.0,Field_Centery+Field_Yrange*0.5+1.0);
	gnu.Gcom("unset key\n");*/
	//cout<<"plate_first_angle="<<plate_first_angle<<endl;
}
////////////////////////////////////////////////////////////////////////////////

//Show

////////////////////////////////////////////////////////////////////////////////
int PlD::Show_Plate_Id()
{
	return plate_id;
}

int PlD::Show_Fiex()
{
	return Fiex;
}

int PlD::Show_Fiey()
{
	return Fiey;
}

double PlD::Show_Create_X()
{
	return create_x;
}

double PlD::Show_Create_Y()
{
	return create_y;
}

double PlD::Show_Create_Time()
{
	return plate_create_time;
}

double PlD::Show_Distance()
{
	return sqrt((Show_Centerx()-Field_Centerx)*(Show_Centerx()-Field_Centerx)
				+(Show_Centery()-Field_Centery)*(Show_Centery()-Field_Centery));
}

double PlD::Show_Area()
{
	return Area;
}

double PlD::Show_V()
{
	return V;
}

double PlD::Show_Angle_First()
{
	return plate_first_angle;
}

double PlD::Show_Angle()
{
	return plate_angle;
}

double PlD::Show_u(int m, int n)
{
	return u[m-Fiex][n-Fiey];
}

double PlD::Show_Ax()
{
	return ax;
}

double PlD::Show_Ay()
{
	return ay;
}

double PlD::Show_Centerx()
{
	return Plate_Centerx+Fiex*dr;
}

double PlD::Show_Centery()
{
	return Plate_Centery+Fiey*dr;
}
////////////////////////////////////////////////////////////////////////////////

//First simulation

////////////////////////////////////////////////////////////////////////////////
void PlD::First_initial_calculation()
{
	Fiex=0.0;
	Fiey=0.0;
	Area=0.0;
	plate_xrange=2*initial_plate_radius+4;
	plate_yrange=2*initial_plate_radius+4;
	u.resize(plate_xrange);
	nu.resize(plate_xrange);
	for(i=0;i<plate_xrange;i++)
	{
		u[i].resize(plate_yrange);
		nu[i].resize(plate_yrange);
	}
	for(i=plate_xrange*0.5-Initial_Plate_Radius*0.7;i<plate_xrange*0.5+Initial_Plate_Radius*0.7;i++)
	{
		for(j=plate_yrange*0.5-Initial_Plate_Radius*0.7;j<plate_yrange*0.5+Initial_Plate_Radius*0.7;j++)
		u[i][j]=1.0;
	}
	Area_Center_calculation();
	V=Initial_Plate_Area;
}

void PlD::First_variable_calculation()
{
	Extension();
	Area_Center_calculation();
}

void PlD::First_calculation()
{
	for(i=1;i<plate_xrange-1;i++)
	{
		for(j=1;j<plate_yrange-1;j++)
		{
			nu[i][j]=u[i][j]
					+D0*(u[i+1][j]+u[i-1][j]+u[i][j+1]+u[i][j-1]-4.0*u[i][j])
					+dt*u[i][j]*(1.0-u[i][j])*(u[i][j]-0.5+alfa*(Initial_Plate_Area*1.1-Area));
		}
	}
	u=nu;
}
////////////////////////////////////////////////////////////////////////////////

//Initial Calculation

////////////////////////////////////////////////////////////////////////////////
void PlD::Velocity_calculation()
{
	ax=(double)(plate_centerx+Fiex-field_centerx);
	ay=(double)(plate_centery+Fiey-field_centery);
	double d=sqrt(ax*ax+ay*ay);
	ax=ax/d;
	ay=ay/d;
	
	
	plate_angle=Angle(ax,ay);
	ax*=Field_Growth;
	ay*=Field_Growth;
}
////////////////////////////////////////////////////////////////////////////////

//Calculation

////////////////////////////////////////////////////////////////////////////////
double PlD::h(double u)
{
	return u*u*(3.0-2.0*u);
}

void PlD::Variable_calculation(const DVV& fai,const DVV& around_plate,const DVV& anus,double t)
{
	Extension();
	Area_Center_calculation();
	Peri_fai_calculation(fai,around_plate,anus);
	Velocity_calculation();
	Timestep_calculation();
}

void PlD::Area_Center_calculation()
{
	Area=0.0;
	Plate_Centerx=0.0;
	Plate_Centery=0.0;
	plate_centerx=1;
	plate_centery=1;
	for(i=0;i<plate_xrange;i++)
	{
		for(j=0;j<plate_yrange;j++)
		{
			if(u[i][j]>threshold_plate)//2016/10/15
			{
				Area+=h(u[i][j]);
				Plate_Centerx+=h(u[i][j])*i;
				Plate_Centery+=h(u[i][j])*j;
			}
		}
	}
	Plate_Centerx/=Area;
	Plate_Centery/=Area;
	Area*=dA;
	Plate_Centerx*=dr;
	Plate_Centery*=dr;
	plate_centerx=(int)(Plate_Centerx/dr);
	plate_centery=(int)(Plate_Centery/dr);
}

void PlD::Peri_fai_calculation(const DVV& fai,const DVV& around_plate,const DVV& anus)
{
	Peri_fai=0.0;
	for(i=0;i<plate_xrange;i++)
	{
		for(j=0;j<plate_yrange;j++)
		{
			if(u[i][j]>min_u_threshold)
			Peri_fai+=fai[Fiex+i][Fiey+j]+around_plate[Fiex+i][Fiey+j]+anus[Fiex+i][Fiey+j];
		}
	}
	Peri_fai*=feedback_co;
	//if(plate_id==2)
	//cout<<"Peri_fai="<<Peri_fai<<endl;
}

void PlD::Timestep_calculation()
{
	//double feedback=D2*sqrt(Area)-0.1*(V-Area);
	double feedback=D2*sqrt(Area)-Peri_fai;
	//double feedback=D2*sqrt(Area);
	if(feedback>0.0)
	V+=dt*feedback;
}

int PlD::Add(double boundary)//////////////////////////////
{
	int ans=0;
	if(boundary>min_u_threshold)
	ans=10;
	else
	ans=0;
	
	return ans;
}

void PlD::Extension()
{
	int add=0;
	double lower=0.0;
	double upper=0.0;
	for(i=0;i<plate_xrange;i++)
	{
		if(u[i][4]>lower)
		lower=u[i][4];
		if(u[i][plate_yrange-5]>upper)
		upper=u[i][plate_yrange-5];
	}
	if(lower)
	{
		add=Add(lower);
		for(i=0;i<plate_xrange;i++)
		{
			DV::iterator where=u[i].begin();
			u[i].insert(where,add,0.0);
			where=nu[i].begin();
			nu[i].insert(where,add,0.0);
		}
		plate_yrange=(int)u[0].size();
		Fiey-=add;
	}
	if(upper)
	{
		add=Add(upper);
		for(i=0;i<plate_xrange;i++)
		{
			DV::iterator where=u[i].end();
			u[i].insert(where,add,0.0);
			where=nu[i].end();
			nu[i].insert(where,add,0.0);
		}
		plate_yrange=(int)u[0].size();
	}
	
	
	lower=0.0;
	upper=0.0;
	for(j=0;j<plate_yrange;j++)
	{
		if(u[4][j]>lower)
		lower=u[4][j];
		if(u[plate_xrange-5][j]>upper)
		upper=u[plate_xrange-5][j];
	}
	if(lower)
	{
		add=Add(lower);
		DV v(plate_yrange,0.0);
		DVV::iterator where=u.begin();
		u.insert(where,add,v);
		where=nu.begin();
		nu.insert(where,add,v);
		plate_xrange=(int)u.size();
		Fiex-=add;
	}
	
	if(upper)
	{
		add=Add(upper);
		DV v(plate_yrange,0.0);
		DVV::iterator where=u.end();
		u.insert(where,add,v);
		where=nu.end();
		nu.insert(where,add,v);
		plate_xrange=(int)u.size();
	}
}
int PlD::Fai_calculation(IVV& divide_field,DVV& fai)
{
	int ans=0;
	for(i=0;i<plate_xrange;i++)
	{
		for(j=0;j<plate_yrange;j++)
		{
			if(Fiex+i<field_xrange && Fiey+j<field_xrange)
			{
				if(fai[Fiex+i][Fiey+j]<u[i][j])
				{
					fai[Fiex+i][Fiey+j]=u[i][j];
					divide_field[Fiex+i][Fiey+j]=plate_id;
				}
				ans=0;
			}
			else
			ans=1;
		}
	}
	return ans;
}

int PlD::Fai_calculation(IVV& divide_field,DVV& fai,DVV& fai1)
{
	int ans=0;
	for(i=0;i<plate_xrange;i++)
	{
		for(j=0;j<plate_yrange;j++)
		{
			if(Fiex+i<field_xrange && Fiey+j<field_xrange)
			{
				if(fai[Fiex+i][Fiey+j]<u[i][j])
				{
					fai[Fiex+i][Fiey+j]=u[i][j];
					divide_field[Fiex+i][Fiey+j]=plate_id;
				}
				if(plate_id==plot_plate_id)
				fai1[Fiex+i][Fiey+j]=u[i][j];
				ans=0;
			}
			else
			ans=1;
		}
	}
	return ans;
}

void PlD::Calculation(const DVV& fai,const DVV& around_plate,const DVV& anus,double t,int ft)
{
	double Ax=D1*ax,Ay=D1*ay;
	for(i=4;i<plate_xrange-4;i++)
	{
		for(j=4;j<plate_yrange-4;j++)
		{
			if(u[i-4][j-4]>min_u_threshold ||
				u[i-4][j+4]>min_u_threshold ||
				u[i+4][j-4]>min_u_threshold ||
				u[i+4][j+4]>min_u_threshold)
			{
				if(u[i-4][j-4]<1.0 ||
					u[i-4][j+4]<1.0 ||
					u[i+4][j-4]<1.0 ||
					u[i+4][j+4]<1.0)
				{
					//if((u[i-4][j-4]-u[i][j])*(u[i-4][j-4]-u[i][j])>0.0001 ||
					//	(u[i-4][j+4]-u[i][j])*(u[i-4][j+4]-u[i][j])>0.0001 ||
					//	(u[i+4][j-4]-u[i][j])*(u[i+4][j-4]-u[i][j])>0.0001 ||
					//	(u[i+4][j+4]-u[i][j])*(u[i+4][j+4]-u[i][j])>0.0001)
					{
						nu[i][j]=u[i][j]
								+D0*(u[i+1][j]+u[i-1][j]+u[i][j+1]+u[i][j-1]-4.0*u[i][j])
								+dt*u[i][j]*(1.0-u[i][j])
								   *(u[i][j]-0.5+alfa*(V-Area)-beta*(fai[Fiex+i][Fiey+j]+around_plate[Fiex+i][Fiey+j]+anus[Fiex+i][Fiey+j]-u[i][j]));
						if(ax>0.0)
						nu[i][j]+=Ax*((u[i-1][j]-u[i][j])
								+0.5*(1.0-ax*dt/dr)*(u[i+1][j]-2.0*u[i][j]+u[i-1][j]));
						if(ax<0.0)
						nu[i][j]+=Ax*((u[i][j]-u[i+1][j])
								-0.5*(1.0+ax*dt/dr)*(u[i+1][j]-2.0*u[i][j]+u[i-1][j]));
						if(ay>0.0)
						nu[i][j]+=Ay*((u[i][j-1]-u[i][j])
								+0.5*(1.0-ay*dt/dr)*(u[i][j+1]-2.0*u[i][j]+u[i][j-1]));
						if(ay<0.0)
						nu[i][j]+=Ay*((u[i][j]-u[i][j+1])
								-0.5*(1.0+ay*dt/dr)*(u[i][j+1]-2.0*u[i][j]+u[i][j-1]));
					}
				}
			}
		}
	}
	u=nu;
	/*
	bool fno=true;
	stringstream filename;
	filename<<"../plot/plate_"<<plate_id<<".dat";
	
	if(!(ft%ftmax))
	{
		file.open(filename.str().c_str(),ios::out);
		if(!file)
		{
			cerr<<"file open error plate_"<<plate_id<<endl;
			fno=false;
		}
		if(fno)
		{
			for(i=0;i<field_xrange;i++)
			{
				for(j=0;j<field_yrange;j++)
				{
					if(i-Fiex>=0 && i-Fiex<=field_xrange && j-Fiey>=0 && j-Fiey<=field_yrange)
					file<<i<<"\t"<<j<<"\t"<<(double)i*dr<<"\t"<<(double)j*dr<<"\t"<<u[i-Fiex][j-Fiey]<<endl;
					file<<endl;
				}
			}
		}
		file.close();
		gnu.Gcom("set title 't=%f'\n",t);

		if(!display_PF)
		{
			stringstream filetitle;
			filetitle<<"set output '.././anime_plot/plate_"<<plate_id<<"/t="<<t<<".png'\n";
			gnu.Gcom("set terminal png\n");
			gnu.Gcom(filetitle.str().c_str());
		}
		if(display_PF)
		gnu.Gcom("set terminal windows\n");
		stringstream splot;
		splot<<"splot '.././plot/plate_"<<plate_id<<".dat' u 3:4:5 w pm3d\n";
		gnu.Gcom(splot.str().c_str());
		gnu.Gflush();
	}
	*/
}