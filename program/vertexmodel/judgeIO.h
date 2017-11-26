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
	int Wind();/////////////////////////////////////////////戻り値が0のとき外側、1以上のとき内側
	int Cross();////////////////////////////////////////////戻り値が0のとき外側、1のとき内側
};
