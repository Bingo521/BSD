#include "stdafx.h"

#define ONLYICM1
CImage debugImg;
int ICM(CImage &res,CImage &des,int k,double M)
{
	debugImg = res;

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
	//KMeans_Core(lab,fenClass_c,width,height,k,M);
	//MergeClassByColor(lab,fenClass_c,width,height,k);
	//delete_empty_calss(fenClass_c,width,height,k);
#else
	init_rand_2_k(fenClass_c,width,height,k);
#endif

	ICM_Core_(lab,fenClass_c,width,height,k);
	//ICM_CoreUseEigen(lab,fenClass_c,width,height,k);
	//MergeClassByColor(lab,fenClass_c,width,height,k);
	//delete_empty_calss(fenClass_c,width,height,k);
	DrawBorder1(des,fenClass_c,width,height);
	//DrawBorderWidthAvgColor(des,fenClass_c,width,height);
	//DrawBorderOnlyLine(des,fenClass_c,width,height);
	//DrawBorderWithColor(des,fenClass_c,k,width,height);
	return 0;
}

int ICM(vector<vector<float>> &lab,vector<int> &fenClass_c,int width,int height,int &k,double M)
{
	/*CString str;
	str.Format("%d",k);
	AfxMessageBox(str);*/

#ifndef ONLYICM
	/*vector<double>grad;
	GetImageGrad(lab,grad,width,height);
	SLIC_Core(lab,grad,fenClass_c,width,height,k,M);*/
	//KMeans_Core(lab,fenClass_c,width,height,k,M);
	//init_rand_2_k(fenClass_c,width,height,k);
	init_n_2_k(fenClass_c,width,height,k);
	KMeans_Core_(lab,fenClass_c,width,height,k,M);
	//MergeClassByColor(lab,fenClass_c,width,height,k);
	//delete_empty_calss(fenClass_c,width,height,k);
#else
	//init_n_2_k(fenClass_c,width,height,k);
	init_rand_2_k(fenClass_c,width,height,k);
#endif
	ICM_Core_(lab,fenClass_c,width,height,k);
	//MergeClassByColor(lab,fenClass_c,width,height,k);
	//delete_empty_calss(fenClass_c,width,height,k);
	return 1;
}
