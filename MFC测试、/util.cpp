#include "stdafx.h"
using namespace std;
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
};
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
void Muti_Matrix(vector<double> &a,vector<double> &b,int m,int n,int k,vector<double> &c)
{
	double * tmp = new double[k];
	for(int i = 0; i < m ; i++)
	{
		for(int j = 0 ; j < k ; j ++)
		{
			tmp[j] = 0;
			for(int w = 0 ; w < n ; w ++)
			{
				tmp[j]+= a[i * n + w] * b[w * n + j];
			}
		}
		for(int j = 0 ; j < k ; j ++)
		{
			c[i*k+j] = tmp[j];
		}
	}
	delete []tmp;
}
double GetAngle(double ax,double ay,double bx,double by)
{
    double tt,kk,sum;
    tt=ax*ax+ay*by;
    kk=bx*bx+by*by;
    kk=sqrt(tt+kk);
    tt=ax*bx+ay*by;
    sum=tt/kk;
    sum=acos(sum);
    tt=sum/Pi;
    return tt;
}
int Find(int *arr,int k)
{
	if(arr[k] == k)
		return k;
	list<int> l;
	while(arr[k]!=k)
	{
		l.push_front(k);
		k = arr[k];
	}
	for(auto ite = l.begin();ite!=l.end();ite++)
	{
		arr[*ite] = k;
	}
	return k;
}

int Find(vector<int> &arr,int k)
{
	if(arr[k] == k)
		return k;
	list<int> l;
	while(arr[k]!=k)
	{
		l.push_front(k);
		k = arr[k];
	}
	for(auto ite = l.begin();ite!=l.end();ite++)
	{
		arr[*ite] = k;
	}
	return k;
}

void Image2Lab(const CImage &des, vector<vector<vector<float>>> &lab)
{
	int width = des.GetWidth();
	int height = des.GetHeight();
	time_t st,et;
	CString str;
	st = clock();
	lab = vector<vector<vector<float>>>(width);
	for(int i = 0;i < width;i++)
	{
		lab[i] = vector<vector<float>>(height);
		for(int j = 0 ; j < height; j++)
		{
			lab[i][j] = vector<float>(3);
		}
	}
	et = clock();
	str.Format("--开辟%.2lf",difftime(et,st));
	AfxMessageBox(str);
	//转换颜色空间

	byte * blt = (byte*)des.GetBits();
	int plt = des.GetPitch();
	for(int i=0;i<width;i++)
	{
		for(int j=0;j<height;j++)
		{	
			RGB2LAB(*(blt + plt*j+ i*3),*(blt + plt*j+ i*3 + 1),*(blt + plt*j+ i*3 + 2),lab[i][j][0],lab[i][j][1],lab[i][j][2]);
		}
	}
}

void Image2Lab(const CImage &des, vector<vector<float>> &lab)
{
	int width = des.GetWidth();
	int height = des.GetHeight();
	int sz = width * height;
	/*time_t st,et;
	CString str;
	st = clock();*/
	lab = vector<vector<float>>(3);
	lab[0] = vector<float>(sz);
	lab[1] = vector<float>(sz);
	lab[2] = vector<float>(sz);
	/*et = clock();
	str.Format("--开辟%.2lf",difftime(et,st));
	AfxMessageBox(str);*/
	//转换颜色空间
	byte * blt = (byte*)des.GetBits();
	int plt = des.GetPitch();
	for(int i=0;i<width;i++)
	{
		for(int j=0;j<height;j++)
		{	
			int pos = i * height + j;
			RGB2LAB(*(blt + plt*j+ i*3),*(blt + plt*j+ i*3 + 1),*(blt + plt*j+ i*3 + 2),lab[0][pos],lab[1][pos],lab[2][pos]);
		}
	}
}
void GetImageGrad(const vector<vector<vector<float>>> &lab,vector<vector<double>> &grad)
{
	int width = lab.size();
	if(width == 0)
		return ;
	int height = lab[0].size();
	if(height == 0)
		return ;
	grad = vector<vector<double>>(width);
	for(int i = 0 ; i < width ; i++)
	{
		grad[i] = vector<double>(height);
	}
	for(int i = 1;i < width - 1; i++)
	{
		for(int j = 1; j < height -1;j++)
		{
			double ra,ga,ba;
			double rb,gb,bb;
			ra = lab[i+1][j][0] - lab[i-1][j][0];
			ga = lab[i+1][j][1] - lab[i-1][j][1];
			ba = lab[i+1][j][2] - lab[i-1][j][2];
			rb = lab[i][j+1][0] - lab[i][j-1][0];
			gb = lab[i][j+1][1] - lab[i][j-1][1];
			bb = lab[i][j+1][2] - lab[i][j-1][2];
			grad[i][j] = sqrt(ra*ra + ga*ga + ba*ba) + sqrt(rb*rb + gb*gb + bb*bb);
		}
	}
	for(int i = 0 ; i < width ; i++)
	{
		grad[i][0] = INFF;
		grad[i][height-1] = INFF;
	}
	for(int i = 0 ; i < height ; i++)
	{
		grad[0][i] = INFF;
		grad[width - 1][i] = INFF;
	}
}
void GetImageGrad(const vector<vector<float>> &lab,vector<double> &grad,int width,int height)
{
	int sz = width * height;
	grad = vector<double>(sz);
	for(int i = 1;i < width - 1; i++)
	{
		for(int j = 1; j < height -1;j++)
		{
			double ra,ga,ba;
			double rb,gb,bb;
			ra = lab[0][(i+1)*height + j] - lab[0][(i-1)*height + j];
			ga = lab[1][(i+1)*height + j] - lab[1][(i-1)*height + j];
			ba = lab[2][(i+1)*height + j] - lab[2][(i-1)*height + j];
			rb = lab[0][i*height + j + 1] - lab[0][i*height + j - 1];
			gb = lab[1][i*height + j + 1] - lab[1][i*height + j - 1];
			bb = lab[2][i*height + j + 1] - lab[2][i*height + j - 1];
			grad[i* height + j] = sqrt(ra*ra + ga*ga + ba*ba) + sqrt(rb*rb + gb*gb + bb*bb);
		}
	}
	for(int i = 0 ; i < width ; i++)
	{
		grad[i*height] = INFF;
		grad[i*height+height-1] = INFF;
	}
	for(int i = 0 ; i < height ; i++)
	{
		grad[i] = INFF;
		grad[(width - 1)*height + i] = INFF;
	}
}
void SLIC_Core(	const vector<vector<vector<float>>> &lab,
				const vector<vector<double>> &grad,
				vector<vector<int>> &fenClass_c,
				vector<int> & point_num,
				int &k,
				double M)
{
	int width = lab.size();
	if(width == 0)
		return ;
	int height = lab[0].size();
	if(height == 0)
		return ;
	vector<vector<double>>fenClass_v(width);
	vector<Tag> k_tags;
	fenClass_c = vector<vector<int>>(width);
	for(int i = 0 ; i < width ; i++)
	{
		fenClass_c[i] = vector<int>(height);
		fenClass_v[i] = vector<double>(height);
	}
	int pixel_num = width * height;
	int S = sqrt(pixel_num / k) + 0.5;
	M *= M;
	
	for(int i = 0 ; i * S < width ; i++ )
	{
		for(int j = 0 ; j * S < height ; j++)
		{
			int si = i * S;
			int sj = j * S;
			k_tags.push_back(Tag(0,0,0,si,sj));
		}
	}
	k = k_tags.size();
	int Move[][2] = {0,1,0,-1,1,0,-1,0,1,1,1,-1,-1,1,-1,-1}; 
	for(int i = 0 ; i < k ; i ++)
	{
		while(true)
		{
			int pos_x = (int)k_tags[i].x;
			int pos_y = (int)k_tags[i].y;
			for(int j = 0 ; j < 8 ; j ++)
			{
				int nx = k_tags[i].x + Move[j][0];
				int ny = k_tags[i].y + Move[j][1];
				if(nx < 0 || nx >= width || ny < 0 || ny >= height)
					continue;
				if(grad[pos_x][pos_y] > grad[nx][ny])
				{
					pos_x = nx;
					pos_y = ny;
				}
			}
			if(pos_x == k_tags[i].x && pos_y == k_tags[i].y)
			{
				k_tags[i].r = lab[pos_x][pos_y][0];
				k_tags[i].g = lab[pos_x][pos_y][1];
				k_tags[i].b = lab[pos_x][pos_y][2];
				break;
			}
			k_tags[i].x = pos_x;
			k_tags[i].y = pos_y;

		}
	}
	//初始结束
	//选取初始值结束
	//开始EM
	double S2 = S*S;
	vector<Tag> pre_point;
	point_num = vector<int>(k);
	int MaxTime = 10;
	while(MaxTime--)
	{
		for(int i = 0 ;i < width ; i++)
		{
			for(int j = 0; j < height; j ++)
			{
				fenClass_c[i][j] = -1;
				fenClass_v[i][j] = INFF;
			}
		}
		for(int i = 0 ;i < k;i++)
		{
			int spos_x,spos_y,epos_x,epos_y;
			Tag &tag = k_tags[i];
			spos_x = (int)max(tag.x - S,0);
			spos_y = (int)max(tag.y - S,0);
			epos_x = (int)min(tag.x + S,width);
			epos_y = (int)min(tag.y + S,height);
			for(int ii = spos_x ; ii < epos_x ;ii ++)
			{
				for(int jj = spos_y ; jj < epos_y ; jj ++)
				{
					double subr = lab[ii][jj][0] - tag.r;
					double subg = lab[ii][jj][1] - tag.g;
					double subb = lab[ii][jj][2] - tag.b;
					double subx = ii - tag.x;
					double suby = jj - tag.y;
					double sub = subr*subr + subg*subg + subb*subb +(subx*subx+suby*suby)*M/S2;
					if(fenClass_v[ii][jj] > sub)
					{
						fenClass_c[ii][jj] = i;
						fenClass_v[ii][jj] = sub;
					}
				}
			}
		}
		//保存重心
		pre_point = k_tags;
		//重新计算重心
		for(int i = 0; i < k ; i++)
		{
			k_tags[i].r = 0;
			k_tags[i].g = 0;
			k_tags[i].b = 0;
			k_tags[i].x = 0;
			k_tags[i].y = 0;
			point_num[i] = 0;
		}
		for(int i = 0 ;i < width ; i++)
		{
			for(int j = 0; j < height; j ++)
			{
				if(fenClass_c[i][j] == -1)
					continue;
				int c = fenClass_c[i][j];
				Tag & tag = k_tags[c];
				tag.r += lab[i][j][0];
				tag.g += lab[i][j][1];
				tag.b += lab[i][j][2];
				tag.x += i;
				tag.y += j;
				point_num[c] ++;
			}
		}
		for(int i = 0 ; i < k; i++)
		{
			if(point_num[i] == 0)
				continue;
			Tag & tag = k_tags[i];
			tag.r /= point_num[i];
			tag.g /= point_num[i];
			tag.b /= point_num[i];
			tag.x /= point_num[i];
			tag.y /= point_num[i];
		}
		//判断是否结束
		bool is_ok = true;
		for(int i = 0;i < k ; i++)
		{
			if(pre_point[i] != k_tags[i])
			{
				is_ok = false;
				break;
			}
		}
		if(is_ok)
			break;
	}
}

void SLIC_Core(	const vector<vector<float>> &lab,
				const vector<double> &grad,
				vector<int> &fenClass_c,
				int width,
				int height,
				int &k,
				double M)
{
	int sz = width * height;
	vector<double>fenClass_v(sz);
	vector<Tag> k_tags;
	fenClass_c = vector<int>(sz);
	
	int pixel_num = sz;
	int S = (int)sqrt(pixel_num / k) + 1;
	M *= M;
	k_tags.reserve((width/S+1) * ( height / S+1));
	for(int i = 0 ; i * S < width ; i++ )
	{
		for(int j = 0 ; j * S < height ; j++)
		{
			int si = i * S;
			int sj = j * S;
			k_tags.push_back(Tag(0,0,0,si,sj));
		}
	}
	k = k_tags.size();
	int Move[][2] = {0,1,0,-1,1,0,-1,0,1,1,1,-1,-1,1,-1,-1}; 
	for(int i = 0 ; i < k ; i ++)
	{
		while(true)
		{
			int pos_x = (int)k_tags[i].x;
			int pos_y = (int)k_tags[i].y;
			int pos = pos_x * height + pos_y;
			for(int j = 0 ; j < 8 ; j ++)
			{
				int nx = k_tags[i].x + Move[j][0];
				int ny = k_tags[i].y + Move[j][1];
				if(nx < 0 || nx >= width || ny < 0 || ny >= height)
					continue;
				if(grad[pos] > grad[nx * height + ny])
				{
					pos_x = nx;
					pos_y = ny;
				}
			}
			if(pos_x == k_tags[i].x && pos_y == k_tags[i].y)
			{
				k_tags[i].r = lab[0][pos];
				k_tags[i].g = lab[1][pos];
				k_tags[i].b = lab[2][pos];
				break;
			}
			k_tags[i].x = pos_x;
			k_tags[i].y = pos_y;

		}
	}
	//初始结束
	//选取初始值结束
	//开始EM
	double S2 = S*S;
	vector<Tag> pre_point;
	vector<int>point_num = vector<int>(k);
	int MaxTime = S ;
	if(MaxTime > 100)
		MaxTime = 100;
	while(MaxTime--)
	{
		for(int i = 0 ;i < width ; i++)
		{
			for(int j = 0; j < height; j ++)
			{
				fenClass_c[i*height + j] = 0;
				fenClass_v[i*height + j] = INFF;
			}
		}
		for(int i = 0 ;i < k;i++)
		{
			int spos_x,spos_y,epos_x,epos_y;
			Tag &tag = k_tags[i];
			spos_x = (int)max(tag.x - S,0);
			spos_y = (int)max(tag.y - S,0);
			epos_x = (int)min(tag.x + S,width);
			epos_y = (int)min(tag.y + S,height);
			for(int ii = spos_x ; ii < epos_x ;ii ++)
			{
				for(int jj = spos_y ; jj < epos_y ; jj ++)
				{
					int pos = ii * height + jj;
					double subr = lab[0][pos] - tag.r;
					double subg = lab[1][pos] - tag.g;
					double subb = lab[2][pos] - tag.b;
					double subx = ii - tag.x;
					double suby = jj - tag.y;
					double sub = subr*subr + subg*subg + subb*subb +(subx*subx+suby*suby)*M/S2;
					if(fenClass_v[pos] > sub)
					{
						fenClass_c[pos] = i;
						fenClass_v[pos] = sub;
					}
				}
			}
		}
		//重新计算重心
		for(int i = 0; i < k ; i++)
		{
			k_tags[i].r = 0;
			k_tags[i].g = 0;
			k_tags[i].b = 0;
			k_tags[i].x = 0;
			k_tags[i].y = 0;
			point_num[i] = 0;
		}
		for(int i = 0 ;i < width ; i++)
		{
			for(int j = 0; j < height; j ++)
			{
				int pos = i * height + j;
				if(fenClass_c[pos] == -1)
					continue;
				int c = fenClass_c[pos];
				Tag & tag = k_tags[c];
				tag.r += lab[0][pos];
				tag.g += lab[1][pos];
				tag.b += lab[2][pos];
				tag.x += i;
				tag.y += j;
				point_num[c] ++;
			}
		}
		for(int i = 0 ; i < k; i++)
		{
			if(point_num[i] == 0)
				continue;
			Tag & tag = k_tags[i];
			tag.r /= point_num[i];
			tag.g /= point_num[i];
			tag.b /= point_num[i];
			tag.x /= point_num[i];
			tag.y /= point_num[i];
		}
	}
}

void MergeClass(const vector<vector<vector<float>>> &lab,vector<vector<int>> &fenClass_c,vector<int> & point_num,int k)
{
	int width = lab.size();
	int height = lab[0].size();
	int rad = 4*k;
	int pixel_num = width * height;
	int S =(int)sqrt( pixel_num / k ) + 1;
	int MinPixelNum = pixel_num / k / 8;
	int MinDisX = S / 2;
	int MinDisY = S / 2;
	queue<CPoint> tem;
	vector<CPoint> vecArr;
	vector<vector<bool>> p(width);
	int Move[][2] = {0,1,0,-1,1,0,-1,0};

	for(int i = 0 ; i < width ; i ++)
	{
		p[i] = vector<bool>(height);
	}
	for(int i = 0; i < width; i++)
	{
		for(int j = 0; j < height; j++)
		{
			p[i][j] = false;
		}
	}
	for(int i=0;i<width;i++)
	{
		for(int j = 0;j<height;j++)
		{
			if(p[i][j])
				continue;
			vecArr.clear();
			int tK = fenClass_c[i][j];
			int wK = tK;
			tem.push(CPoint(i,j));
			vecArr.push_back(CPoint(i,j));
			p[i][j] = 1;
			int minX = i;
			int minY = j;
			int maxX = i;
			int maxY = j;
			vector<Tag> cRgb(k);
			vector<int> cNum(k);
			for(int ii = 0; ii<k ;ii++)
			{
				/*cRgb[ii].r = 0;
				cRgb[ii].g = 0;
				cRgb[ii].b = 0;*/
				cNum[ii] = 0;
			}
			/*cRgb[tK].r += lab[i][j][0];
			cRgb[tK].g += lab[i][j][1];
			cRgb[tK].b += lab[i][j][2];*/
			cNum[tK] ++;
			while(!tem.empty())
			{
				CPoint b = tem.front();
				tem.pop();
				for(int k=0;k<4;k++)
				{
					int nx = b.x + Move[k][0];
					int ny = b.y + Move[k][1];
					if( nx<0 || ny< 0 || nx >=width || ny >= height)
						continue;
					int c = fenClass_c[nx][ny];
					if(c == tK)
					{
						if(p[nx][ny])
							continue;
						p[nx][ny] = 1;
						vecArr.push_back(CPoint(nx,ny));
						tem.push(CPoint(nx,ny));
						if(nx >maxX)
							maxX = nx;
						if(nx < minX)
							minX = nx;
						if(ny > maxY)
							maxY = ny;
						if(ny < minY)
							minY = ny;
						
					}
					//cRgb[c].r += lab[nx][ny][0];
					//cRgb[c].g += lab[nx][ny][1];
					//cRgb[c].b += lab[nx][ny][2];
					cNum[c] ++;
				}
			}
			if(maxX - minX < MinDisX &&  maxY - minY < MinDisY || vecArr.size() < MinPixelNum)
			{
				int preSub = 0;
				for(int ii = 0; ii < k ;ii++)
				{
					if(ii == tK || cNum[ii] == 0)
						continue;
					if(preSub < point_num[ii])
					{
						preSub = point_num[ii];
						wK = ii;
					}
				}
				
				int len = vecArr.size();
				point_num[wK] += len;
				point_num[tK] -= len;
				for(int ii = 0; ii < len; ii++)
				{
					fenClass_c[vecArr[ii].x][vecArr[ii].y] = wK;
					p[vecArr[ii].x][vecArr[ii].y] = 0;
				}
			}
		}
	}
}

void MergeClassByColor(const vector<vector<vector<float>>> &lab,vector<vector<int>> &fenClass_c,int k)
{
	int width = lab.size();
	int height = lab[0].size();
	int pixel_num = width * height;
	int S =(int)sqrt( pixel_num / k ) ;
	int MinPixelNum = pixel_num / k / 4;
	int MinDisX = S / 2;
	int MinDisY = S / 2;

	//分配空间、定义变量
	vector<vector<int>> fenClass_r(width);
	vector<vector<bool>> is_visit(width);
	queue<CPoint> point_c;
	vector<CPoint> point_r;
	vector<Tag> new_class;
	vector<int> new_class_num;
	//vector<vector<vector<double>>> lab_sum(width);
	for(int i = 0 ; i < width ; i++)
	{
		fenClass_r[i] = vector<int>(height);
		is_visit[i] = vector<bool>(height);
		/*lab_sum[i] = vector<vector<double>>(height);
		for(int j = 0 ; j < height ; j++)
		{
			lab_sum[i][j] = vector<double>(3);
		}*/
	}

	//计算lab和
	/*lab_sum[0][0][0] = lab[0][0][0];
	lab_sum[0][0][1] = lab[0][0][1];
	lab_sum[0][0][2] = lab[0][0][2];

	for(int i = 1 ;i < width ; i ++)
	{
		lab_sum[i][0][0] = lab[i][0][0] + lab_sum[i-1][0][0];
		lab_sum[i][0][1] = lab[i][0][1] + lab_sum[i-1][0][1];
		lab_sum[i][0][2] = lab[i][0][2] + lab_sum[i-1][0][2];
	}
	for(int j = 1 ; j < height ; j ++)
	{
		lab_sum[0][j][0] = lab[0][j][0] + lab_sum[0][j-1][0];
		lab_sum[0][j][1] = lab[0][j][1] + lab_sum[0][j-1][0];
		lab_sum[0][j][2] = lab[0][j][2] + lab_sum[0][j-1][0];
	}
	for(int i = 1 ; i < width ; i++)
	{
		for(int j = 1 ; j < height ; j++)
		{
			lab_sum[i][j][0] = lab_sum[i][j-1][0] + lab_sum[i-1][j][0] - lab_sum[i-1][j-1][0] + lab[i][j][0];
			lab_sum[i][j][1] = lab_sum[i][j-1][1] + lab_sum[i-1][j][1] - lab_sum[i-1][j-1][1] + lab[i][j][1];
			lab_sum[i][j][2] = lab_sum[i][j-1][2] + lab_sum[i-1][j][2] - lab_sum[i-1][j-1][2] + lab[i][j][2];
		}
	}*/
	//初始化
	for(int i = 0 ; i < width ; i ++)
	{
		for(int j = 0 ; j < height ; j ++)
		{
			is_visit[i][j] = false;
		}
	}

	//重新划分类
	int newC = 0;
	int Move[][2] = {0,1,0,-1,1,0,-1,0,1,1,1,-1,-1,1,-1,-1};
	for(int i = 0 ; i < width ; i ++)
	{
		for(int j = 0 ; j < height ; j ++)
		{
			if(is_visit[i][j])
				continue;
			is_visit[i][j] = true;
			point_c.push(CPoint(i,j));
			point_r.push_back(CPoint(i,j));
			int c = fenClass_c[i][j];
			CPoint tmp;
			while(!point_c.empty())
			{
				tmp = point_c.front();
				point_c.pop();

				for(int w = 0 ; w < 4 ; w ++)
				{
					int nx = tmp.x + Move[w][0];
					int ny = tmp.y + Move[w][1];
					if(nx < 0 || nx >= width || ny < 0 || ny >= height || is_visit[nx][ny])
						continue;
					if(fenClass_c[nx][ny] == c)
					{
						is_visit[nx][ny] = true;
						point_c.push(CPoint(nx,ny));
						point_r.push_back(CPoint(nx,ny));
					}
				}
			}
			int len = point_r.size();
			new_class.push_back(Tag(0,0,0,0,0));
			new_class_num.push_back(0);
			Tag &tag = new_class[newC];
			for(int w = 0 ; w < len; w++)
			{
				CPoint &pos = point_r[w];
				fenClass_r[pos.x][pos.y] = newC;
				tag.r += lab[pos.x][pos.y][0];
				tag.g += lab[pos.x][pos.y][1];
				tag.b += lab[pos.x][pos.y][2];
				tag.x += pos.x;
				tag.y += pos.y;
			}
			new_class_num[newC] = len;
			point_r.clear();
			newC++;
		}
	}
	//LogDebug(fenClass_r,"test.txt");
	//消除小类
	int tk = new_class.size();
	k = tk;
	map<int,int>link_c;
	vector<bool>is_done(tk,false);
	for(int i = 0 ; i < width ; i ++)
	{
		for(int j = 0 ; j < height ; j++)
		{
			int c = fenClass_r[i][j];
			if(is_done[c] || new_class_num[c] > MinPixelNum)
			{
				is_done[c] = true;
				continue;
			}
			is_visit[i][j] = false;
			point_c.push(CPoint(i,j));
			point_r.push_back(CPoint(i,j));
			is_done[c] = true;
			
			CPoint tmp;
			int maxx = i,maxy = j, minx = i,miny = j;
			while(!point_c.empty())
			{
				tmp = point_c.front();
				point_c.pop();

				for(int w = 0 ; w < 4 ; w ++)
				{
					int nx = tmp.x + Move[w][0];
					int ny = tmp.y + Move[w][1];
					if(nx < 0 || nx >= width || ny < 0 || ny >= height)
						continue;
					int nc = fenClass_r[nx][ny];
					if(nc == c)
					{
						if(!is_visit[nx][ny])
						{
							continue;
						}
						is_visit[nx][ny] = false;
						point_c.push(CPoint(nx,ny));
						point_r.push_back(CPoint(nx,ny));
						if(nx > maxx)
							maxx = nx;
						else if(nx < minx)
							minx = nx;
						if(ny > maxy)
							maxy = ny;
						else if(ny < miny)
							miny = ny;
					}
					else 
					{
						link_c[nc]++;	
					}
				}
			}

			if(maxx - minx < MinDisX &&  maxy - miny < MinDisY)
			{
				Tag &tag = new_class[c];
				int &c_num = new_class_num[c];
				Tag avg_tag(tag.r / c_num,tag.g / c_num,tag.b / c_num,tag.x / c_num,tag.y / c_num);
				double min_sub = INFF;
				int new_c = 0;
				/*int rwidth = maxx - minx +1;
				int rheight = maxy - miny +1;
				int p_num = rwidth * rheight;
				int sx,sy,ex,ey;
				sx = max(minx - rwidth /2 ,0);
				sy = max(miny - rheight /2 ,0);

				ex = min(maxx + rwidth/2,width-1);
				ey = min(maxy + rheight/2,height-1);

				double r_sum = lab_sum[ex][ey][0] - lab_sum[ex][sy][0] - lab_sum[sx][ey][0] + lab_sum[sx][sy][0];
				double g_sum = lab_sum[ex][ey][1] - lab_sum[ex][sy][1] - lab_sum[sx][ey][1] + lab_sum[sx][sy][1];
				double b_sum = lab_sum[ex][ey][2] - lab_sum[ex][sy][2] - lab_sum[sx][ey][2] + lab_sum[sx][sy][2];

				r_sum /= p_num;
				g_sum /= p_num;
				b_sum /= p_num;*/
				for(auto ite = link_c.begin();ite != link_c.end();ite++)
				{
					
					Tag &tag1 = new_class[ite->first];
					int &c_num1 = new_class_num[ite->first];
					Tag avg_tag1(tag1.r / c_num1,tag1.g / c_num1,tag1.b / c_num1,tag1.x / c_num1,tag1.y / c_num1);
					double sub =	(avg_tag.r - avg_tag1.r) * (avg_tag.r - avg_tag1.r) +
									(avg_tag.g - avg_tag1.g) * (avg_tag.g - avg_tag1.g) +
									(avg_tag.b - avg_tag1.b) * (avg_tag.b - avg_tag1.b) +
									(avg_tag.x - avg_tag1.x) * (avg_tag.x - avg_tag1.x) +
									(avg_tag.y - avg_tag1.y) * (avg_tag.y - avg_tag1.y) ;
					sub /= ite->second;
					//double sub =	(r_sum - avg_tag1.r) * (r_sum - avg_tag1.r) +
					//				(g_sum - avg_tag1.g) * (g_sum - avg_tag1.g) +
					//				(b_sum - avg_tag1.b) * (b_sum - avg_tag1.b) +
					//				(avg_tag.x - avg_tag1.x) * (avg_tag.x - avg_tag1.x) +
					//				(avg_tag.y - avg_tag1.y) * (avg_tag.y - avg_tag1.y) ;
					if(min_sub > sub)
					{
						min_sub = sub;
						new_c = ite->first;
					}
				}
				int len = point_r.size();
				for(int w = 0 ; w < len; w ++)
				{
					fenClass_r[point_r[w].x][point_r[w].y] = new_c;
					if(!is_done[new_c])
						is_visit[point_r[w].x][point_r[w].y] = true;
				}
				new_class[new_c] += tag;
				new_class_num[new_c] += c_num;	
			}
			point_r.clear();
			link_c.clear();
		}
	}
	for(int i = 0 ; i < width ; i ++)
	{
		for(int j = 0 ; j < height ; j++)
		{
			fenClass_c[i][j] = fenClass_r[i][j];
		}
	}
	//LogDebug(fenClass_r,"test1.txt");
}

void MergeClassByColor(const vector<vector<float>> &lab,vector<int> &fenClass_c,int width,int height,int k)
{
	int pixel_num = width * height;
	int sz = pixel_num;
	int S =(int)sqrt( pixel_num / k ) ;
	int MinPixelNum = pixel_num / k / 4 ;
	int MinDisX = S / 2;
	int MinDisY = S / 2;

	//分配空间、定义变量
	vector<int> fenClass_r(sz);
	vector<bool> is_visit(sz,false);
	queue<CPoint> point_c;
	vector<CPoint> point_r;
	vector<Tag> new_class;
	vector<int> new_class_num;
	//重新划分类
	int newC = 0;
	int Move[][2] = {0,1,0,-1,1,0,-1,0,1,1,1,-1,-1,1,-1,-1};
	for(int i = 0 ; i < width ; i ++)
	{
		for(int j = 0 ; j < height ; j ++)
		{
			int pos = i*height + j;
			if(is_visit[pos])
				continue;
			is_visit[pos] = true;
			point_c.push(CPoint(i,j));
			point_r.push_back(CPoint(i,j));
			int c = fenClass_c[pos];
			CPoint tmp;
			while(!point_c.empty())
			{
				tmp = point_c.front();
				point_c.pop();

				for(int w = 0 ; w < 4 ; w ++)
				{
					int nx = tmp.x + Move[w][0];
					int ny = tmp.y + Move[w][1];
					if(nx < 0 || nx >= width || ny < 0 || ny >= height)
						continue;
					int npos = nx * height + ny;
					if(is_visit[npos])
						continue;
					if(fenClass_c[npos] == c)
					{
						is_visit[npos] = true;
						point_c.push(CPoint(nx,ny));
						point_r.push_back(CPoint(nx,ny));
					}
				}
			}
			int len = point_r.size();
			new_class.push_back(Tag(0,0,0,0,0));
			new_class_num.push_back(0);
			Tag &tag = new_class[newC];
			for(int w = 0 ; w < len; w++)
			{
				CPoint &pos = point_r[w];
				int ip = pos.x * height + pos.y;
				fenClass_r[ip] = newC;
				tag.r += lab[0][ip];
				tag.g += lab[1][ip];
				tag.b += lab[2][ip];
				tag.x += pos.x;
				tag.y += pos.y;
			}
			new_class_num[newC] = len;
			point_r.clear();
			newC++;
		}
	}
	//消除小类
	int tk = new_class.size();
	k = tk;
	map<int,int>link_c;
	vector<bool>is_done(tk,false);
	for(int i = 0 ; i < width ; i ++)
	{
		for(int j = 0 ; j < height ; j++)
		{
			int pos = i * height + j;
			int c = fenClass_r[pos];
			if(is_done[c] || new_class_num[c] > MinPixelNum)
			{
				is_done[c] = true;
				continue;
			}
			is_visit[pos] = false;
			point_c.push(CPoint(i,j));
			point_r.push_back(CPoint(i,j));
			is_done[c] = true;
			
			CPoint tmp;
			int maxx = i,maxy = j, minx = i,miny = j;
			while(!point_c.empty())
			{
				tmp = point_c.front();
				point_c.pop();

				for(int w = 0 ; w < 4 ; w ++)
				{
					int nx = tmp.x + Move[w][0];
					int ny = tmp.y + Move[w][1];
					if(nx < 0 || nx >= width || ny < 0 || ny >= height)
						continue;
					int npos = nx * height + ny;
					int nc = fenClass_r[npos];
					if(nc == c)
					{
						if(!is_visit[npos])
						{
							continue;
						}
						is_visit[npos] = false;
						point_c.push(CPoint(nx,ny));
						point_r.push_back(CPoint(nx,ny));
						if(nx > maxx)
							maxx = nx;
						else if(nx < minx)
							minx = nx;
						if(ny > maxy)
							maxy = ny;
						else if(ny < miny)
							miny = ny;
					}
					else 
					{
						link_c[nc]++;	
					}
				}
			}

			//if(abs(maxx - minx - maxy + miny) < MinDisY)
			{
				Tag &tag = new_class[c];
				int &c_num = new_class_num[c];
				Tag avg_tag(tag.r / c_num,tag.g / c_num,tag.b / c_num,tag.x / c_num,tag.y / c_num);
				double min_sub = INFF;
				int new_c = 0;
				for(auto ite = link_c.begin();ite != link_c.end();ite++)
				{
					
					Tag &tag1 = new_class[ite->first];
					int &c_num1 = new_class_num[ite->first];
					Tag avg_tag1(tag1.r / c_num1,tag1.g / c_num1,tag1.b / c_num1,tag1.x / c_num1,tag1.y / c_num1);
					double sub =	(avg_tag.r - avg_tag1.r) * (avg_tag.r - avg_tag1.r) +
									(avg_tag.g - avg_tag1.g) * (avg_tag.g - avg_tag1.g) +
									(avg_tag.b - avg_tag1.b) * (avg_tag.b - avg_tag1.b) +
									(avg_tag.x - avg_tag1.x) * (avg_tag.x - avg_tag1.x) +
									(avg_tag.y - avg_tag1.y) * (avg_tag.y - avg_tag1.y) ;
					sub /= ite->second;
					if(min_sub > sub)
					{
						min_sub = sub;
						new_c	= ite->first;
					}
				}
				int len = point_r.size();
				for(int w = 0 ; w < len; w ++)
				{
					int tpos = point_r[w].x * height + point_r[w].y;
					fenClass_r[tpos] = new_c;
					if(!is_done[new_c])
						is_visit[tpos] = true;
				}
				new_class[new_c] += tag;
				new_class_num[new_c] += c_num;	
			}
			point_r.clear();
			link_c.clear();
		}
	}
	for(int i = 0 ; i < sz ; i++)
	{
		fenClass_c[i] = fenClass_r[i];
	}
}

void MergeClassByNum(const vector<vector<vector<float>>> &lab,vector<vector<int>> &fenClass_c,int k)
{
	int width = lab.size();
	int height = lab[0].size();
	int pixel_num = width * height;
	int S =(int)sqrt( pixel_num / k ) + 1;
	int MinPixelNum = pixel_num / k / 4;
	int MinDisX = S ;
	int MinDisY = S ;

	//分配空间、定义变量
	vector<vector<int>> fenClass_r(width);
	vector<vector<bool>> is_visit(width);
	queue<CPoint> point_c;
	vector<CPoint> point_r;
	vector<int> new_class_num;
	for(int i = 0 ; i < width ; i++)
	{
		fenClass_r[i] = vector<int>(height);
		is_visit[i] = vector<bool>(height);
	}
	//初始化
	for(int i = 0 ; i < width ; i ++)
	{
		for(int j = 0 ; j < height ; j ++)
		{
			is_visit[i][j] = false;
		}
	}

	//重新划分类
	int newC = 0;
	int Move[][2] = {0,1,0,-1,1,0,-1,0,1,1,1,-1,-1,1,-1,-1};
	for(int i = 0 ; i < width ; i ++)
	{
		for(int j = 0 ; j < height ; j ++)
		{
			if(is_visit[i][j])
				continue;
			is_visit[i][j] = true;
			point_c.push(CPoint(i,j));
			point_r.push_back(CPoint(i,j));
			int c = fenClass_c[i][j];
			CPoint tmp;
			while(!point_c.empty())
			{
				tmp = point_c.front();
				point_c.pop();

				for(int w = 0 ; w < 8 ; w ++)
				{
					int nx = tmp.x + Move[w][0];
					int ny = tmp.y + Move[w][1];
					if(nx < 0 || nx >= width || ny < 0 || ny >= height || is_visit[nx][ny])
						continue;
					if(fenClass_c[nx][ny] == c)
					{
						is_visit[nx][ny] = true;
						point_c.push(CPoint(nx,ny));
						point_r.push_back(CPoint(nx,ny));
					}
				}
			}
			int len = point_r.size();
			new_class_num.push_back(0);
			for(int w = 0 ; w < len; w++)
			{
				CPoint &pos = point_r[w];
				fenClass_r[pos.x][pos.y] = newC;
			}
			new_class_num[newC] = len;
			point_r.clear();
			newC++;
		}
	}

	//消除小类
	int tk = new_class_num.size();
	k = tk;
	set<int> link_c;
	vector<bool>is_done(tk);
	vector<int> link(tk);
	for(int i = 0 ; i < tk;i++)
	{
		is_done[i] = false;
		link[i] = i;
	}
	for(int i = 0 ; i < width ; i ++)
	{
		for(int j = 0 ; j < height ; j++)
		{
			int c = fenClass_r[i][j];
			is_done[c] = true;
			if(new_class_num[c] >= MinPixelNum){
				continue;
			}
			if(c == 50)
			{
				c = 50;
			}
			link_c.clear();
			is_visit[i][j] = false;
			point_c.push(CPoint(i,j));
			point_r.push_back(CPoint(i,j));
			CPoint tmp;
			int maxx = i,maxy = j, minx = i,miny = j;
			while( !point_c.empty() )
			{
				tmp = point_c.front();
				point_c.pop();

				for(int w = 0 ; w < 8 ; w ++)
				{
					int nx = tmp.x + Move[w][0];
					int ny = tmp.y + Move[w][1];
					if(nx < 0 || nx >= width || ny < 0 || ny >= height)
						continue;
					int nc = fenClass_r[nx][ny];
					if(  nc == c )
					{
						if(!is_visit[nx][ny])
						{
							continue;
						}
						is_visit[nx][ny] = false;
						point_c.push(CPoint(nx,ny));
						point_r.push_back(CPoint(nx,ny));
						if(nx > maxx)
							maxx = nx;
						else if(nx < minx)
							minx = nx;
						if(ny > maxy)
							maxy = ny;
						else if(ny < miny)
							miny = ny;
					}
					else 
					{
						link_c.insert(nc);
					}
				}
			}
			if(maxx - minx < MinDisX &&  maxy - miny < MinDisY || point_r.size() < MinPixelNum)
			{
				int &c_num = new_class_num[c];
				int min_sub = INFI;
				int new_c = 0;
				for(auto ite = link_c.begin();ite != link_c.end();ite ++)
				{
					int &c_num1 = new_class_num[*ite];
					if(min_sub > c_num1)
					{
						min_sub = c_num1;
						new_c = *ite;
					}
				}
				int len = point_r.size();
				for(int w = 0 ; w < len; w ++)
				{
					fenClass_r[point_r[w].x][point_r[w].y] = new_c;
					if(!is_done[new_c])
						is_visit[point_r[w].x][point_r[w].y] = true;
				}
				new_class_num[new_c] += c_num;	
			}
			point_r.clear();

		}
	}
	for(int i = 0 ; i < width ; i++)
	{
		for(int j = 0 ; j < height ; j++)
		{
			fenClass_c[i][j] = fenClass_r[i][j];
		}
	}
}

void MergeClassByAngle(const vector<vector<vector<float>>> &lab,vector<vector<int>> &fenClass_c,int k)
{
	int width = lab.size();
	int height = lab[0].size();
	int pixel_num = width * height;
	int S =(int)sqrt( pixel_num / k ) + 1;
	int MinPixelNum = pixel_num / k / 4;
	int MinDisX = S ;
	int MinDisY = S ;

	//分配空间、定义变量
	vector<vector<int>> fenClass_r(width);
	vector<vector<bool>> is_visit(width);
	queue<CPoint> point_c;
	vector<CPoint> point_r;
	for(int i = 0 ; i < width ; i++)
	{
		fenClass_r[i] = vector<int>(height);
		is_visit[i] = vector<bool>(height);
	}
	//初始化
	vector<DPoint> ZX(k);
	for(int i = 0 ; i < width ; i ++)
	{
		for(int j = 0 ; j < height ; j ++)
		{
			is_visit[i][j] = false;
		}
	}
	//求重心
	for(int i = 0 ; i < width ; i ++)
	{
		for(int j = 0 ; j < height ; j++)
		{
			int c = fenClass_c[i][j];
			ZX[c].x += i;
			ZX[c].y += j;
			ZX[c].num ++;
 		}
	}
	/*for(int i = 0 ; i < k; i ++)
	{
		if(ZX[i].num == 0)
			continue;
		ZX[i].x /= ZX[i].num;
		ZX[i].y /= ZX[i].num;
	}*/
	//重新划分类
	int newC = 0;
	int Move[][2] = {0,1,0,-1,1,0,-1,0,1,1,1,-1,-1,1,-1,-1};
	vector<DPoint> nc_ZX;
	for(int i = 0 ; i < width ; i ++)
	{
		for(int j = 0 ; j < height ; j ++)
		{
			if(is_visit[i][j])
				continue;
			is_visit[i][j] = true;
			point_c.push(CPoint(i,j));
			point_r.push_back(CPoint(i,j));
			int c = fenClass_c[i][j];
			CPoint tmp;
			while(!point_c.empty())
			{
				tmp = point_c.front();
				point_c.pop();

				for(int w = 0 ; w < 8 ; w ++)
				{
					int nx = tmp.x + Move[w][0];
					int ny = tmp.y + Move[w][1];
					if(nx < 0 || nx >= width || ny < 0 || ny >= height || is_visit[nx][ny])
						continue;
					if(fenClass_c[nx][ny] == c)
					{
						is_visit[nx][ny] = true;
						point_c.push(CPoint(nx,ny));
						point_r.push_back(CPoint(nx,ny));
					}
				}
			}
			int len = point_r.size();
			nc_ZX.push_back(DPoint());
			for(int w = 0 ; w < len; w++)
			{
				CPoint &pos = point_r[w];
				fenClass_r[pos.x][pos.y] = newC;
				nc_ZX[newC].x += pos.x;
				nc_ZX[newC].y += pos.y;
			}
			nc_ZX[newC].num = len;
			point_r.clear();
			newC++;
		}
	}

	//消除小类
	int tk = nc_ZX.size();
	k = tk;
	set<int> link_c;
	vector<bool>is_done(tk);
	vector<int> link(tk);
	for(int i = 0 ; i < tk;i++)
	{
		is_done[i] = false;
		link[i] = i;
	}
	for(int i = 0 ; i < width ; i ++)
	{
		for(int j = 0 ; j < height ; j++)
		{
			int c = fenClass_r[i][j];
			is_done[c] = true;
			if(nc_ZX[c].num >= MinPixelNum){
				continue;
			}
			link_c.clear();
			is_visit[i][j] = false;
			point_c.push(CPoint(i,j));
			point_r.push_back(CPoint(i,j));
			CPoint tmp;
			int maxx = i,maxy = j, minx = i,miny = j;
			while( !point_c.empty() )
			{
				tmp = point_c.front();
				point_c.pop();

				for(int w = 0 ; w < 8 ; w ++)
				{
					int nx = tmp.x + Move[w][0];
					int ny = tmp.y + Move[w][1];
					if(nx < 0 || nx >= width || ny < 0 || ny >= height)
						continue;
					int nc = fenClass_r[nx][ny];
					if(  nc == c )
					{
						if(!is_visit[nx][ny])
						{
							continue;
						}
						is_visit[nx][ny] = false;
						point_c.push(CPoint(nx,ny));
						point_r.push_back(CPoint(nx,ny));
						if(nx > maxx)
							maxx = nx;
						else if(nx < minx)
							minx = nx;
						if(ny > maxy)
							maxy = ny;
						else if(ny < miny)
							miny = ny;
					}
					else 
					{
						link_c.insert(nc);
					}
				}
			}
			if(maxx - minx < MinDisX &&  maxy - miny < MinDisY )
			{
				DPoint & n_zx = nc_ZX[c];
				CPoint &pos = point_r[0];
				int pc = fenClass_c[pos.x][pos.y];
				DPoint & p_zx = ZX[pc]; 
				double zxnx = n_zx.x / n_zx.num;
				double zxny = n_zx.y / n_zx.num;
				double zxpx = p_zx.x / p_zx.num;
				double zxpy = p_zx.y / p_zx.num;
				double ax = zxpx - zxnx;
				double ay = zxpy - zxny;
				double min_angle = 10;
				int newC = 0;
				for(auto ite = link_c.begin() ; ite != link_c.end() ; ite++)
				{
					DPoint &t_zx = nc_ZX[*ite];
					double zxtx = t_zx.x / t_zx.num;
					double zxty = t_zx.y / t_zx.num;
					double bx = zxpx - zxtx;
					double by = zxpy - zxty;
					double angle = GetAngle(ax,ay,bx,by);
					if(angle < min_angle)
					{
						min_angle = angle;
						newC = *ite;
					}
				}
				int len = point_r.size();
				for(int w = 0 ; w < len; w ++)
				{
					fenClass_r[point_r[w].x][point_r[w].y] = newC;
					if(!is_done[newC])
						is_visit[point_r[w].x][point_r[w].y] = true;
				}
				nc_ZX[newC].num += nc_ZX[c].num;
				nc_ZX[newC].x += nc_ZX[c].x;
				nc_ZX[newC].y += nc_ZX[c].y;
			}
			point_r.clear();

		}
	}
	for(int i = 0 ; i < width ; i++)
	{
		for(int j = 0 ; j < height ; j++)
		{
			fenClass_c[i][j] = fenClass_r[i][j];
		}
	}
}

void DrawBorder(CImage &des,vector<vector<int>> &fenClass_c)
{
	int width = fenClass_c.size();
	int height = fenClass_c[0].size();
	int Move[][2] = {0,1,0,-1,1,0,-1,0,1,1,1,-1,-1,1,-1,-1};
	for(int i=0;i<width;i++)
	{
		for(int j=0;j<height;j++)
		{
			bool isWhite = false;
			for(int w=0;w<8;w++)
			{
				int nx = i+Move[w][0];
				int ny = j+Move[w][1];
				if(nx<0 || ny<0 || nx>= width || ny>=height)
				{
					continue;
				}
				if(fenClass_c[i][j]!=fenClass_c[nx][ny])
				{
					isWhite = true;
					break;
				}
			}
			if(isWhite)
			{
				des.SetPixel(i,j,RGB(255,255,255));
			}
		}
	}
}

void DrawBorder1(CImage &des,vector<vector<int>> &fenClass_c)
{
	int width = fenClass_c.size();
	int height = fenClass_c[0].size();
	int sz = width * height;
	int Move[][2] = {0,1,0,-1,1,0,-1,0,1,1,1,-1,-1,1,-1,-1};
	vector<int> contourx(sz);
	vector<int> contoury(sz);
	vector<bool> is_taken(sz,false);
	int cou = 0;
	int main_taken = 0;
	byte * blt = (byte *)des.GetBits();
	int plt = des.GetPitch();
	for(int i=0;i<width;i++)
	{
		for(int j=0;j<height;j++)
		{
			bool isWhite = false;
			int nup(0);
			for(int w=0;w<8;w++)
			{
				int nx = i+Move[w][0];
				int ny = j+Move[w][1];
				if(nx<0 || ny<0 || nx>= width || ny>=height)
				{
					continue;
				}
				if(fenClass_c[i][j]!=fenClass_c[nx][ny])
				{
					nup ++;
				}
			}
			if(nup > 1)
			{
				contourx[cou] = i;
				contoury[cou] = j;
				is_taken[main_taken] = true;
				cou ++;
			}
			main_taken ++;
		}
	}
	for(int i = 0 ;i<cou ; i++)
	{
		int pos = contourx[i] * height + contoury[i] ;
		*(blt + plt*contoury[i]+ contourx[i]*3) = 0xff;
		*(blt + plt*contoury[i]+ contourx[i]*3 + 1) = 0xff;
		*(blt + plt*contoury[i]+ contourx[i]*3 + 2) = 0xff;
		for(int j = 0 ; j < 8 ; j ++)
		{
			int nx = contourx[i] + Move[j][0];
			int ny = contoury[i] + Move[j][1];
			
			if(nx<0 || ny<0 || nx>= width || ny>=height)
			{
				continue;
			}
			int npos = nx * height + ny;
			if(!is_taken[npos])
			{
				*(blt + plt*ny + nx*3) = 0;
				*(blt + plt*ny + nx*3 + 1) = 0;
				*(blt + plt*ny + nx*3 + 2) = 0;
			}
		}
	}
}

void DrawBorder1(CImage &des,vector<int> &fenClass_c,int width,int height)
{
	int sz = width * height;
	int Move[][2] = {0,1,0,-1,1,0,-1,0,1,1,1,-1,-1,1,-1,-1};
	vector<int> contourx(sz);
	vector<int> contoury(sz);
	vector<bool> is_taken(sz,false);
	int cou = 0;
	int main_taken = 0;
	byte * blt = (byte *)des.GetBits();
	int plt = des.GetPitch();
	for(int i=0;i<width;i++)
	{
		for(int j=0;j<height;j++)
		{
			bool isWhite = false;
			int nup(0);
			int pos = i * height + j;
			for(int w=0;w<8;w++)
			{
				int nx = i+Move[w][0];
				int ny = j+Move[w][1];
				if(nx<0 || ny<0 || nx>= width || ny>=height)
				{
					continue;
				}
				int npos = nx * height + ny;
				if(fenClass_c[pos]!=fenClass_c[npos])
				{
					nup ++;
				}
			}
			if(nup > 1)
			{
				contourx[cou] = i;
				contoury[cou] = j;
				is_taken[main_taken] = true;
				cou ++;
			}
			main_taken ++;
		}
	}
	for(int i = 0 ;i<cou ; i++)
	{
		int pos = contourx[i] * height + contoury[i] ;
		*(blt + plt*contoury[i]+ contourx[i]*3) = 0xff;
		*(blt + plt*contoury[i]+ contourx[i]*3 + 1) = 0xff;
		*(blt + plt*contoury[i]+ contourx[i]*3 + 2) = 0xff;
		for(int j = 0 ; j < 8 ; j ++)
		{
			int nx = contourx[i] + Move[j][0];
			int ny = contoury[i] + Move[j][1];
			
			if(nx<0 || ny<0 || nx>= width || ny>=height)
			{
				continue;
			}
			int npos = nx * height + ny;
			if(!is_taken[npos])
			{
				*(blt + plt*ny + nx*3) = 0;
				*(blt + plt*ny + nx*3 + 1) = 0;
				*(blt + plt*ny + nx*3 + 2) = 0;
			}
		}
	}
}

void DrawBorderWithColor(CImage &des,vector<int> &fenClass_c,int k,int width,int height)
{
	int sz = width * height;
	int Move[][2] = {0,1,0,-1,1,0,-1,0,1,1,1,-1,-1,1,-1,-1};
	int cou = 0;
	int main_taken = 0;
	byte * blt = (byte *)des.GetBits();
	int plt = des.GetPitch();
	vector<vector<byte>> color(3,vector<byte>(sz));
	srand(time(NULL));
	for(int i = 0 ; i < sz ; i ++)
	{
		color[0][i] = rand() % 256;
		color[1][i] = rand() % 256;
		color[2][i] = rand() % 256;
	}
	for(int i = 0 ;i<width ; i++)
	{
		for(int j = 0 ; j < height ; j++)
		{
			int pos = i * height + j;
			*(blt + plt*j+ i*3) = color[0][fenClass_c[pos]];
			*(blt + plt*j+ i*3 + 1) = color[1][fenClass_c[pos]];
			*(blt + plt*j+ i*3 + 2) = color[2][fenClass_c[pos]];
		}
	}
}
void DrawBorderOnlyLine(CImage &des,vector<int> &fenClass_c,int width,int height)
{
	int sz = width * height;
	int Move[][2] = {0,1,0,-1,1,0,-1,0,1,1,1,-1,-1,1,-1,-1};
	vector<int> contourx(sz);
	vector<int> contoury(sz);
	vector<bool> is_taken(sz,false);
	int cou = 0;
	int main_taken = 0;
	byte * blt = (byte *)des.GetBits();
	int plt = des.GetPitch();
	for(int i=0;i<width;i++)
	{
		for(int j=0;j<height;j++)
		{
			bool isWhite = false;
			int nup(0);
			int pos = i * height + j;
			for(int w=0;w<8;w++)
			{
				int nx = i+Move[w][0];
				int ny = j+Move[w][1];
				if(nx<0 || ny<0 || nx>= width || ny>=height)
				{
					continue;
				}
				int npos = nx * height + ny;
				if(fenClass_c[pos]!=fenClass_c[npos])
				{
					nup ++;
				}
			}
			if(nup > 1)
			{
				contourx[cou] = i;
				contoury[cou] = j;
				is_taken[main_taken] = true;
				cou ++;
			}
			main_taken ++;
		}
	}
	//染色
	for(int i = 0 ; i < width ; i++)
	{
		for(int j = 0 ; j < height ; j++)
		{
			*(blt + plt*j+ i*3) = 0xff;
			*(blt + plt*j+ i*3 + 1) = 0xff;
			*(blt + plt*j+ i*3 + 2) = 0xff;
		}
	}
	for(int i = 0 ;i<cou ; i++)
	{
		int pos = contourx[i] * height + contoury[i] ;
		*(blt + plt*contoury[i]+ contourx[i]*3) = 0xff;
		*(blt + plt*contoury[i]+ contourx[i]*3 + 1) = 0xff;
		*(blt + plt*contoury[i]+ contourx[i]*3 + 2) = 0xff;
		for(int j = 0 ; j < 8 ; j ++)
		{
			int nx = contourx[i] + Move[j][0];
			int ny = contoury[i] + Move[j][1];
			
			if(nx<0 || ny<0 || nx>= width || ny>=height)
			{
				continue;
			}
			int npos = nx * height + ny;
			if(!is_taken[npos])
			{
				*(blt + plt*ny + nx*3) = 0;
				*(blt + plt*ny + nx*3 + 1) = 0;
				*(blt + plt*ny + nx*3 + 2) = 0;
			}
		}
	}
}
void GMM_Core(	const vector<vector<vector<float>>> &lab,
				vector<vector<int>> &fenClass_c,
				vector<int> & point_num,
				int &k)
{
	//int width = lab.size();
	//int height  = lab[0].size();
	//int pixel_num = width * height;
	//int label_num = 5;
	//int label_num2 = label_num *label_num;
	//vector<double> pi(k);
	//vector<Tag> E(k);
	//vector<vector<double>> D(k);
	//vector<vector<double>> _D_(k);
	//vector<vector<double>> p(width);
	//vector<vector<vector<double>>> p_all(width);
	//for(int i = 0 ; i < width ; i++)
	//{
	//	//p_all
	//}
	//for(int i = 0 ; i < k ; i ++)
	//{
	//	_D_[i] = vector<double>(label_num2);
	//}
	//int MaxTime = 10;
	//int S = (int)sqrt(pixel_num / k);
	//
	//while(MaxTime --)
	//{
	//	for(int i=0;i<k;i++)
	//	{
	//		pi[i] = 0;
	//		E[i].r = 0;
	//		E[i].g = 0;
	//		E[i].b = 0;
	//		E[i].x = 0;
	//		E[i].y = 0;
	//		for(int j = 0; j < label_num2 ; j++)
	//		{
	//			_D_[i][j] = 0;
	//		}
	//		point_num[i] = 0;
	//	}
	//	for(int i=0;i<width;i++)
	//	{
	//		for(int j=0;j<height;j++)
	//		{
	//			int c = fenClass_c[i][j];
	//			if(c == -1)
	//				continue;
	//			point_num[c]++;
	//			E[c].r+=lab[i][j][0];
	//			E[c].g+=lab[i][j][1];
	//			E[c].b+=lab[i][j][2];
	//			E[c].x+=i;
	//			E[c].y+=j;
	//		}
	//	}
	//	for(int i = 0; i < k;i++)
	//	{
	//		pi[i] = 1.0 * point_num[i] / pixel_num;
	//	}
	//	vector<double> tmp(label_num2);
	//	for(int i = 0; i < width ; i++)
	//	{
	//		for(int j = 0;j < height ;j++)
	//		{
	//			int c = fenClass_c[i][j];
	//			if(c == -1)
	//				continue;
	//			vector<double> row(label_num);
	//			row[0] = (lab[i][j][0] - E[c].r) * (lab[i][j][0] - E[c].r);
	//			row[1] = (lab[i][j][1] - E[c].g) * (lab[i][j][1] - E[c].g);
	//			row[2] = (lab[i][j][2] - E[c].b) * (lab[i][j][2] - E[c].b);
	//			row[3] = (i - E[c].x) * (i - E[c].x);
	//			row[4] = (j - E[c].y) * (j - E[c].y);
	//			Muti_Matrix(row,row,label_num,1,label_num,tmp);
	//			Add_Matrix(_D_[c],tmp);
	//		}
	//	}
	//	Tag tmp;
	//	double pMax = -1;
	//	int c = 0;
	//	
	//	for(int i = 0 ; i < width ; i++)
	//	{
	//		for(int j = 0 ; j < height ; j++)
	//		{
	//			pMax = -1;
	//			c = 0;
	//			for(int w = 0 ; w < k ; w ++)
	//			{
	//				if(_D_[w] == 0)
	//					continue;
	//				tmp.r = lab[i][j][0] - E[w].r;
	//				tmp.g = lab[i][j][1] - E[w].g;
	//				tmp.b = lab[i][j][2] - E[w].b;
	//				tmp.x = i - E[w].x;
	//				tmp.y = j - E[w].y;
	//				double mi = -0.5 * (tmp.r / D[w].r * tmp.r + 
	//									tmp.g / D[w].g * tmp.g + 
	//									tmp.b / D[w].b * tmp.b +
	//									tmp.x / D[w].x * tmp.x +
	//									tmp.y / D[w].y * tmp.y);
	//				
	//				double pw = INFF * pi[w] / sqrt(2 * Pi * _D_[w]) * exp(mi);
	//				if(pMax < pw)
	//				{
	//					c = w;
	//					pMax = pw;
	//				}
	//			}
	//			fenClass_c[i][j] = c;
	//		}
	//	}		
	//}

}

void GMM_Core(	const vector<vector<float>> &lab,
				vector<int> &fenClass_c,
				int width,
				int height,
				int &k)
{
	int sz = width * height;
	int label_num = 5;
	int label_num2 = label_num * label_num;
	vector<vector<double>> pi(k,vector<double>(sz));
	vector<vector<double>> D(k,vector<double>(label_num2,0));
	vector<vector<double>> E(k,vector<double>(label_num,0));
	vector<double> P(k,0);
	vector<vector<double>> k_tags(k,vector<double>(label_num,0));
	vector<int> k_tags_num(k,0);

	//构造初始参数
	for(int i = 0 ; i < width ; i ++)
	{
		for(int j = 0 ; j < height ; j++)
		{
			int pos = i * height + j;
			int c = fenClass_c[pos];
			k_tags[c][0] += lab[0][pos];
			k_tags[c][1] += lab[1][pos];
			k_tags[c][2] += lab[2][pos];
			k_tags[c][3] += i;
			k_tags[c][4] += j;
			k_tags_num[c] ++;
		}
	}
	for(int i = 0 ; i < k ; i ++)
	{
		if( k_tags_num[i] == 0)
			continue;
		P[i] = 1.0 * k_tags_num[i] / sz;
		E[i][0] = k_tags[i][0] / k_tags_num[i] ;
		E[i][1] = k_tags[i][1] / k_tags_num[i] ;
		E[i][2] = k_tags[i][2] / k_tags_num[i] ;
		E[i][3] = k_tags[i][3] / k_tags_num[i] ;
		E[i][4] = k_tags[i][4] / k_tags_num[i] ;
	}
	k_tags.clear();
	vector<double> et(label_num);
	vector<double> dt(label_num2);
	for(int i = 0 ; i < width ; i ++)
	{
		for(int j = 0 ; j < height ; j ++)
		{
			int pos = i * height + j;
			int c = fenClass_c[pos];
			et[0] = lab[0][pos] - E[c][0];
			et[1] = lab[1][pos] - E[c][1];
			et[2] = lab[2][pos] - E[c][2];
			et[3] = i - E[c][3];
			et[4] = j - E[c][4];
			Muti_Matrix(et,et,label_num,1,label_num,dt);
			for(int w = 0 ; w < label_num2 ; w ++)
			{
				D[c][w] += dt[w];
			}
		}
	}
	for(int i = 0 ; i < k ; i ++)
	{
		if( k_tags_num[i] == 0)
			continue;
		for(int w = 0 ; w < label_num2 ; w ++)
		{
			D[i][w] /= k_tags_num[i];
		}
	}

	//

	int MaxStepTime = 10;
	vector<CMatrix> all_matrix(k,CMatrix(label_num));
	vector<double> all_val(k);
	vector<double> subt(label_num);
	vector<double> tmp(label_num);
	vector<double> ans(1);
	vector<double> sum_pi(sz);
	vector<double> tmp_pi(k);
	while(MaxStepTime -- )
	{
		for(int i = 0 ; i < k; i ++)
		{
			all_val[i] = all_matrix[i].inverse(D[i]);
		}
		//后验概率
		for(int w = 0 ; w < k ; w ++)
		{
			if(all_val[w] == 0)
				continue;
			for(int i = 0 ;i < width ; i ++)
			{
				for(int j = 0 ; j < height ; j ++)
				{
					int pos = i * height + j;
					double sum = 0;
					subt[0] = lab[0][pos] - E[w][0];
					subt[1] = lab[1][pos] - E[w][1];
					subt[2] = lab[2][pos] - E[w][2];
					subt[3] = i - E[w][3];
					subt[4] = j - E[w][4];
					Muti_Matrix(subt,all_matrix[w].m_matrix,1,label_num,label_num,tmp);
					Muti_Matrix(tmp,subt,1,label_num,1,ans);
					pi[w][pos] = P[w] / sqrt(2 * Pi * all_val[w]) * exp(-0.5 * ans[0]);
				}
			}
		}
		
		for(int pos = 0 ; pos < sz ;pos++)
		{
			double sum_tmp = 0;
			for(int w = 0 ; w < k ; w ++)
			{
				sum_tmp += pi[w][pos];
			}
			for(int w = 0 ;w < k ; w ++)
			{
				pi[w][pos] /= sum_tmp;
			}
		}
		//求pi
		for(int w = 0 ; w < k; w ++)
		{
			double sum_tmp = 0;
			for(int pos = 0 ; pos < sz ;pos++)
			{
				sum_tmp += pi[w][pos];
			}
			P[w] = sum_tmp / k_tags_num[w];
			tmp_pi[w] = sum_tmp;
		}

		//求u
		for(int w = 0 ; w < k; w ++)
		{
			E[w][0] = 0;
			E[w][1] = 0;
			E[w][2] = 0;
			E[w][3] = 0;
			E[w][4] = 0;
			for(int i = 0 ; i < width ; i ++)
			{
				for(int j = 0 ; j < height ; j ++)
				{
					int pos = i * height +j;
					E[w][0] += pi[w][pos] * lab[0][pos] ;
					E[w][1] += pi[w][pos] * lab[1][pos] ;
					E[w][2] += pi[w][pos] * lab[2][pos] ;
					E[w][3] += pi[w][pos] * i ;
					E[w][4] += pi[w][pos] * j ;
				}
			}
			E[w][0] /= tmp_pi[w];
			E[w][1] /= tmp_pi[w];
			E[w][2] /= tmp_pi[w];
			E[w][3] /= tmp_pi[w];
			E[w][4] /= tmp_pi[w];
		}
		//求协方差矩阵
		for(int w = 0 ; w < k; w ++)
		{
			for(int i = 0 ; i < label_num2 ; i ++)
			{
				D[w][i] = 0;
			}
			for(int i = 0; i < width ; i ++)
			{
				for(int j = 0; j < height ; j++)
				{
					int pos = i * height + j;
					subt[0] = lab[0][pos] - E[w][0];
					subt[1] = lab[1][pos] - E[w][1];
					subt[2] = lab[2][pos] - E[w][2];
					subt[3] = i - E[w][3];
					subt[4] = j - E[w][4];
					Muti_Matrix(subt,subt,label_num,1,label_num,dt);
					for(int t = 0 ; t < label_num2 ; t ++)
					{
						D[w][t] += pi[w][pos] * dt[t];
					}
				}
			}
			for(int i = 0 ; i < label_num2 ; i ++)
			{
				D[w][i] /= tmp_pi[w];
			}
		} 

	}
	//EM算法结束

	for(int pos = 0 ; pos < sz ; pos ++)
	{
		int max_index = 0;
		for(int w = 1 ; w < k ; w ++)
		{
			if(pi[max_index][pos] < pi[w][pos])
			{
				max_index = w;
			}
		}
		fenClass_c[pos] = max_index;
	}
}
void GMM_Core1(	const vector<vector<vector<float>>> &lab,
				vector<vector<int>> &fenClass_c,
				vector<int> & point_num,
				int &k)
{
	int width = lab.size();
	int height  = lab[0].size();
	int pixel_num = width * height;
	vector<double> pi(k);
	vector<Tag> E(k);
	vector<Tag> D(k);
	vector<double> _D_(k);
	vector<vector<double>> p(width);
	for(int i = 0 ; i < width ; i++)
	{
		p [i] = vector<double>(height);
	}
	int MaxTime = 10;
	int S = (int)sqrt(pixel_num / k);
	
	while(MaxTime --)
	{
		for(int i=0;i<k;i++)
		{
			pi[i] = 0;
			E[i].r = 0;
			E[i].g = 0;
			E[i].b = 0;
			E[i].x = 0;
			E[i].y = 0;
			D[i].r = 0;
			D[i].g = 0;
			D[i].b = 0;
			D[i].x = 0;
			D[i].y = 0;
			point_num[i] = 0;
		}
	
		for(int i=0;i<width;i++)
		{
			for(int j=0;j<height;j++)
			{
				int c = fenClass_c[i][j];
				point_num[c]++;
				E[c].r+=lab[i][j][0];
				E[c].g+=lab[i][j][1];
				E[c].b+=lab[i][j][2];
				E[c].x+=i;
				E[c].y+=j;
			}
		}
		for(int i = 0; i < k;i++)
		{
			E[i].r /= point_num[i];
			E[i].g /= point_num[i];
			E[i].b /= point_num[i];
			E[i].x /= point_num[i];
			E[i].y /= point_num[i];
			pi[i] = 1.0 * point_num[i] / pixel_num;
		}
		for(int i = 0; i < width ; i++)
		{
			for(int j = 0;j < height ;j++)
			{
				int c = fenClass_c[i][j];
				if(c == -1)
					continue;
				D[c].r += (lab[i][j][0] - E[c].r) * (lab[i][j][0] - E[c].r);
				D[c].g += (lab[i][j][1] - E[c].g) * (lab[i][j][1] - E[c].g);
				D[c].b += (lab[i][j][2] - E[c].b) * (lab[i][j][2] - E[c].b);
				D[c].x += (i - E[c].x) * (i - E[c].x);
				D[c].y += (j - E[c].y) * (j - E[c].y);
			}
		}
		for(int i = 0 ;i < k;i++)
		{
			D[i].r /= point_num[i];
			D[i].g /= point_num[i];
			D[i].b /= point_num[i];
			D[i].x /= point_num[i];
			D[i].y /= point_num[i];
			_D_[i] = D[i].r * D[i].g * D[i].b * D[i].x * D[i].y;
		}
		Tag tmp;
		for(int i = 0 ;i < width ; i++)
		{
			for(int j = 0 ;j < height ; j++)
			{
				p[i][j] = -1;
				fenClass_c[i][j] = 0;
			}
		}

		for(int i = 0 ; i < k ; i++)
		{
			if(_D_[i] == 0)
				continue;
			int sx = max(E[i].x - S * 2 , 0);
			int sy = max(E[i].y - S * 2 , 0);
			int ex = min(E[i].x + S * 2 , width);
			int ey = min(E[i].y + S * 2 , height);
			for(int ii = sx ; ii < ex ; ii ++)
			{
				for(int jj = sy ; jj < ey ; jj++)
				{
					
					tmp.r = lab[ii][jj][0] - E[i].r;
					tmp.g = lab[ii][jj][1] - E[i].g;
					tmp.b = lab[ii][jj][2] - E[i].b;
					tmp.x = ii - E[i].x;
					tmp.y = jj - E[i].y;
					double mi = -0.5 * (tmp.r / D[i].r * tmp.r + 
										tmp.g / D[i].g * tmp.g + 
										tmp.b / D[i].b * tmp.b +
										tmp.x / D[i].x * tmp.x +
										tmp.y / D[i].y * tmp.y);
					
					double pw = INFF * pi[i] / sqrt(2 * Pi * _D_[i]) * exp(mi);
					if(p[ii][jj] < pw)
					{
						p[ii][jj] = pw;
						fenClass_c[ii][jj] = i;
					}
				}
			}
		}
		
	}
}

void LogDebug(const vector<vector<int>> & fenClass_c,CString path)
{
	int width = fenClass_c.size();
	int height = fenClass_c[0].size();
	FILE * file = NULL;
	fopen_s(&file,path.GetBuffer(),"w");
	for(int i = 0; i < width ; i++)
	{
		for(int j = 0 ; j < height ;j++)
		{
			fprintf_s(file,"%10d",fenClass_c[i][j]);
		}
		fprintf_s(file,"\n");
	}
	fclose(file);
}

void LogDebug(const vector<int> & fenClass_c,int width,int height,CString path)
{
	FILE * file = NULL;
	fopen_s(&file,path.GetBuffer(),"w");
	for(int i = 0; i < width ; i++)
	{
		for(int j = 0 ; j < height ;j++)
		{
			fprintf_s(file,"%10d",fenClass_c[i * height + j]);
		}
		fprintf_s(file,"\n");
	}
	fclose(file);
}
void KMeans_Core(	const vector<vector<vector<float>>> &lab,
					const vector<vector<double>> &grad,
					vector<vector<int>> &fenClass_c,
					int &k,
					double M)
{
	int MaxTime = 20;
	int width = lab.size();
	int height = lab[0].size();
	vector<Tag> k_tags;
	fenClass_c = vector<vector<int>>(width);
	for(int i = 0 ; i < width ; i++)
	{
		fenClass_c[i] = vector<int>(height);
	}
	int pixel_num = width * height;
	int S = (int)sqrt(pixel_num / k) + 1;
	for(int i = 0 ; i * S < width ; i++ )
	{
		for(int j = 0 ; j * S < height ; j++)
		{
			int si = i * S;
			int sj = j * S;
			k_tags.push_back(Tag(0,0,0,si,sj));
		}
	}
	k = k_tags.size();
	int Move[][2] = {0,1,0,-1,1,0,-1,0,1,1,1,-1,-1,1,-1,-1}; 
	for(int i = 0 ; i < k ; i ++)
	{
		while(true)
		{
			double pos_x = (int)k_tags[i].x;
			double pos_y = (int)k_tags[i].y;
			for(int j = 0 ; j < 8 ; j ++)
			{
				int nx = k_tags[i].x + Move[j][0];
				int ny = k_tags[i].y + Move[j][1];
				if(nx < 0 || nx >= width || ny < 0 || ny >= height)
					continue;
				if(grad[pos_x][pos_y] > grad[nx][ny])
				{
					pos_x = nx;
					pos_y = ny;
				}
			}
			if(pos_x == k_tags[i].x && pos_y == k_tags[i].y)
			{
				k_tags[i].r = lab[pos_x][pos_y][0];
				k_tags[i].g = lab[pos_x][pos_y][1];
				k_tags[i].b = lab[pos_x][pos_y][2];
				break;
			}
			k_tags[i].x = pos_x;
			k_tags[i].y = pos_y;

		}
	}
	vector<vector<CPoint>> vec(k);
	while( MaxTime --)
	{
		for(int i=0;i<k;i++)
		{
			vec[i].clear();
		}
		for(int i=0;i<width;i++)
		{
			for(int j=0;j<height;j++)
			{
				int c=0;
				double sub = INFF;
				
				for(int w=0;w<k;w++)
				{
					double subr = lab[i][j][0]-k_tags[w].r;
					double subg = lab[i][j][1]-k_tags[w].g;
					double subb = lab[i][j][2]-k_tags[w].b;
					int subx = i-k_tags[w].x;
					int suby = j-k_tags[w].y;
					double nowSub = (subr*subr + subg*subg + subb*subb + subx*subx*M + suby*suby*M);

					if(sub >nowSub)
					{
						c = w;
						sub = nowSub;
					}
				}
				vec[c].push_back(CPoint(i,j));
			}
		}
		for(int i=0;i<k;i++)
		{
			int len = vec[i].size();
			if(len == 0)
				continue;
			double r=0,g=0,b=0;
			int x=0,y=0;
			for(int j=0;j<len;j++)
			{
				int posX = vec[i][j].x;
				int posY = vec[i][j].y;
				r+=lab[posX][posY][0];
				g+=lab[posX][posY][1];
				b+=lab[posX][posY][2];
				x+=posX;
				y+=posY;
			}
			k_tags[i].r = r / len;
			k_tags[i].g = g / len;
			k_tags[i].b = b / len;
			k_tags[i].x = x / len;
			k_tags[i].y = y / len;
		}
	}
	for(int i = 0 ; i < k ;i++)
	{
		int len = vec[i].size();
		for(int j = 0 ; j < len ; j++)
		{
			fenClass_c[vec[i][j].x][vec[i][j].y] = i; 
		}
	}
}

void ICM_Core(const vector<vector<float>> &lab,
				vector<int> &fenClass_c,
				int width,
				int height,
				int &k)
{
	int label_num = 5;
	int T = 1.5;
	int label_num2 = label_num * label_num;
	int sz = width * height;
	vector<vector<double>> E(k,vector<double>(label_num,0));
	vector<vector<double>> D(k,vector<double>(label_num2,0));
	vector<int> c_num(k,0);
	vector<double>e(label_num);
	vector<double>t(label_num);
	vector<double>d(label_num2);
	vector<int> fenClass_t(sz);
	vector<double> ans(1);
	for(int i = 0 ; i < width ; i ++)
	{
		for(int j = 0 ; j < height ; j++)
		{
			int pos = i * height + j;
			int c = fenClass_c[pos];
			E[c][0] += lab[0][c];
			E[c][1] += lab[1][c];
			E[c][2] += lab[2][c];
			E[c][3] += i;
			E[c][4] += j;
			c_num[c] ++;
		}
	}
	for(int i = 0 ; i < k ; i ++)
	{
		if(c_num[i] == 0)
			continue;
		E[i][0] /= c_num[i];
		E[i][1] /= c_num[i];
		E[i][2] /= c_num[i];
		E[i][3] /= c_num[i];
		E[i][4] /= c_num[i];
	}
	for(int i = 0 ; i < width ; i ++)
	{
		for(int j = 0 ; j < height ; j++)
		{
			int pos = i * height + j;
			int c = fenClass_c[pos];
			e[0] = lab[0][c] - E[c][0];
			e[1] = lab[1][c] - E[c][1];
			e[2] = lab[2][c] - E[c][2];
			e[3] = i - E[c][3];
			e[4] = j - E[c][4];
			Muti_Matrix(e,e,label_num,1,label_num,d);
			for(int w = 0 ; w < label_num2 ;w ++)
			{
				D[c][w] += d[w];
			}
		}
	}
	for(int i = 0 ; i < k ; i ++)
	{
		if(c_num[i] == 0)
			continue;
		for(int w = 0 ; w < label_num2; w++)
		{
			D[i][w] /= c_num[i];
		}
	}
	int Max_Time = 10;
	int Move8[][2] = {0,1,0,-1,1,0,-1,0,1,1,1,-1,-1,1,-1,-1};
	vector<int> num(k,0);
	vector<double> P(k);
	int s_8[8];
	int spos = 0;
	CMatrix matrix(label_num);
	while(Max_Time -- )
	{
		for(int i = 0 ; i < sz ; i++)
			fenClass_t[i] = fenClass_c[i];
		for(int i = 0 ; i < width ; i ++)
		{
			for(int j = 0 ; j < height ; j++)
			{
				int pos = i * height + j;
				int c = fenClass_t[pos];
				for(int w = 0 ; w < 8 ; w++)
				{
					int nx = i + Move8[w][0];
					int ny = j + Move8[w][1];
					int npos = nx * height + ny;
					if(npos <0 || npos >=sz)
						continue;
					if(num[fenClass_t[npos]] == 0)
					{
						s_8[spos] = fenClass_t[npos];
						spos ++;
					}
					num[fenClass_t[npos]] ++;
				}
				double z = 0;
				for(int w = 0 ; w < spos ; w ++)
				{
					int tnum = num[s_8[w]];
					int b = tnum + tnum - 8;
					P[s_8[w]] = exp(-1.0 * b / T);
					z += P[s_8[w]];
				}
				for(int w = 0 ; w < spos ; w ++)
				{
					P[s_8[w]] /= z;
				}
				int tc = c;
				int max_pi = -1;
				for(int w = 0 ; w < spos ; w ++)
				{
					int c = s_8[w];
					double dv = matrix.inverse(D[c]);
					if(dv == 0)
						continue;
					d[0] = lab[0][pos] - E[c][0];
					d[1] = lab[1][pos] - E[c][1];
					d[2] = lab[2][pos] - E[c][2];
					d[3] = i - E[c][3];
					d[4] = j - E[c][4];
					Muti_Matrix(d,matrix.m_matrix,1,label_num,label_num,t);
					Muti_Matrix(t,d,1,label_num,1,ans);
					double pi = P[c] / pow(2*Pi,2.5) / sqrt(dv) *exp(-0.5 * ans[0]);  
					if(pi > max_pi)
					{
						tc = c;
						max_pi = pi;
					}
				}
				fenClass_c[pos] = tc;
				for(int w = 0 ; w < spos ; w ++)
				{
					num[s_8[w]] = 0;
				}
				spos = 0;
			}
		}
	}
	
}

void Avg_n2(vector<vector<float>> & lab,int width,int height,int n)
{
	int sz = width * height;
	int len = lab.size();
	vector<vector<float>> lab2(3,vector<float>(sz));

	lab2[0][0] = lab[0][0];
	lab2[1][0] = lab[1][0];
	lab2[2][0] = lab[2][0];
	for(int i = 1 ; i < width ; i++)
	{
		int pos = i * height;
		int pos_ = pos - height;
		lab2[0][pos] = lab[0][pos] + lab2[0][pos_];
		lab2[1][pos] = lab[1][pos] + lab2[1][pos_];
		lab2[2][pos] = lab[2][pos] + lab2[2][pos_];
	}
	for(int i = 1 ; i < height ; i++)
	{
		lab2[0][i] = lab[0][i] + lab2[0][i-1];
		lab2[1][i] = lab[1][i] + lab2[1][i-1];
		lab2[2][i] = lab[2][i] + lab2[2][i-1];
	}
	for(int i = 1 ; i < width ; i++)
	{
		for(int j = 1; j < height ; j++)
		{
			int pos = i * height + j;
			int pos1 = pos - height;
			int pos2 = pos - 1;
			int pos3 = pos1 - 1;
			lab2[0][pos] = lab[0][pos] + lab2[0][pos1] + lab2[0][pos2] - lab2[0][pos3];
			lab2[1][pos] = lab[1][pos] + lab2[1][pos1] + lab2[1][pos2] - lab2[1][pos3];
			lab2[2][pos] = lab[2][pos] + lab2[2][pos1] + lab2[2][pos2] - lab2[2][pos3];
		}
	}
	for(int i = 0; i < width ; i++)
	{
		for(int j = 0 ; j < height ; j++)
		{
			int min_x = max(0, i - n );
			int min_y = max(0, j - n);
			int max_x = min(i + n, width-1);
			int max_y = min(j + n, height -1);
			min_x --;
			min_y --;
			int pos = i * height + j;
			int pos1 = min_x * height + min_y;
			int pos2 = max_x * height + max_y;
			int pos3 = min_x * height + max_y;
			int pos4 = max_x * height + min_y;
			int lensz = (max_x - min_x) * (max_y - min_y);
			if(min_x >=0 && min_y >=0)
			{
				lab[0][pos] = lab2[0][pos2] + lab2[0][pos1] - lab2[0][pos3] - lab2[0][pos4];
				lab[1][pos] = lab2[1][pos2] + lab2[1][pos1] - lab2[1][pos3] - lab2[1][pos4];
				lab[2][pos] = lab2[2][pos2] + lab2[2][pos1] - lab2[2][pos3] - lab2[2][pos4];
			}
			else if(min_x <0 && min_y < 0)
			{
				lab[0][pos] = lab2[0][pos2] ;
				lab[1][pos] = lab2[1][pos2] ;
				lab[2][pos] = lab2[2][pos2] ;
			}
			else if(min_x < 0)
			{
				lab[0][pos] = lab2[0][pos2] - lab2[0][pos4];
				lab[1][pos] = lab2[1][pos2] - lab2[1][pos4];
				lab[2][pos] = lab2[2][pos2] - lab2[2][pos4];
			}
			else 
			{
				lab[0][pos] = lab2[0][pos2] - lab2[0][pos3];
				lab[1][pos] = lab2[1][pos2] - lab2[1][pos3];
				lab[2][pos] = lab2[2][pos2] - lab2[2][pos3];
			}
			lab[0][pos] /= lensz;
			lab[1][pos] /= lensz;
			lab[2][pos] /= lensz;
		}
	}
}

