#include "stdafx.h"
using namespace std;

int SLIC(CImage &res,CImage &des,int k,double M)
{
	des  = res;
	int image_bpp = des.GetBPP();
	int width = des.GetWidth();
	int height = des.GetHeight();
	vector<vector<float>> lab;
	vector<double>grad;
	vector<int>fenClass_c;
	/*time_t st,et;
	CString str;
	st = clock();
	Image2Lab(des,lab);
	et = clock();
	str.Format("Image2Lab time:%.2lf",difftime(et,st));
	AfxMessageBox(str);
	st = clock();
	GetImageGrad(lab,grad,width,height);
	et = clock();
	str.Format("GetImageGrad time:%.2lf",difftime(et,st));
	AfxMessageBox(str);
	st = clock();
	SLIC_Core(lab,grad,fenClass_c,width,height,k,M);
	et = clock();
	str.Format("SLIC_Core time:%.2lf",difftime(et,st));
	AfxMessageBox(str);
	st = clock();
	MergeClassByColor(lab,fenClass_c,width,height,k);
	et = clock();
	str.Format("MergeClassByColor time:%.2lf",difftime(et,st));
	AfxMessageBox(str);
	st = clock();
	DrawBorder1(des,fenClass_c,width,height);
	et = clock();
	str.Format("DrawBorder1 time:%.2lf",difftime(et,st));
	AfxMessageBox(str);*/

	Image2Lab(des,lab);
	Avg_n2(lab,width,height,3);
	GetImageGrad(lab,grad,width,height);
	SLIC_Core(lab,grad,fenClass_c,width,height,k,M);
	MergeClassByColor(lab,fenClass_c,width,height,k);
	DrawBorder1(des,fenClass_c,width,height);
	//DrawBorderWithColor(des,fenClass_c,k,width,height);
	//DrawBorderOnlyLine(des,fenClass_c,width,height);
	return 0;
}