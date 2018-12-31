#pragma once
#include "stdafx.h"
using namespace std;
struct TagEx
{
	double r,g,b;
	double x,y;
	int num;
	TagEx(double r,double g,double b,double x,double y,int num)
	{
		this->r = r;
		this->g = g;
		this->b = b;
		this->x = x;
		this->y = y;
		this->num = num;
	}
	TagEx(const TagEx &tag)
	{
		this->r = tag.r;
		this->g = tag.g;
		this->b = tag.b;
		this->x = tag.x;
		this->y = tag.y;
		this->num = tag.num;
	}
	TagEx()
	{
		this->r = 0;
		this->g = 0;
		this->b = 0;
		this->x = 0;
		this->y = 0;
		this->num = 0;
	}
	bool operator==(const TagEx &obj)const
	{
		if(obj.r == r && obj.g == g && obj.b == b && obj.x == x && obj.y == y)
			return true;
		return false;
	}
	bool operator!=(const TagEx &obj)const
	{
		if(obj.r == r && obj.g == g && obj.b == b && obj.x == x && obj.y == y)
			return false;
		return true;
	}
	TagEx & operator +=(const TagEx &obj)
	{
		r += obj.r;
		g += obj.g;
		b += obj.b;
		x += obj.x;
		y += obj.y;
		num += obj.num;
		return *this;
	}
};


int Find(int *arr,int k);
int Find(vector<int> &arr,int k);
void Image2Lab(const CImage &des, vector<vector<float>> &lab);
void Image2Lab(const CImage &des, vector<vector<vector<float>>> &lab);
void GetImageGrad(const vector<vector<vector<float>>> &lab,vector<vector<double>> &grad);
void GetImageGrad(const vector<vector<float>> &lab,vector<double> &grad,int width,int height);
void SLIC_Core(	const vector<vector<vector<float>>> &lab,
				const vector<vector<double>> &grad,
				vector<vector<int>> &fenClass_c,
				vector<int> & point_num,
				int &k,
				double M);
void SLIC_Core(	const vector<vector<float>> &lab,
				const vector<double> &grad,
				vector<int> &fenClass_c,
				int width,
				int height,
				int &k,
				double M);
void MergeClass(const vector<vector<vector<float>>> &lab,vector<vector<int>> &fenClass_c,vector<int> & point_num,int k);
void MergeClassByColor(const vector<vector<vector<float>>> &lab,vector<vector<int>> &fenClass_c,int k);
void MergeClassByColor(const vector<vector<float>> &lab,vector<int> &fenClass_c,int width,int height,int k);
void MergeClassByNum(const vector<vector<vector<float>>> &lab,vector<vector<int>> &fenClass_c,int k);
void MergeClassByAngle(const vector<vector<vector<float>>> &lab,vector<vector<int>> &fenClass_c,int k);
void DrawBorder(CImage &des,vector<vector<int>> &fenClass_c);
void DrawBorder1(CImage &des,vector<vector<int>> &fenClass_c);
void DrawBorder1(CImage &des,vector<int> &fenClass_c,int width,int height);
void DrawBorderWithColor(CImage &des,vector<int> &fenClass_c,int k,int width,int height);
void DrawBorderOnlyLine(CImage &des,vector<int> &fenClass_c,int width,int height);
void GMM_Core(	const vector<vector<vector<float>>> &lab,
				vector<vector<int>> &fenClass_c,
				vector<int> & point_num,
				int &k);
void GMM_Core(	const vector<vector<float>> &lab,
				vector<int> &fenClass_c,
				int width,
				int height,
				int &k);
void GMM_Core1(	const vector<vector<vector<float>>> &lab,
				vector<vector<int>> &fenClass_c,
				vector<int> & point_num,
				int &k);
void LogDebug(const vector<vector<int>> & fenClass_c,CString path);
void LogDebug(const vector<int> & fenClass_c,int width,int height,CString path);

void KMeans_Core(	const vector<vector<vector<float>>> &lab,
					const vector<vector<double>> &grad,
					vector<vector<int>> &fenClass_c,
					int &k,
					double M);
double GetAngle(double ax,double ay,double bx,double by);
void ICM_Core(const vector<vector<float>> &lab,
				vector<int> &fenClass_c,
				int width,
				int height,
				int &k);
void Avg_n2(vector<vector<float>> & lab,int width,int height,int n);