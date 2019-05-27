#pragma once

#include"stdafx.h"
using std::vector;
void KMeans(CImage &res,CImage &des,int k,double rate,int step,double sub);
void KMeans1(CImage &res,CImage &des,int k,double rate);
int KMeans(vector<vector<float>> &lab,vector<int> &fenClass_c,int width,int height,int &k,double M);