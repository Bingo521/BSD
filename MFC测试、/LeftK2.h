#pragma once
#include "stdafx.h"
struct Value_Pos
{
	double sub;
	int a;
	int b;
	bool operator < (const Value_Pos & obj)const
	{
		return sub < obj.sub;
	}
};
void LeftK2(CImage &res,CImage &des,int k,CString path);