#include "stdafx.h"

int ICM(CImage &res,CImage &des,int k,double M)
{
#define ONLYICM
	des  = res;
	int image_bpp = des.GetBPP();
	int width = des.GetWidth();
	int height = des.GetHeight();
	vector<vector<float>> lab;
	vector<double>grad;
	vector<int>fenClass_c;
	Image2Lab(des,lab);
	
	//Avg_n2(lab,width,height,1);
#ifndef ONLYICM
	GetImageGrad(lab,grad,width,height);
	SLIC_Core(lab,grad,fenClass_c,width,height,k,M);
	MergeClassByColor(lab,fenClass_c,width,height,k);
	delete_empty_calss(fenClass_c,width,height,k);
#else
	n_avg_2_k_(fenClass_c,width,height,k);
#endif
	ICM_Core(lab,fenClass_c,width,height,k);
	//ICM_CoreUseEigen(lab,fenClass_c,width,height,k);
	MergeClassByColor(lab,fenClass_c,width,height,k);
	delete_empty_calss(fenClass_c,width,height,k);
	DrawBorder1(des,fenClass_c,width,height);
	//DrawBorderWidthAvgColor(des,fenClass_c,width,height);
	//DrawBorderOnlyLine(des,fenClass_c,width,height);
	//DrawBorderWithColor(des,fenClass_c,k,width,height);
	return 0;
}