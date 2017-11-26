#pragma once
#include <cstdio>
#include <cstdlib>
#include <stdarg.h>
#include "ygnuplot.h"



CGnuplot::CGnuplot()
{
	//gp=_popen("pgnuplot", "w");
	gp=popen("pgnuplot", "w");
	if(gp==NULL)
	{
		printf("pipe error\n");
	}
}


CGnuplot::~CGnuplot()
{
	Gsystem();
	Gcom("exit\n");
	Gflush();
	//_pclose(gp);
	pclose(gp);
}

bool CGnuplot::Gcheck()
{
	if (gp==NULL)
	{
		return false;
	}
	return true;
}

void CGnuplot::Gcom(const char* format, ...)
{
	char buf[1024];
	va_list ap;
	
	va_start(ap, format);
	vsprintf(buf, format, ap);
	va_end(ap);
	
	fprintf(gp,"%s",buf);
	
	Gflush();
}
/*
void CGnuplot::operator<<(const char* format, ...)
{
	char buf[1024];
	va_list ap;
	
	va_start(ap, format);
	vsprintf(buf, format, ap);
	va_end(ap);
	
	fprintf(gp,"%s",buf);
	//Gflush();
}*/

void CGnuplot::Gsystem()
{
	//system("pause");
	Gcom("pause -1\n");
	fprintf(gp,"pause -1\n");
}

void CGnuplot::Gflush()
{
	fflush(gp);
}

void CGnuplot::Gexit()
{
	Gcom("exit\n");
}
