#pragma once
#include "data_PF.cpp"
#include "plate_PF.cpp"
#include "random.cpp"
typedef vector<vector<int> > IVV;
typedef vector<vector<double> > DVV;
typedef vector<PlD> PV;
typedef vector<int> IV;
typedef vector<double> DV;


class FiD: public DaD
{
private:
	int pn;
	int boundary_radius;
	double Boundary_Radius;
	double Anus_Centerx,Anus_Centery;
	int anus_centerx,anus_centery;
	
	Ran ran;
	PlD pld;
	IVV divide_field;
	DVV fai,fai1;
	DVV around_plate;
	DVV anus;
	PV per;
	
public:
	////////////////////////////////////Constructa
	FiD();
	~FiD();
	FiD(const FiD& fid);
	FiD& operator=(const FiD& fid);
	////////////////////////////////////Show
	double Fai(int x,int y);
	////////////////////////////////////First calculation
	void First_calculation();
	////////////////////////////////////Initialize
	void Initialize();
	void Initial_Point_Plate_Create();
	void Initial_placement();
	////////////////////////////////////Calculate
	void Calculate(double t,int ft);
	void Around_plate_calculate_circle();
	void Around_plate_calculate_ellipse();
	void Anus_calculate_center();
	void Fai_calculate();
	void Plate_create(double t);
	void Plate_create2(double t);
	////////////////////////////////////Input
	void Input_data_to_file();
	////////////////////////////////////Timechange
	void Timechange();
};

////////////////////////////////////////////////////////////////////////////////

//Constructa

////////////////////////////////////////////////////////////////////////////////
FiD::FiD()
{
	if(_mkdir("../plot")==0)
    cout<<"success plot folder"<<endl;
    else
    cout<<"false plot folder"<<endl;
    
    if(_mkdir("../anime_plot")==0)
	{
		cout<<"success anime_plot folder"<<endl;
		//if(_mkdir("../anime_plot/plate_All")==0)
		//cout<<"success plate_All folder"<<endl;
		//else
		//cout<<"false plate_All folder"<<endl;
	}
	else
	cout<<"false anime_plot folder"<<endl;
	
	Initialize();
	//Initial_Point_Plate_Create();
	First_calculation();
	Initial_placement();
}

FiD::~FiD()
{
}

FiD::FiD(const FiD& fid)
{
}

FiD& FiD::operator=(const FiD& fid)
{
	return *this;
}
////////////////////////////////////////////////////////////////////////////////

//Show

////////////////////////////////////////////////////////////////////////////////
double FiD::Fai(int x,int y)
{
	return fai[x][y];
}
////////////////////////////////////////////////////////////////////////////////

//First calculation

////////////////////////////////////////////////////////////////////////////////
void FiD::First_calculation()
{
	for(t=0.0;t<1000.0;t+=dt)
	{
		pld.First_variable_calculation();
		pld.First_calculation();
		if(pld.Show_Area()>Initial_Plate_Area)
		{
			cout<<"Initial plate area is "<<pld.Show_Area()<<endl;
			cout<<"t="<<t<<endl;
			break;
		}
	}
}
////////////////////////////////////////////////////////////////////////////////

//Initialize

////////////////////////////////////////////////////////////////////////////////
void FiD::Initialize()
{
	Boundary_Radius=Initial_Boundary_Radius;
	boundary_radius=(int)(Boundary_Radius/dr);
	fai.resize(field_xrange);
	around_plate.resize(field_xrange);
	anus.resize(field_xrange);
	divide_field.resize(field_xrange);
	if(plate_plot)
	{
		fai1.resize(field_xrange);
		for(i=0;i<field_xrange;i++)
		fai1[i].resize(field_yrange);
		for(i=0;i<field_xrange;i++)
		{
			for(j=0;j<field_yrange;j++)
			fai1[i][j]=0.0;
		}
	}
	for(i=0;i<field_xrange;i++)
	{
		fai[i].resize(field_yrange);
		around_plate[i].resize(field_yrange);
		anus[i].resize(field_yrange);
		divide_field[i].resize(field_yrange);
	}
	for(i=0;i<field_xrange;i++)
	{
		for(j=0;j<field_yrange;j++)
		{
			fai[i][j]=0.0;
			around_plate[i][j]=0.0;
			anus[i][j]=0.0;
			divide_field[i][j]=0;
		}
	}
	Anus_calculate_center();
}

void FiD::Initial_Point_Plate_Create()
{
	fstream file_data_base;
	stringstream filename_data_base;
	filename_data_base<<".././plot/point at seting plate.dat";
	file_data_base.open(filename_data_base.str().c_str(),ios::out);
	if(!file_data_base)
	{
		cerr<<"file_data_base open error"<<endl;
		//return 0;
	}
	file_data_base<<"x"<<"\t"<<"y"<<"\t"<<"angle"<<endl;
	int judge_range=plate_create_radius+plate_create_donut_range;
	for(i=field_centerx-judge_range;i<=field_centerx+judge_range;i++)
	{
		for(j=field_centery-judge_range;j<=field_centery+judge_range;j++)
		{
			int jud=(i-field_centerx)*(i-field_centerx)
					+(j-field_centery)*(j-field_centery);
			if(jud>plate_create_radius*plate_create_radius && jud<judge_range*judge_range)
			file_data_base<<i*dr<<"\t"<<j*dr<<"\t"<<Angle(i*dr-Field_Centerx,j*dr-Field_Centery)<<endl;
		}
	}
	file_data_base.close();
}

void FiD::Initial_placement()
{
	pld.Get(1,0.0,Field_Centerx,Field_Centery+Anus_Radius+Initial_Plate_Radius);
	per.push_back(pld);
	
	double theta=ran.Gaussian_Distribution(Pi,initial_plate_random_theta);
	cout<<"Initial theta="<<theta/Pi*180<<endl;
	pld.Get(2,0.0,Field_Centerx-(Anus_Radius+Initial_Plate_Radius)*sin(theta),Field_Centery+(Anus_Radius+Initial_Plate_Radius)*cos(theta));
	per.push_back(pld);
	pn=(int)per.size();
}
////////////////////////////////////////////////////////////////////////////////

//Calculate

////////////////////////////////////////////////////////////////////////////////
void FiD::Calculate(double t,int ft)
{
	//cout<<"cal st"<<endl;
	if(around)
	{
		Around_plate_calculate_circle();
		//Around_plate_calculate_pentagon();
	}
	//cout<<"ar fin"<<endl;
	Fai_calculate();
	//cout<<"fc fin"<<endl;
	for(i=0;i<pn;i++)
	{
		per[i].Variable_calculation(fai,around_plate,anus,t);
		per[i].Calculation(fai,around_plate,anus,t,ft);
	}
	//cout<<"vc fin"<<endl;
	//Plate_create(t);
	Plate_create2(t);
	//cout<<"pc fin"<<endl;
}

void FiD::Around_plate_calculate_circle()
{
	for(i=0;i<field_xrange;i++)
	{
		for(j=0;j<field_yrange;j++)
		{
			double jud=dA*(double)(((i-field_centerx)*(i-field_centerx))
								  +((j-field_centery)*(j-field_centery)));
			if(jud>Boundary_Radius*Boundary_Radius)
			around_plate[i][j]=1.0;
			else
			around_plate[i][j]=0.0;
		}
	}
}

void FiD::Anus_calculate_center()
{
	for(i=0;i<field_xrange;i++)
	{
		for(j=0;j<field_yrange;j++)
		{
			double jud=dA*(double)(((i-field_centerx)*(i-field_centerx))
								  +((j-field_centery)*(j-field_centery)));
			if(jud<Anus_Radius*Anus_Radius)
			{
				anus[i][j]=1.0;
				divide_field[i][j]=1;
			}
			else
			anus[i][j]=0.0;
		}
	}
}

void FiD::Fai_calculate()
{
	if(plate_plot)
	{
		for(i=0;i<field_xrange;i++)
		{
			for(j=0;j<field_yrange;j++)
			fai1[i][j]=0.0;
		}
		for(i=0;i<pn;i++)
		per[i].Fai_calculation(divide_field,fai,fai1);
	}
	for(i=0;i<field_xrange;i++)
	{
		for(j=0;j<field_yrange;j++)
		fai[i][j]=0.0;
	}
	for(i=0;i<pn;i++)
	per[i].Fai_calculation(divide_field,fai);
}

void FiD::Plate_create(double t)
{
	bool fin=false;
	int judge_range=plate_create_radius+plate_create_donut_range;
	for(i=field_centerx-judge_range;i<=field_centerx+judge_range;i++)
	{
		for(j=field_centery-judge_range;j<=field_centery+judge_range;j++)
		{
			if(!fin)
			{
				int jud=(i-field_centerx)*(i-field_centerx)
						+(j-field_centery)*(j-field_centery);
				if(jud>plate_create_radius*plate_create_radius
				&& jud<judge_range*judge_range)
				{
					if(fai[i][j]<=threshold_plate)
					{
						if((fai[i][j]<fai[i+1][j] && fai[i][j]<fai[i-1][j])
						|| (fai[i][j]<fai[i][j+1] && fai[i][j]<fai[i][j-1]))
						{
							pld.Get(pn+1,t,i*dr,j*dr);
							cout<<"t="<<t<<",pn="<<pn<<endl;
							per.push_back(pld);
							pn=(int)per.size();
							fin=true;
						}
					}
				}
			}
		}
	}
}

void FiD::Plate_create2(double t)
{
	int ic=boundary_radius*2/initial_plate_radius+1;
	int box[ic][ic];
	for(i=0;i<ic;i++)
	{
		for(j=0;j<ic;j++)
		box[i][j]=0;
	}
	for(i=field_centerx-boundary_radius,k=0;i<=field_centerx+boundary_radius;i+=initial_plate_radius,k++)
	{
		for(j=field_centery-boundary_radius,l=0;j<=field_centery+boundary_radius;j+=initial_plate_radius,l++)
		{
			for(int ii=i;ii<i+initial_plate_radius;ii++)
			{
				for(int jj=j;jj<j+initial_plate_radius;jj++)
				{
					if(fai[ii][jj]+around_plate[ii][jj]+anus[ii][jj]<=threshold_plate)
					box[k][l]++; 
				}
			}
		}
	}
	for(i=0;i<ic-1;i++)
	{
		for(j=0;j<ic-1;j++)
		{
			if((box[i][j]+box[i+1][j]+box[i][j+1]+box[i+1][j+1])*dA>Initial_Plate_Area)
			{
				//cout<<"A="<<(box[i][j]+box[i+1][j]+box[i][j+1]+box[i+1][j+1])*dA<<endl;
				pld.Get(pn+1,t,dr*(field_centerx-boundary_radius+initial_plate_radius*(i+1)),
						dr*(field_centery-boundary_radius+initial_plate_radius*(j+1)));
				cout<<"t="<<t<<",pn="<<pn
					<<",x="<<dr*(field_centerx-boundary_radius+initial_plate_radius*(i+1))
					<<",y="<<dr*(field_centery-boundary_radius+initial_plate_radius*(j+1))
					<<",r="<<dr*sqrt((field_centerx-boundary_radius+initial_plate_radius*(i+1))*(field_centerx-boundary_radius+initial_plate_radius*(i+1))
							+(field_centery-boundary_radius+initial_plate_radius*(j+1))*(field_centery-boundary_radius+initial_plate_radius*(j+1)))<<endl;
				//cout<<"anus="<<anus[field_centerx-boundary_radius+initial_plate_radius*(i+1)][field_centery-boundary_radius+initial_plate_radius*(j+1)]<<endl;
				per.push_back(pld);
				pn=(int)per.size();
				box[i][j]=0;
				box[i+1][j]=0;
				box[i][j+1]=0;
				box[i+1][j+1]=0;
				i+=ic;
				j+=ic;
			}
		}
	}
	return;
}
////////////////////////////////////////////////////////////////////////////////

//Input

////////////////////////////////////////////////////////////////////////////////
void FiD::Input_data_to_file()
{
	if(data_input_PF)
	{
		bool fno1=true,fno2=true,fno3=true,fno4=true;
		fstream file_data1,file_data2,file_data3,file_data4;
		stringstream filename_data1,filename_data2,filename_data3,filename_data4;
		filename_data1<<"../plot/plate_1_data.dat";//
		filename_data2<<"../plot/plate_2_data.dat";//
		filename_data3<<"../plot/plate_3_data.dat";//
		filename_data4<<"../plot/plate_4_data.dat";//
		////////////////////////////////////////////////////data_file1
		file_data1.open(filename_data1.str().c_str(),ios::out);
		if(!file_data1)
		{
			cerr<<"file_data1 open error"<<endl;
			fno1=false;
			//return 0;
		}
		if(fno1)
		{
			file_data1<<"plate number"<<"\t"
					  <<"plate create time"<<"\t"
					  <<"plate distance"<<"\t"
					  <<"plate area"<<"\t"
					  <<"plate first angle"<<"\t"
					  <<"plate angle"<<"\t"
					  <<"first delta angle"<<"\t"
					  <<"delta angle"<<endl;
			for(i=0;i<pn;i++)
			{
				double delta_angle,first_delta_angle;
				if(i>0)
				delta_angle=per[i].Show_Angle()-per[i-1].Show_Angle();
				else
				delta_angle=0.0;
				
				if(i>0)
				first_delta_angle=per[i].Show_Angle_First()-per[i-1].Show_Angle_First();
				else
				first_delta_angle=0.0;
				
				
				file_data1<<i<<"\t"<<per[i].Show_Create_Time()<<"\t"
					 <<per[i].Show_Distance()<<"\t"
					 <<per[i].Show_Area()<<"\t"
					 <<per[i].Show_Angle_First()<<"\t"
					 <<per[i].Show_Angle()<<"\t";
				if(first_delta_angle>0.0)
				file_data1<<first_delta_angle<<"\t";
				else
				file_data1<<360.0+first_delta_angle<<"\t";
				
				if(delta_angle>0.0)
				file_data1<<delta_angle<<endl;
				else
				file_data1<<360.0+delta_angle<<endl;
			}
		}
		file_data1.close();
		cout<<"file_data1_input is finished"<<endl;
		
		////////////////////////////////////////////////////data_file2
		file_data2.open(filename_data2.str().c_str(),ios::out);
		if(!file_data2)
		{
			cerr<<"file_data2 open error"<<endl;
			fno2=false;
			//return 0;
		}
		file_data3.open(filename_data3.str().c_str(),ios::out);
		if(!file_data3)
		{
			cerr<<"file_data3 open error"<<endl;
			fno3=false;
			//return 0;
		}
		file_data4.open(filename_data4.str().c_str(),ios::out);
		if(!file_data4)
		{
			cerr<<"file_data4 open error"<<endl;
			fno4=false;
			//return 0;
		}
		if(fno2 && fno3 && fno4)
		{
			file_data2<<"plate number"<<"\t"<<"ax"<<"\t"<<"ay"<<"\t\t"<<"distance between each plates"<<"\t";
			for(i=0;i<pn;i++)
			file_data2<<i<<"\t";
			file_data2<<"\t"<<"1st"<<"\t"<<"2nd"<<"\t"<<"3rd"<<"\t"<<"4th"<<endl;
			
			vector<pair<int,int> > segs;
			for(i=0;i<pn;i++)
			{
				vector<pair<double,int> > pairs(pn);
				file_data2<<i<<"\t"<<per[i].Show_Ax()<<"\t"<<per[i].Show_Ay()<<"\t\t"<<i<<"\t";
				for(j=0;j<pn;j++)
				{
					double distance_between_each_plates=
								sqrt((per[i].Show_Ax()-per[j].Show_Ax())*(per[i].Show_Ax()-per[j].Show_Ax())
								+(per[i].Show_Ay()-per[j].Show_Ay())*(per[i].Show_Ay()-per[j].Show_Ay()));
					file_data2<<distance_between_each_plates<<"\t";
					pairs[j]=make_pair(distance_between_each_plates,j);
				}
				sort(pairs.begin(),pairs.end());
				file_data2<<"\t"<<pairs[1].second<<"\t"<<pairs[2].second<<"\t"<<pairs[3].second<<"\t"<<pairs[4].second<<"\t\t";
				
				int count=0;
				for(j=1;j<5;j++)
				{
					
					if(j==3 && count>0)
					break;
					double angle;
					double px=per[pairs[j].second].Show_Ax()-per[i].Show_Ax();
					double py=per[pairs[j].second].Show_Ay()-per[i].Show_Ay();
					double theta1=per[i].Show_Angle();
					double theta2=Angle(px,py);
					if(theta1<0.0 && theta2>0.0)
					angle=theta1-theta2+360.0;
					else
					angle=theta1-theta2;
					if(angle>0.0 && angle<90.0)
					{
						count++;
						pair<int,int> seg(i,pairs[j].second);
						segs.push_back(seg);
						if(count>1)
						cout<<"A paraciticy diverge, the number is "<<count<<endl;
					}
					if(j==4 && count==0)
					{
						pair<int,int> seg(i,-1);
						segs.push_back(seg);
					}
				}
				file_data2<<endl;
			}
			file_data2<<endl<<"plate number"<<"\t"<<"clockwise plate number"<<endl;
			vector<vector<int> > parastichies;
			int discount=0;
			for(i=0;i<(int)segs.size();i++)
			{
				file_data2<<segs[i].first<<"\t"<<segs[i].second<<endl;
				bool exist=false;
				for(j=0;j<(int)parastichies.size();j++)
				{
					IV::iterator end=parastichies[j].end();
					if(*end==segs[i].first)
					{
						parastichies[j].push_back(segs[i].second);
						exist=true;
					}
				}
				if(!exist)
				{
					if(segs[i].second!=-1)
					{
						IV para(segs[i].first,segs[i].second);
						parastichies.push_back(para);
					}
					else
					discount++;
				}
			}
			file_data2<<endl<<"parastichy anus end"<<"\t"<<"->"<<"\t"<<"around plates end"<<endl;
			for(i=0;i<(int)parastichies.size();i++)
			{
				for(j=0;j<(int)parastichies[i].size();j++)
				file_data2<<parastichies[i][j]<<"\t";
				file_data2<<endl;
			}
			file_data2<<endl;
			file_data2<<"number of parastichies="<<"\t"<<parastichies.size()<<endl;
			file_data2<<"discount, for example edge="<<"\t"<<discount;
		
			////////////////////////////////////////////////////file_data3
			file_data3<<"plate number"<<"\t"<<"x"<<"\t"<<"y"<<endl;
			for(i=0;i<pn;i++)
			file_data3<<i<<"\t"<<per[i].Show_Centerx()<<"\t"<<per[i].Show_Centery()<<endl;
			file_data3<<endl;
			////////////////////////////////////////////////////file_data4
			cout<<"parastichies.size()="<<parastichies.size()<<endl;
			for(i=0;i<(int)parastichies.size();i++)
			{
				file_data4<<endl;
				cout<<"parastichies["<<i<<"].size()="<<parastichies[i].size()<<endl;
				for(j=0;j<(int)parastichies[i].size();j++)
				file_data4<<per[parastichies[i][j]].Show_Centerx()<<"\t"<<per[parastichies[i][j]].Show_Centery()<<endl;
				file_data4<<endl;
			}	
		}
		file_data2.close();
		file_data3.close();
		file_data4.close();
		cout<<"file_data2,3,4_input is finished"<<endl;
			
		////////////////////////////////////////////////////
		/*CGnuplot gnu;
		gnu.Gcom("set size square\n");
		gnu.Gcom("set xrange [%f:%f]\n",Field_Centerx-Field_Xrange*0.5-1.0,Field_Centerx+Field_Xrange*0.5+1.0);
		gnu.Gcom("set yrange [%f:%f]\n",Field_Centery-Field_Yrange*0.5-1.0,Field_Centery+Field_Yrange*0.5+1.0);
		gnu.Gcom("unset key\n");
		if(!display_PF)
		{
			gnu.Gcom("set terminal png\n");
			gnu.Gcom("set output '.././anime/parastichies_plot.png'\n");
		}
		gnu.Gcom("plot '.././plot/plate_3_data.dat' u 2:3 w p\n");
		gnu.Gcom("replot '.././plot/plate_4_data.dat' u 1:2 w lp\n");
		gnu.Gflush();
		cout<<"parastichies_plot is finished"<<endl;
		*/
		
	}
}
////////////////////////////////////////////////////////////////////////////////

//Timechange

////////////////////////////////////////////////////////////////////////////////
void FiD::Timechange()
{
	bool fno1=true,fno2=true;
	CGnuplot gnu1;
	fstream file,file1,file2;
	stringstream filename,filename1,filename2;
	filename<<"../plot/plate_All.dat";
	filename1<<"../plot/plate_info.dat";
	filename2<<"../plot/plate_1.dat";
	gnu1.Gcom("set pm3d map interpolate 0.0,0.0\n");
	gnu1.Gcom("set size square\n");
	gnu1.Gcom("set cbrange [0:1]\n");
	gnu1.Gcom("set palette defined (0 'white',1 'red')\n");
	gnu1.Gcom("set xrange [%f:%f]\n",Field_Centerx-Field_Xrange*0.5-1.0,Field_Centerx+Field_Xrange*0.5+1.0);
	gnu1.Gcom("set yrange [%f:%f]\n",Field_Centery-Field_Yrange*0.5-1.0,Field_Centery+Field_Yrange*0.5+1.0);
	gnu1.Gcom("unset key\n");
	if(display_PF)
	gnu1.Gcom("set terminal windows\n");
	else
	gnu1.Gcom("set terminal png\n");
	
	file1.open(filename1.str().c_str(),ios::out);
	if(!file1)
	{
		cerr<<"file open error timechange"<<endl;
		fno2=false;
	}
	file1<<"time"<<"\t"<<"plate_id"<<"\t"<<"plate_create_time"<<"\t"
		 <<"distance"<<"\t"
		 <<"Area"<<"\t"<<"V"<<"\t"
		 <<"create_x"<<"\t"<<"create_y"<<"\t"
		 <<"Plate_Centerx"<<"\t"<<"Plate_Centery"<<"\t"
		 <<"First angle"<<"\t"
		 <<"ax"<<"\t"<<"ay"<<endl;
	
	if(fno2)
	{
		for(ft=1,t=dt;t<tmax;t+=dt,ft++,Boundary_Radius+=dr_boundary,boundary_radius=(int)(Boundary_Radius/dr))
		{
			Calculate(t,ft);
			if(!(ft%ftmax))
			{
				for(i=0;i<pn;i++)
				file1<<t<<"\t"<<per[i].Show_Plate_Id()<<"\t"<<per[i].Show_Create_Time()<<"\t"
				<<per[i].Show_Distance()<<"\t"
				<<per[i].Show_Area()<<"\t"<<per[i].Show_V()<<"\t"
				<<per[i].Show_Create_X()<<"\t"<<per[i].Show_Create_Y()<<"\t"
				<<per[i].Show_Centerx()<<"\t"<<per[i].Show_Centery()<<"\t"
				<<per[i].Show_Angle_First()<<"\t"
				<<per[i].Show_Ax()<<"\t"<<per[i].Show_Ay()<<endl;
				file1<<endl;
				file.open(filename.str().c_str(),ios::out);
				if(!file)
				{
					cerr<<"file open error timechange"<<endl;
					fno1=false;
					//return 0;
				}
				if(plate_plot)
				{
					file2.open(filename2.str().c_str(),ios::out);
					if(!file2)
					{
						cerr<<"file2 open error timechange"<<endl;
						fno1=false;
						//return 0;
					}
				}
				if(fno1)
				{
					for(i=0;i<field_xrange;i++)
					{
						for(j=0;j<field_yrange;j++)
						{
							//if(i==field_xrange-1 && j==field_yrange-1)
							//cout<<"fin last"<<endl;
							file<<i<<"\t"<<j<<"\t"<<(double)i*dr<<"\t"<<(double)j*dr<<"\t"<<fai[i][j]+around_plate[i][j]+anus[i][j]<<endl;
							if(plate_plot)
							file2<<i<<"\t"<<j<<"\t"<<(double)i*dr<<"\t"<<(double)j*dr<<"\t"<<fai1[i][j]<<endl;
						}
						file<<endl;
						if(plate_plot)
						file2<<endl;
					}
				}
				file.close();
				if(plate_plot)
				file2.close();
				gnu1.Gcom("set title 't=%f,pn=%d'\n",t,pn);
				gnu1.Gcom("splot '.././plot/plate_All.dat' u 3:4:5 w pm3d\n");
				if(!display_PF)
				gnu1.Gcom("set output '.././anime_plot/t=%f.png'\n",t);
				gnu1.Gflush();
				if(plate_plot)
				{
					gnu1.Gcom("set title 't=%f,pn=%d'\n",t,pn);
					gnu1.Gcom("splot '.././plot/plate_1.dat' u 3:4:5 w pm3d\n");
					if(!display_PF)
					gnu1.Gcom("set output '.././anime_plot/t=%f,plate1.png'\n",t);
					gnu1.Gflush();
				}
			}
		}
		Input_data_to_file();
	}
}