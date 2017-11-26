#pragma once
#include <vector>
using namespace std;
typedef vector<double> DV;
typedef vector<vector<double> > DVV;


class Jio
{
private:
	int i,size,cn,wn;
	double vt;
	DV jp;
	DVV data;
	
public:
	Jio(const DVV& data,const DV njp);
	bool Left(int i);
	int Wind();/////////////////////////////////////////////�߂�l��0�̂Ƃ��O���A1�ȏ�̂Ƃ�����
	int Cross();////////////////////////////////////////////�߂�l��0�̂Ƃ��O���A1�̂Ƃ�����
};
