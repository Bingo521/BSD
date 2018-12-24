#pragma once
#include "stdafx.h"
struct Tag
{
	double r,g,b;
	double x,y;
	Tag(double r,double g,double b,double x,double y)
	{
		this->r = r;
		this->g = g;
		this->b = b;
		this->x = x;
		this->y = y;
	}
	Tag(const Tag &tag)
	{
		this->r = tag.r;
		this->g = tag.g;
		this->b = tag.b;
		this->x = tag.x;
		this->y = tag.y;
	}
	Tag(){}
	bool operator==(const Tag &obj)const
	{
		if(obj.r == r && obj.g == g && obj.b == b && obj.x == x && obj.y == y)
			return true;
		return false;
	}
	bool operator!=(const Tag &obj)const
	{
		if(obj.r == r && obj.g == g && obj.b == b && obj.x == x && obj.y == y)
			return false;
		return true;
	}
	Tag & operator +=(const Tag &obj)
	{
		r += obj.r;
		g += obj.g;
		b += obj.b;
		x += obj.x;
		y += obj.y;
		return *this;
	}
};

int SLIC(CImage &res,CImage &des,int k,double M);