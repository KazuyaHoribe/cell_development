#pragma once
#include "data_SN.cpp"
#include "atom.cpp"
#include"connect.cpp"
#include "norm.cpp"

typedef vector<double> DV;
typedef vector<vector<double> > DVV;
typedef vector<vector<vector<double> > > DVVV;


class InD:public DaD
{
private:
	vector<vector<AtD> > net;//三角メッシュの頂点の場
	vector<NoD> nods;//ここからnormの設定
	vector<CoD> cods;
	vector<int> AFI;//z方向に力をかける点（Anus周辺）のID
	DVVV dvvv;//ファイルを読み込み、分割したデータ列(各頂点の座標)のデータ列(全ての頂点)
	
public:
	InD();
	~InD();
	InD(const InD& ind);
	InD& operator=(const InD& ind);
	void Initialize();
	void Connection();
	bool Judge_i(int ni);
	bool Judge_j(int nj);
	void Calculate(double t);
	void Simulate();
};

InD::InD()
{
	Initialize();
	Connection();
}

InD::~InD()
{
}

InD::InD(const InD& ind)
{
}

InD& InD::operator=(const InD& ind)
{
}
////////////////////////////////////////////////////////////////////////////////

//Initialize　点をセットして、各点をつなげて、PFの結果に近似して、計算する

////////////////////////////////////////////////////////////////////////////////
void InD::Initialize()
{
	//////////netのリサイズ
	net.resize(height_num);
	for(i=0;i<height_num;i++)
	net[i].resize(range_num);
	//////////netの初期化
	for(i=0;i<height_num;i++)
	{
		for(j=0;j<range_num;j++)
		net[i][j].Where(j*sen_L+(i%2)*sen_L*0.5,i*sen_l,0.0);
	}
	
	if(Periproct_Form)
	{
		for(i=0;i<height_num;i++)
		{
			for(j=0;j<range_num;j++)
			{
				double judge_range=(Field_Centerx-net[i][j].r[0])*(Field_Centerx-net[i][j].r[0])+
								   (Field_Centery-net[i][j].r[1])*(Field_Centery-net[i][j].r[1]);
				if(judge_range>boundary_radius*boundary_radius)//頂上系はStatus=2
				net[i][j].Which_Status(2);
				if(judge_range<anus_radius*anus_radius)//肛門はStatus=3
				net[i][j].Which_Status(3);
				else if(judge_range<(anus_radius+sen_L)*(anus_radius+sen_L))
				AFI.push_back(i),AFI.push_back(j);
			}
		}
	}
	cout<<"AFI_size="<<AFI.size()<<endl;
	/*
	net[4][4].Which_Status(1);
	net[5][4].Which_Status(1);
	net[4][5].Which_Status(1);
	net[5][5].Which_Status(1);
	*/
	
	
	//net[0][0].r[2]=2.0*0.1;
	
	//net[4][4].r[2]=0.2;
	//net[5][4].r[2]=0.2;
	//net[6][4].r[2]=2.0*0.5;
	//net[4][5].r[2]=0.2;
	//net[5][5].r[2]=0.2;
	//net[6][5].r[2]=2.0*0.5;
	//net[4][6].r[2]=2.0*0.5;
	//net[5][6].r[2]=2.0*0.5;
	//net[6][6].r[2]=2.0*0.5;
	
	//////////cods,lidsのリサイズ
	cods.resize(cods_num);
	nods.resize(nods_num);
	
	ifstream ifs("../data_SN/uni1_data.csv");
	dvvv=CDSD(ifs);
	
	for(i=0;i<height_num;i++)
	{
		for(j=0;j<range_num;j++)
		{
			for(k=0;k<dvvv.size();k++)
			{
				Jio jio(dvvv[k],net[i][j].Show_2D_Coordinate());
				if(jio.Wind()>0)
				net[i][j].Which_Status(1);
			}
		}
	}
}

void InD::Connection()
{
	for(i=0,k=0;i<height_num-1;i++)
	{
		for(j=0;j<range_num-1;j++)
		{
			if(!(i%2))//iが偶数の時
			{
				if((!(net[i+1][j].Show_Status()==2 && net[i][j+1].Show_Status()==2))
					&& (net[i][j].Show_Status()!=3) && (net[i+1][j].Show_Status()!=3)
					&& (net[i][j+1].Show_Status()!=3) && (net[i+1][j+1].Show_Status()!=3))
				{
					nods[k].Norm(i,j,i+1,j,i,j+1,i+1,j+1,
								net[i+1][j].Show_Status(),net[i][j+1].Show_Status());
					k++;
				}
				if(i<height_num-2)
				{
					if((!(net[i+1][j].Show_Status()==2 && net[i+1][j+1].Show_Status()==2))
						&& (net[i][j+1].Show_Status()!=3) && (net[i+1][j].Show_Status()!=3)
						&& (net[i+1][j+1].Show_Status()!=3) && (net[i+2][j+1].Show_Status()!=3))
					{
						nods[k].Norm(i,j+1,i+1,j,i+1,j+1,i+2,j+1,
									net[i+1][j].Show_Status(),net[i+1][j+1].Show_Status());
						k++;
					}
				}
				if(j<range_num-2)
				{
					if((!(net[i+1][j+1].Show_Status()==2 && net[i][j+1].Show_Status()==2))
						&& (net[i+1][j].Show_Status()!=3) && (net[i+1][j+1].Show_Status()!=3)
						&& (net[i][j+1].Show_Status()!=3) && (net[i][j+2].Show_Status()!=3))
					{
						nods[k].Norm(i+1,j,i+1,j+1,i,j+1,i,j+2,
									net[i+1][j+1].Show_Status(),net[i][j+1].Show_Status());
						k++;
					}
				}
			}
			else//iが奇数の時
			{
				if((!(net[i+1][j+1].Show_Status()==2 && net[i][j].Show_Status()==2))
					&& (net[i+1][j].Show_Status()!=3) && (net[i+1][j+1].Show_Status()!=3)
					&& (net[i][j].Show_Status()!=3) && (net[i][j+1].Show_Status()!=3))
				{
					nods[k].Norm(i+1,j,i+1,j+1,i,j,i,j+1,
								net[i+1][j+1].Show_Status(),net[i][j].Show_Status());
					k++;
				}
				if(i<height_num-2)
				{
					if((!(net[i+1][j].Show_Status()==2 && net[i+1][j+1].Show_Status()==2))
						&& (net[i][j].Show_Status()!=3) && (net[i+1][j].Show_Status()!=3)
						&& (net[i+1][j+1].Show_Status()!=3) && (net[i+2][j].Show_Status()!=3))
					{
						nods[k].Norm(i,j,i+1,j,i+1,j+1,i+2,j,
									net[i+1][j].Show_Status(),net[i+1][j+1].Show_Status());
						k++;
					}
				}
				if(j<range_num-2)
				{
					if((!(net[i+1][j+1].Show_Status()==2 && net[i][j+1].Show_Status()==2))
						&& (net[i][j].Show_Status()!=3) && (net[i+1][j+1].Show_Status()!=3)
						&& (net[i][j+1].Show_Status()!=3) && (net[i+1][j+2].Show_Status()!=3))
					{
						nods[k].Norm(i,j,i+1,j+1,i,j+1,i+1,j+2,
									net[i+1][j+1].Show_Status(),net[i][j+1].Show_Status());
						k++;
					}
				}
			}
		}
	}
	
	if(k!=nods_num)
	nods_num=k;
	for(i=0,k=0;i<height_num;i++)
	{
		for(j=0;j<range_num;j++)
		{
			if(Judge_j(j+1))
			{
				if((!(net[i][j].Show_Status()==2 && net[i][j+1].Show_Status()==2))
					&& (net[i][j].Show_Status()!=3) && (net[i][j+1].Show_Status()!=3))
				{
					cods[k].Connect(i,j,i,j+1,net[i][j].Show_Status(),net[i][j+1].Show_Status());
					k++;
				}
			}
			if(Judge_i(i+1))
			{
				if((!(net[i][j].Show_Status()==2 && net[i+1][j].Show_Status()==2))
					&& (net[i][j].Show_Status()!=3) && (net[i+1][j].Show_Status()!=3))
				{
					cods[k].Connect(i,j,i+1,j,net[i][j].Show_Status(),net[i+1][j].Show_Status());
					k++;
				}
			}
			if(i%2)
			{
				if(Judge_i(i+1) && Judge_j(j+1))
				{
					if((!(net[i][j].Show_Status()==2 && net[i+1][j+1].Show_Status()==2))
						&& (net[i][j].Show_Status()!=3) && (net[i+1][j+1].Show_Status()!=3))
					{
						cods[k].Connect(i,j,i+1,j+1,net[i][j].Show_Status(),net[i+1][j+1].Show_Status());
						k++;
					}
				}
			}
			else
			{
				if(Judge_i(i+1) && Judge_j(j-1))
				{
					if((!(net[i][j].Show_Status()==2 && net[i+1][j-1].Show_Status()==2))
						&& (net[i][j].Show_Status()!=3) && (net[i+1][j-1].Show_Status()!=3))
					{
						cods[k].Connect(i,j,i+1,j-1,net[i][j].Show_Status(),net[i+1][j-1].Show_Status());
						k++;
					}
				}
			}
		}
	}
	if(k!=cods_num)
	cods_num=k;
}

void InD::Calculate(double t)
{
	
	for(i=0;i<cods_num;i++)
	{
		cods[i].Spring(net[cods[i].Show_Atom(0,0)][cods[i].Show_Atom(0,1)].r[0],
					   net[cods[i].Show_Atom(0,0)][cods[i].Show_Atom(0,1)].r[1],
					   net[cods[i].Show_Atom(0,0)][cods[i].Show_Atom(0,1)].r[2],
					   net[cods[i].Show_Atom(1,0)][cods[i].Show_Atom(1,1)].r[0],
					   net[cods[i].Show_Atom(1,0)][cods[i].Show_Atom(1,1)].r[1],
					   net[cods[i].Show_Atom(1,0)][cods[i].Show_Atom(1,1)].r[2]);
		for(j=0;j<2;j++)
		net[cods[i].Show_Atom(j,0)][cods[i].Show_Atom(j,1)].Get_Mr(cods[i].Show_Mr(j,0),cods[i].Show_Mr(j,1),cods[i].Show_Mr(j,2));
	}
	
	for(i=0;i<nods_num;i++)
	{
		nods[i].Parallel(net[nods[i].Show_Atom(0,0)][nods[i].Show_Atom(0,1)].r[0],
						 net[nods[i].Show_Atom(0,0)][nods[i].Show_Atom(0,1)].r[1],
						 net[nods[i].Show_Atom(0,0)][nods[i].Show_Atom(0,1)].r[2],
						 net[nods[i].Show_Atom(1,0)][nods[i].Show_Atom(1,1)].r[0],
						 net[nods[i].Show_Atom(1,0)][nods[i].Show_Atom(1,1)].r[1],
						 net[nods[i].Show_Atom(1,0)][nods[i].Show_Atom(1,1)].r[2],
						 net[nods[i].Show_Atom(2,0)][nods[i].Show_Atom(2,1)].r[0],
						 net[nods[i].Show_Atom(2,0)][nods[i].Show_Atom(2,1)].r[1],
						 net[nods[i].Show_Atom(2,0)][nods[i].Show_Atom(2,1)].r[2],
						 net[nods[i].Show_Atom(3,0)][nods[i].Show_Atom(3,1)].r[0],
						 net[nods[i].Show_Atom(3,0)][nods[i].Show_Atom(3,1)].r[1],
						 net[nods[i].Show_Atom(3,0)][nods[i].Show_Atom(3,1)].r[2]);
		for(j=0;j<4;j++)
		net[nods[i].Show_Atom(j,0)][nods[i].Show_Atom(j,1)].Get_Mr(nods[i].Show_Mr(j,0),nods[i].Show_Mr(j,1),nods[i].Show_Mr(j,2));
	}
	if(t<tmax)
	{
		//net[1][1].mr[2]+=force/1000.0;
		for(i=0;i<AFI.size()*0.5;i++)
		net[AFI[2*i]][AFI[2*i+1]].mr[2]+=force;
	}
	
	//double U=0.0;
	for(i=0;i<height_num;i++)
	{
		for(j=0;j<range_num;j++)
		{
			//U+=(net[i][j].mr[0]+net[i][j].mr[1]+net[i][j].mr[2]);
			//cout<<"Ux["<<i<<"]["<<j<<"]="<<net[i][j].mr[0]<<",Uy["<<i<<"]["<<j<<"]="<<net[i][j].mr[1]<<",Uz["<<i<<"]["<<j<<"]="<<net[i][j].mr[2]<<endl;
			if(net[i][j].Show_Status()!=2)
			net[i][j].Move_Atom();
		}
	}
	//cout<<"U="<<U<<endl;
}

bool InD::Judge_i(int ni)
{
	bool go=false;
	if(ni>=0 && ni<height_num)
	go=true;
	return go;
}

bool InD::Judge_j(int nj)
{
	bool go=false;
	if(nj>=0 && nj<range_num)
	go=true;
	return go;
}
////////////////////////////////////////////////////////////////////////////////

//Simulate

////////////////////////////////////////////////////////////////////////////////
void InD::Simulate()
{
	CGnuplot gnu;
	fstream file_scalar,file_vector;
	stringstream filename_scalar,filename_vector;
	filename_scalar<<".././plot/seung_scalar.dat";
	filename_vector<<".././plot/seung_vector.dat";
	gnu.Gcom("set size square\n");
	gnu.Gcom("set xrange [%f:%f]\n",-0.2,Field_Xrange+0.2);
	gnu.Gcom("set yrange [%f:%f]\n",-0.2,Field_Yrange+0.2);
	gnu.Gcom("set zrange [%f:%f]\n",-0.2,0.5);
	gnu.Gcom("set xlabel 'x'\n");
	gnu.Gcom("set ylabel 'y'\n");
	gnu.Gcom("set zlabel 'z'\n");
	gnu.Gcom("unset key\n");
	
	for(ft=0,t=0.0;t<tmax;ft++,t+=dt)
	{
		Calculate(t);
		if(!(ft%ftmax))
		{
			file_scalar.open(filename_scalar.str().c_str(),ios::out);
			if(!file_scalar)
			{
				cerr<<"seung file_scalar open error"<<endl;
				//return 0;
			}
			
			file_vector.open(filename_vector.str().c_str(),ios::out);
			if(!file_vector)
			{
				cerr<<"seung file_vector open error"<<endl;
				//return 0;
			}
			////////////////////////////codsでプロット
			if(Cods_Plot)
			{
				for(i=0;i<cods_num;i++)
				{
					file_scalar<<endl<<endl;
					for(j=0;j<2;j++)
					{
						for(k=0;k<3;k++)
						file_scalar<<net[cods[i].Show_Atom(j,0)][cods[i].Show_Atom(j,1)].r[k]<<"\t";
						file_scalar<<endl;
					}
				}
			}
			////////////////////////////nodsでプロット
			else if(Nods_Plot)
			{
				for(i=0;i<nods_num;i++)
				{
					file_scalar<<endl;
					//file_scalar<<i<<"\t";
					for(j=0;j<3;j++)
					file_scalar<<net[nods[i].Show_Atom(1,0)][nods[i].Show_Atom(1,1)].r[j]<<"\t";
					file_scalar<<endl;
					//file<<i<<"\t";
					for(j=0;j<3;j++)
					file_scalar<<net[nods[i].Show_Atom(2,0)][nods[i].Show_Atom(2,1)].r[j]<<"\t";
					file_scalar<<endl;
					//file_scalar<<i<<"\t";
					for(j=0;j<3;j++)
					file_scalar<<net[nods[i].Show_Atom(3,0)][nods[i].Show_Atom(3,1)].r[j]<<"\t";
					file_scalar<<endl;
					//file_scalar<<i<<"\t";
					for(j=0;j<3;j++)
					file_scalar<<net[nods[i].Show_Atom(1,0)][nods[i].Show_Atom(1,1)].r[j]<<"\t";
					file_scalar<<endl;
					//file_scalar<<i<<"\t";
					for(j=0;j<3;j++)
					file_scalar<<net[nods[i].Show_Atom(0,0)][nods[i].Show_Atom(0,1)].r[j]<<"\t";
					file_scalar<<endl;
					//file_scalar<<i<<"\t";
					for(j=0;j<3;j++)
					file_scalar<<net[nods[i].Show_Atom(2,0)][nods[i].Show_Atom(2,1)].r[j]<<"\t";
					file_scalar<<endl<<endl;
				}
			}
			////////////////////////////Normの2つの三角形の連結部をプロット(確認)
			if(Center_Norm_Plot)
			{
				for(i=0;i<nods_num;i++)
				{
					double center[2][3];
					file_scalar<<endl;
					for(j=0;j<2;j++)
					{
						for(k=0;k<3;k++)
						center[j][k]=(net[nods[i].Show_Atom(0+j,0)][nods[i].Show_Atom(0+j,1)].r[k]
									 +net[nods[i].Show_Atom(1+j,0)][nods[i].Show_Atom(1+j,1)].r[k]
									 +net[nods[i].Show_Atom(2+j,0)][nods[i].Show_Atom(2+j,1)].r[k])/3.0;
					}
					for(j=0;j<2;j++)
					{
						for(k=0;k<3;k++)
						file_scalar<<center[j][k]<<"\t";
						file_scalar<<endl;
					}
				}
			}
			////////////////////////////ベクトルをプロット
			if(Vector_Plot)
			{
				for(i=0;i<cods_num;i++)
				{
					for(j=0;j<2;j++)
					{
						for(k=0;k<3;k++)
						file_vector<<net[cods[i].Show_Atom(j,0)][cods[i].Show_Atom(j,1)].r[k]<<"\t";
						for(k=0;k<3;k++)
						{
							if(abs(net[cods[i].Show_Atom(j,0)][cods[i].Show_Atom(j,1)].mr[k])>0.001)
							file_vector<<net[cods[i].Show_Atom(j,0)][cods[i].Show_Atom(j,1)].mr[k]<<"\t";//各値を長さに持つベクトル
							else
							file_vector<<0.0<<"\t";
						}
						file_vector<<endl;
					}
				}
			}
			
			file_scalar.close();
			file_vector.close();
			
			gnu.Gcom("set title 't=%f'\n",t);
			if(!display_SN)
			{
				gnu.Gcom("set terminal png\n");
				gnu.Gcom("set output '.././anime/t=%f.png'\n",t);
			}
			if(display_SN)
			gnu.Gcom("set terminal windows\n");
			gnu.Gcom("set ticslevel 0\n");
			if(!((!Cods_Plot) && (!Nods_Plot) && (!Center_Norm_Plot)))
			gnu.Gcom("splot '.././plot/seung_scalar.dat' u 1:2:3 w l lt 7 \n");
			//gnu.Gcom("plot '.././plot/seung_scalar.dat' u 1:2 w lp lt 7 \n");
			if(Vector_Plot)
			{
				//gnu.Gcom("splot '.././plot/seung_vector.dat' with vectors \n");
				gnu.Gcom("splot '.././plot/seung_vector.dat' u 1:2:4:5 with vectors \n");
			}
			//gnu.Gcom("replot\n");
			gnu.Gflush();
		}
		for(i=0;i<height_num;i++)
		{
			for(j=0;j<range_num;j++)
			net[i][j].Reset_Mr();
		}
	}
}
