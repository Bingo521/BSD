#pragma once
#include "stdafx.h"
using namespace std;

#define getNext(i,m,n)  (((i)%(n))*(m) + (i)/(n))
#define getPre(i,m,n)   (((i)%(m))*(n) + (i)/(m))

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

struct L_Tag
{
	TagEx taga;
	Tag tagb;
};
struct DPoint{
	double x;
	double y;
	int num;
	DPoint()
	{
		x = 0;
		y = 0;
		num = 0;
	}
	DPoint(double x,double y,int num)
	{
		this->x = x;
		this->y = y;
		this->num = num;
	}
};

/*
CMatrix为计算出逆矩阵的类，初始化传入一个参数n为n*n矩阵，用inverse
*/
class CMatrix
{
public:
    int n;
    vector<double> m_matrix;
    vector<double> bb;
    vector<double> adjoint;
    CMatrix(int n)
    {
        this->n = n;
		m_matrix = vector<double>(n * n);
		bb = vector<double>((n -1) * (n - 1));
		adjoint = vector<double>(n * n);
    }
    double det(int n,double *aa)
    {
        if (n == 1)
            return aa[0];
        int mov = 0;//判断行是否移动
        double sum = 0.0;//sum为行列式的值
        for (int arow = 0; arow<n; arow++) // a的行数把矩阵a(nn)赋值到b(n-1)
        {
            for (int brow = 0; brow<n - 1; brow++)//把aa阵第一列各元素的代数余子式存到bb
            {
                mov = arow > brow ? 0 : 1; //bb中小于arow的行，同行赋值，等于的错过，大于的加一
                for (int j = 0; j<n - 1; j++)  //从aa的第二列赋值到第n列
                {
                    bb[brow*(n - 1) + j] = aa[(brow + mov)*n + j + 1];
                }
            }
            int flag = (arow % 2 == 0 ? 1: -1);//因为列数为0，所以行数是偶数时候，代数余子式为1.
            sum += flag* aa[arow*n] * det(n - 1, bb);//aa第一列各元素与其代数余子式积的和即为行列式
        }
        return sum;
    }
    double inverse(double *aa)
    {
        double det_aa = det(n,aa);
        if (det_aa == 0)
        {
            return 0;
        }
        int pi, pj, q;
        for (int ai = 0; ai<n; ai++) // a的行数把矩阵a(nn)赋值到b(n-1)
        {
            for (int aj = 0; aj<n; aj++)
            {
                for (int bi = 0; bi<n - 1; bi++)//把元素aa[ai][0]代数余子式存到bb[][]
                {
                    for (int bj = 0; bj<n - 1; bj++)//把元素aa[ai][0]代数余子式存到bb[][]
                    {
                        if (ai>bi)    //ai行的代数余子式是：小于ai的行，aa与bb阵，同行赋值
                            pi = 0;
                        else
                            pi = 1;     //大于等于ai的行，取aa阵的ai+1行赋值给阵bb的bi行
                        if (aj>bj)    //ai行的代数余子式是：小于ai的行，aa与bb阵，同行赋值
                            pj = 0;
                        else
                            pj = 1;     //大于等于ai的行，取aa阵的ai+1行赋值给阵bb的bi行

                        bb[bi*(n - 1) + bj] = aa[(bi + pi)*n + bj + pj];
                    }
                }
                if ((ai + aj) % 2 == 0)  q = 1;//因为列数为0，所以行数是偶数时候，代数余子式为-1.
                else  q = (-1);
                adjoint[ai*n + aj] = q*det(n - 1, bb);
            }
        }
        for(int i = 0; i < n; i++)//adjoint 转置
        {
            for(int j = 0; j < i; j++)
            {
                double tem = adjoint[i*n + j];
                adjoint[i*n + j] =  adjoint[j*n + i];
                adjoint[j*n + i] =  tem;
            }
        }
        for (int i = 0; i < n; i++) //打印逆矩阵
        {
            for (int j = 0; j < n; j++)
            {
                m_matrix[i * n + j] = adjoint[i*n + j] / det_aa;
            }
        }
        return det_aa;
    }
	double det(int n,const vector<double> &aa)
    {
        if (n == 1)
            return aa[0];
        int mov = 0;//判断行是否移动
        double sum = 0.0;//sum为行列式的值
        for (int arow = 0; arow<n; arow++) // a的行数把矩阵a(nn)赋值到b(n-1)
        {
            for (int brow = 0; brow<n - 1; brow++)//把aa阵第一列各元素的代数余子式存到bb
            {
                mov = arow > brow ? 0 : 1; //bb中小于arow的行，同行赋值，等于的错过，大于的加一
                for (int j = 0; j<n - 1; j++)  //从aa的第二列赋值到第n列
                {
                    bb[brow*(n - 1) + j] = aa[(brow + mov)*n + j + 1];
                }
            }
            int flag = (arow % 2 == 0 ? 1: -1);//因为列数为0，所以行数是偶数时候，代数余子式为1.
            sum += flag* aa[arow*n] * det(n - 1, bb);//aa第一列各元素与其代数余子式积的和即为行列式
        }
        return sum;
    }
    double inverse(const vector<double> &aa)
    {
        double det_aa = det(n,aa);
        if (det_aa == 0)
        {
            return 0;
        }
        int pi, pj, q;
        for (int ai = 0; ai<n; ai++) // a的行数把矩阵a(nn)赋值到b(n-1)
        {
            for (int aj = 0; aj<n; aj++)
            {
                for (int bi = 0; bi<n - 1; bi++)//把元素aa[ai][0]代数余子式存到bb[][]
                {
                    for (int bj = 0; bj<n - 1; bj++)//把元素aa[ai][0]代数余子式存到bb[][]
                    {
                        if (ai>bi)    //ai行的代数余子式是：小于ai的行，aa与bb阵，同行赋值
                            pi = 0;
                        else
                            pi = 1;     //大于等于ai的行，取aa阵的ai+1行赋值给阵bb的bi行
                        if (aj>bj)    //ai行的代数余子式是：小于ai的行，aa与bb阵，同行赋值
                            pj = 0;
                        else
                            pj = 1;     //大于等于ai的行，取aa阵的ai+1行赋值给阵bb的bi行

                        bb[bi*(n - 1) + bj] = aa[(bi + pi)*n + bj + pj];
                    }
                }
                if ((ai + aj) % 2 == 0)  q = 1;//因为列数为0，所以行数是偶数时候，代数余子式为-1.
                else  q = (-1);
                adjoint[ai*n + aj] = q*det(n - 1, bb);
            }
        }
        for(int i = 0; i < n; i++)//adjoint 转置
        {
            for(int j = 0; j < i; j++)
            {
                double tem = adjoint[i*n + j];
                adjoint[i*n + j] =  adjoint[j*n + i];
                adjoint[j*n + i] =  tem;
            }
        }
        for (int i = 0; i < n; i++) //打印逆矩阵
        {
            for (int j = 0; j < n; j++)
            {
                m_matrix[i * n + j] = adjoint[i*n + j] / det_aa;
            }
        }
        return det_aa;
    }

    void Muti_Matrix(vector<double> &a,vector<double> &b,int m,int n,int k,vector<double> &c)
    {
        for(int i = 0; i < m ; i++)
        {
            for(int j = 0 ; j < k ; j ++)
            {
                adjoint[j] = 0;
                for(int w = 0 ; w < n ; w ++)
                {
                    adjoint[j]+= a[i * n + w] * b[j + w * k];
                }
            }
            for(int j = 0 ; j < k ; j ++)
            {
                c[i*k+j] = adjoint[j];
            }
        }
    }

	void Add_Matrix(vector<double> &a,vector<double> &b)
	{
		int n = a.size();
		for(int i = 0; i < n ; i ++)
		{
			a[i] += b[i];
		}
	}

	void Sub_Matrix(vector<double> &c,const vector<double> &a,const vector<double> &b)
	{
		int n = a.size();
		for(int i = 0; i < n ; i ++)
		{
			c[i] = a[i] - b[i];
		}
	}
};

class CMatrixEx
{
	public:
		const double lim0 ; 
		vector<double> tmp_n,tmp_n2,A_mirror,inv_A_each,B,b,L,U;
		vector<int> P;
		int N;
		int N2;
	public:
		CMatrixEx(int n):tmp_n(n),N(n),tmp_n2(n*n),A_mirror(n*n),inv_A_each(n),B(n*n),b(n),L(n*n),U(n*n),P(n),lim0(1e-10),N2(n*n)
		{
        
		}
		void mul(vector<double> &A,vector<double> &B,int m,int n,int k,vector<double> &C)
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
			//若绝对值小于10^-10,则置为0（这是我自己定的）
			for(int i=0;i<m*k;i++)
			{
				if(abs(C[i])<1e-10)
				{
					C[i]=0;
				}
			}
		}
		//LUP分解
	bool LUP_Descomposition(vector<double>&A,vector<double>&L,vector<double>&U,vector<int>&P)
	{
		int row=0;
		for(int i=0;i<N;i++)
		{
			P[i]=i;
		}
		for(int i=0;i<N-1;i++)
		{
			double p=0.0;
			for(int j=i;j<N;j++)
			{
				if(abs(A[j*N+i])>p)
				{
					p=abs(A[j*N+i]);
					row=j;
				}
			}
			if(0==p)
			{
				return false;
			}

			//交换P[i]和P[row]
			int tmp=P[i];
			P[i]=P[row];
			P[row]=tmp;

			double tmp2=0.0;
			for(int j=0;j<N;j++)
			{
				//交换A[i][j]和 A[row][j]
				tmp2=A[i*N+j];
				A[i*N+j]=A[row*N+j];
				A[row*N+j]=tmp2;
			}

			//以下同LU分解
			double u=A[i*N+i],l=0.0;
			for(int j=i+1;j<N;j++)
			{
				l=A[j*N+i]/u;
				A[j*N+i]=l;
				for(int k=i+1;k<N;k++)
				{
					A[j*N+k]=A[j*N+k]-A[i*N+k]*l;
				}
			}

		}

		//构造L和U
		for(int i=0;i<N;i++)
		{
			for(int j=0;j<=i;j++)
			{
				if(i!=j)
				{
					L[i*N+j]=A[i*N+j];
				}
				else
				{
					L[i*N+j]=1;
				}
			}
			for(int k=i;k<N;k++)
			{
				U[i*N+k]=A[i*N+k];
			}
		}
		return true;
	}

	//LUP求解方程
	void LUP_Solve(vector<double> &L,vector<double> &U,vector<int> &P,vector<double> &b,vector<double> &x)
	{
		//正向替换
		for(int i = 0;i < N;i++)
		{
			tmp_n[i] = b[P[i]];
			for(int j = 0;j < i;j++)
			{
				tmp_n[i] = tmp_n[i] - L[i*N+j]*tmp_n[j];
			}
		}
		//反向替换
		for(int i = N-1;i >= 0; i--)
		{
			x[i]=tmp_n[i];
			for(int j = N-1;j > i;j--)
			{
				x[i] = x[i] - U[i*N+j]*x[j];
			}
			x[i] /= U[i*N+i];
		}
	}
	/* 处理以下标i为起点的环 */
	void movedata(vector<double>&mtx, int i, int m, int n)
	{
	  double temp = mtx[i]; // 暂存
	  int cur = i;    // 当前下标
	  int pre = getPre(cur, m, n);
	  while(pre != i)
	  {
		mtx[cur] = mtx[pre];
		cur = pre;
		pre = getPre(cur, m, n);
	  }
	  mtx[cur] = temp;
	}
	/* 转置，即循环处理所有环 */
	void transpose(vector<double>&mtx, int m, int n)
	{
	  for(int i=0; i<m*n; ++i)
	  {
		int next = getNext(i, m, n);
		while(next > i) // 若存在后继小于i说明重复,就不进行下去了（只有不重复时进入while循环）
		  next = getNext(next, m, n);
		if(next == i)  // 处理当前环
		  movedata(mtx, i, m, n);
	  }
	}
	//LUP求逆(将每列b求出的各列x进行组装)
	bool LUP_solve_inverse(const vector<double> &A,vector<double> &inv_A)
	{
		//创建矩阵A的副本，注意不能直接用A计算，因为LUP分解算法已将其改变
		bool ans;
		for(int i=0;i<N;i++)
		{
			//构造单位阵的每一列
			for(int j=0;j<N;j++)
			{
				b[j]=0;
			}
			b[i]=1;

			//每次都需要重新将A复制一份
			for(int j=0;j<N2;j++)
			{
				A_mirror[j]=A[j];
			}

			ans = LUP_Descomposition(A_mirror,L,U,P);
			if(!ans)
				return ans;

			LUP_Solve (L,U,P,b,inv_A_each);
			memcpy(&inv_A[i*N],&inv_A_each[0],N*sizeof(double));//将各列拼接起来
		}
		transpose(inv_A,N,N);//由于现在根据每列b算出的x按行存储，因此需转置

		return true;
	}

};

int Find(int *arr,int k);
int Find(vector<int> &arr,int k);
bool GetSubImg(const CImage &res,CImage &des,CPoint pos,int width,int height);
CString GetPath(CString path,CString app);
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
void DrawBorderWidthAvgColor(CImage &des,vector<int> &fenClass_c,int width,int height);
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

void delete_empty_calss(vector<int> &fenClass_c,int width,int height,int &k);

void n_avg_2_k(vector<int> &fenClass_c,int width,int height,int &k);

void n_avg_2_k_(vector<int> &fenClass_c,int width,int height,int &k);

void LogDebug(vector<vector<double>>&,int width,int height,int k,CString name,int t);