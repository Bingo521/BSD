#pragma once
#include"stdafx.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <iostream>
#include <vector>

class MatrixQuick{
	int n;
	int *is;
	int *js;
	double *bb;
	double *nn;
	int n2;
	const double LIM0;
public:
	MatrixQuick(int n):n(n),is(NULL),js(NULL),bb(NULL),n2(n*n),nn(NULL),LIM0(1e-8)
	{
		is = new int[n*n];
		js = new int[n*n];
		bb = new double[(n-1)*(n-1)];
		nn = new double[n*n];
	}
	~MatrixQuick(){
		delete []is;
		delete []js;
		delete []nn;
		delete bb;
	}
	double Inverse(double*  res,double *  des){
		int i,j,k,l,u,v;
		double d,p;
		memcpy_s(&des[0],sizeof(double)*n2,&res[0],sizeof(double)*n2);
		for (k=0; k<=n-1; k++)
		{
			d=0.0;
			for (i=k; i<=n-1; i++)
			for (j=k; j<=n-1; j++)
			{
				l=i*n+j; p=fabs(des[l]);
				if (p>d) { d=p; is[k]=i; js[k]=j;}
			}
			if (d+1.0==1.0)
			{
				return(0);
			}
			if (is[k]!=k)
				for (j=0; j<=n-1; j++)
				{
					u=k*n+j; v=is[k]*n+j;
					p=des[u]; des[u]=des[v]; des[v]=p;
				}
			if (js[k]!=k)
				for (i=0; i<=n-1; i++)
				{
					u=i*n+k; v=i*n+js[k];
					p=des[u]; des[u]=des[v]; des[v]=p;
				}
			l=k*n+k;
			des[l]=1.0/des[l];
			for (j=0; j<=n-1; j++)
				if (j!=k)
				{ u=k*n+j; des[u]=des[u]*des[l];}
			for (i=0; i<=n-1; i++)
				if (i!=k)
					for (j=0; j<=n-1; j++)
			if (j!=k)
			{
				u=i*n+j;
				des[u] -= des[i*n+k]*des[k*n+j];
			}
			for (i=0; i<=n-1; i++)
				if (i!=k)
				{
					u=i*n+k;
					des[u] = -des[u]*des[l];
				}
		}
		for (k=n-1; k>=0; k--)
		{
			if (js[k]!=k)
			for (j=0; j<=n-1; j++)
			{
				u=k*n+j; v=js[k]*n+j;
			p=des[u]; des[u]=des[v]; des[v]=p;
			}
			if (is[k]!=k)
			for (i=0; i<=n-1; i++)
			{ 
				u=i*n+k; v=i*n+is[k];
				p=des[u]; des[u]=des[v]; des[v]=p;
			}
		}
		return(1);
	}
	void Mul(double * A,double * B,int m,int n,int k,double * C)
	{
		for(int i=0;i<m;i++)
		{
			for(int j=0;j<k;j++)
			{
				C[i * k + j] = 0;
				for(int w=0;w<n;w++)
				{
					C[i * k + j] += A[i * n + w]*B[w * k + j];
				}
			}
		}
	}
    double Det(double * matrix)
    {

        int pos;
        for(int i = 0;i < n; i++)
        {
            for(int j = 0; j < n; j++)
            {
				pos = i*n+j;
                nn[pos] = matrix[pos];
            }
        }
        for (int i = 0; i < n; i++)
        { 
			pos = i*n+i;
            if (fabs(nn[pos]) < LIM0)
            {
                int j = -1;
                for (j = i + 1; j < n; j++)
                {
                    if (fabs(nn[j*n+i]) > LIM0) break;
                }
                if (j == n) return 0;
                for (int r = i; r < n; r++)
                {
                    nn[i*n+r] += nn[j*n+r];
                }
            }
            for (int j = i + 1; j < n; j++)
            {
                double e = -1 * (nn[j*n+i] / nn[pos]);
                for (int r = i; r < n; r++)
                {
                    nn[j*n+r] += e * nn[i*n+r];
                }
            }
        }
        double det = 1.0;
        for (int i = 0; i < n; i++) det *= nn[i*n+i];
        return det;
    }
	void Add(double *a,double *b,double *c)
	{
		for(int i=0;i<n2;i++)
		{
			c[i]=a[i]+b[i];
		}
	}
	void Sub(double *a,double *b,double *c)
	{
		for(int i=0;i<n2;i++)
		{
			c[i]=a[i]-b[i];
		}
	}
};