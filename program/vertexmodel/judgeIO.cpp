#pragma once
#include "judgeIO.h"

Jio::Jio(const DVV& ndata,const DV njp)
{
	data=ndata;
	jp=njp;
	size=data.size();
	cn=0;
	wn=0;
}

//p(i)→p(i+1)の線分ベクトルに対し(jp[0],jp[1])の点が左にあればtrue、右にあればfalse
bool Jio::Left(int i)
{
	bool left;
	double jud_num=(data[(i+1)%size][0]-data[i%size][0])*(jp[1]-data[i%size][1])
					-(jp[0]-data[i%size][0])*(data[(i+1)%size][1]-data[i%size][1]);
	if(jud_num>0.0)
	left=true;
	
	else if(jud_num<0.0)
	left=false;
	
	return left;
}

//Wind numberをカウント(内外判定の手法1)
int Jio::Wind()
{
	for(i=0;i<size;i++)
	{
		if((data[i%size][1]<=jp[1]) && (data[(i+1)%size][1]>jp[1]))
		{
			if(Left(i))
			++wn;
		}
		
		else if((data[i%size][1]>jp[1]) && (data[(i+1)%size][1]<=jp[1]))
		{
			if(!Left(i))
			--wn;
		}
	}
	//cout<<"judgeIO.Wind().wn="<<wn<<endl;
	return wn;
}

//Crossing numberをカウント(内外判定の手法2)
int Jio::Cross()
{
	for(i=0;i<size;i++)
	{
		if(((data[i%size][1]<=jp[1]) && (data[(i+1)%size][1]>jp[1]))
		|| ((data[i%size][1]>jp[1]) && (data[(i+1)%size][1]<=jp[1])))
		{
			vt=(jp[1]-data[i%size][1])/(data[(i+1)%size][1]-data[i%size][1]);
			if(jp[0]<data[i%size][0]+vt*(data[(i+1)%size][0]-data[i%size][0]))
			++cn;
		}
	}
	return (cn%2);
}



