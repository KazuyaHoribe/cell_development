#pragma once
#include <vector>
#include <fstream>
#include <string>
//#define GNUPLOT_PATH "/Applications/gnuplot.app/gnuplot"

class CGnuplot {
private:
	FILE* gp;
	
	void Gexit();

public:
	CGnuplot();
	~CGnuplot();
	CGnuplot(const CGnuplot& gnu);
	CGnuplot& operator=(const CGnuplot& gnu);
	bool Gcheck();
	void Gflush();
	void Gsystem();
	void Gcom(const char* format, ...);
	//void operator<<(const char* format, ...);
};