#include "stdafx.h"

using namespace std;
int GMM(CImage &res,CImage &des,int k,double M)
{
	des  = res;
	//获取宽和高
	//vector<vector<vector<float>>> lab;
	//vector<vector<double>> grad;
	//vector<vector<int>>fenClass_c;
	//vector<int> point_num;
	//Image2Lab(des,lab);
	//GetImageGrad(lab,grad);
	//SLIC_Core(lab,grad,fenClass_c,point_num,k,M);
	//GMM_Core(lab,fenClass_c,point_num,k);
	//MergeClass(lab,fenClass_c,point_num,k);
	////MergeClassByColor(lab,fenClass_c,k);
	//DrawBorder(des,fenClass_c);

	des  = res;
	int image_bpp = des.GetBPP();
	int width = des.GetWidth();
	int height = des.GetHeight();
	vector<vector<float>> lab;
	vector<double>grad;
	vector<int>fenClass_c;

	Image2Lab(des,lab);
	GetImageGrad(lab,grad,width,height);
	SLIC_Core(lab,grad,fenClass_c,width,height,k,M);
	GMM_Core(lab,fenClass_c,width,height,k);
	MergeClassByColor(lab,fenClass_c,width,height,k);
	DrawBorder1(des,fenClass_c,width,height);
	return 0;
}
int GMM1(CImage &res,CImage &des,int k,double M)
{
	des  = res;
	//获取宽和高
	vector<vector<vector<float>>> lab;
	vector<vector<double>> grad;
	vector<vector<int>>fenClass_c;
	vector<int> point_num;
	Image2Lab(des,lab);
	GetImageGrad(lab,grad);
	SLIC_Core(lab,grad,fenClass_c,point_num,k,M);
	GMM_Core1(lab,fenClass_c,point_num,k);
	MergeClassByColor(lab,fenClass_c,k);
	DrawBorder(des,fenClass_c);
	return 0;
}