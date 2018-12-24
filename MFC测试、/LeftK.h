#pragma once
class _Rgb
{
public:
	double r;
	double g;
	double b;
	_Rgb(){}
	_Rgb(double r,double g,double b)
	{
		this->r = r;
		this->g = g;
		this->b = b;
	}
};
class Point
{
public:
	int x,y;
	Point(){};
	Point(int x,int y)
	{
		if(x < y)
		{
			this->x = x;
			this->y = y;
		}
		else 
		{
			this->x = y;
			this->y = x;
		}
	};
	bool operator <(const Point & obj)const
	{
		if(obj.x == x)
		{
			return y < obj.y;
		}
		return x < obj.x;
	}
};
class Select_Key
{
public:
	int pointNum;
	double val;
	Select_Key(int pointNum,int val)
	{
		this->pointNum = pointNum;
		this->val =val;
	}
	bool operator <(const  Select_Key& obj)const
	{
		if(obj.pointNum == pointNum)
		{
			return val < obj.val;
		}
		return pointNum < obj.pointNum;
	}
};
void LeftK(CImage &res,CImage &des,int k,CString path);