#pragma once
#include "stdafx.h"
int GMM(CImage &res,CImage &des,int k,double M);
int GMM1(CImage &res,CImage &des,int k,double M);
int GMM2(vector<vector<float>> &lab,vector<int> &fenClass_c,int width,int height,int &k,double M);