#pragma once
#include <vector>
#include <fstream>
#include <string>
//#define GNUPLOT_PATH "C:/gnuplot/bin/pgnuplot.exe"

/*gnuplot���������C�u����
gnuplot�������N���X�Ɋւ��郉�C�u�����ł��D
pgnuplot�ւƃp�C�v���q���C�֐���f�[�^��gnuplot��p���ăv���b�g����菕�������܂��D
pgnuplot.exe�����ϐ��ւƓo�^����Ă���K�v������܂��D
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