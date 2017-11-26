#pragma once
#include <vector>
#include <fstream>
#include <string>
//#define GNUPLOT_PATH "C:/gnuplot/bin/pgnuplot.exe"

/*gnuplotを扱うライブラリ
gnuplotを扱うクラスに関するライブラリです．
pgnuplotへとパイプを繋ぎ，関数やデータをgnuplotを用いてプロットする手助けをします．
pgnuplot.exeが環境変数へと登録されている必要があります．
*/

class CGnuplot {
private:
	FILE* gp;
	
	
	void Gexit();

public:
	CGnuplot();
	~CGnuplot();
	bool Gcheck();
	void Gflush();
	void Gsystem();
	void Gcom(const char* format, ...);
	//void operator<<(const char* format, ...);
};