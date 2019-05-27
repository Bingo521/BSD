#include "stdafx.h"
//
using namespace std;
using namespace Eigen;

extern CImage debugImg;
MyLog logs("123.txt");
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

bool GetSubImg(const CImage &res,CImage &des,CPoint pos,int width,int height)
{
	des.Create(width,height,res.GetBPP());
	CImageDC imageDC(des);
	POINT p;
	CRect rect = CRect(0,0,width,height);
	p.x=pos.x;
	p.y=pos.y;
	res.BitBlt(imageDC,rect,p);
	return true;
}

CString GetPath(CString path,CString app)
{
	int i;
	for(i=path.GetLength();i>0;i--)
	{
		if(path[i]=='.')
		{
			break;
		}
	}
	CString left =  path.Left(i);
	CString right = path.Right(path.GetLength()-i);
	return left+app+right;
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
	//str.Format("--����%.2lf",difftime(et,st));
	//AfxMessageBox(str);
	//ת����ɫ�ռ�

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
	str.Format("--����%.2lf",difftime(et,st));
	AfxMessageBox(str);*/
	//ת����ɫ�ռ�
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
	//��ʼ����
	//ѡȡ��ʼֵ����
	//��ʼEM
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
		//��������
		pre_point = k_tags;
		//���¼�������
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
		//�ж��Ƿ����
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
	//��ʼ����
	//ѡȡ��ʼֵ����
	//��ʼEM
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
		//���¼�������
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

	//����ռ䡢�������
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

	//����lab��
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
	//��ʼ��
	for(int i = 0 ; i < width ; i ++)
	{
		for(int j = 0 ; j < height ; j ++)
		{
			is_visit[i][j] = false;
		}
	}

	//���»�����
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
	//����С��
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

	//����ռ䡢�������
	vector<int> fenClass_r(sz);
	vector<bool> is_visit(sz,false);
	queue<CPoint> point_c;
	vector<CPoint> point_r;
	vector<Tag> new_class;
	vector<int> new_class_num;
	//���»�����
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
	//����С��
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

			if(abs(maxx - minx - maxy + miny) < MinDisY || point_r.size() < MinPixelNum)
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

	//����ռ䡢�������
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
	//��ʼ��
	for(int i = 0 ; i < width ; i ++)
	{
		for(int j = 0 ; j < height ; j ++)
		{
			is_visit[i][j] = false;
		}
	}

	//���»�����
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

	//����С��
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

	//����ռ䡢�������
	vector<vector<int>> fenClass_r(width);
	vector<vector<bool>> is_visit(width);
	queue<CPoint> point_c;
	vector<CPoint> point_r;
	for(int i = 0 ; i < width ; i++)
	{
		fenClass_r[i] = vector<int>(height);
		is_visit[i] = vector<bool>(height);
	}
	//��ʼ��
	vector<DPoint> ZX(k);
	for(int i = 0 ; i < width ; i ++)
	{
		for(int j = 0 ; j < height ; j ++)
		{
			is_visit[i][j] = false;
		}
	}
	//������
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
	//���»�����
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

	//����С��
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
	//Ⱦɫ
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
	//int LABEL_NUM = 5;
	//int LABEL_NUM2 = LABEL_NUM *LABEL_NUM;
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
	//	_D_[i] = vector<double>(LABEL_NUM2);
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
	//		for(int j = 0; j < LABEL_NUM2 ; j++)
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
	//	vector<double> tmp(LABEL_NUM2);
	//	for(int i = 0; i < width ; i++)
	//	{
	//		for(int j = 0;j < height ;j++)
	//		{
	//			int c = fenClass_c[i][j];
	//			if(c == -1)
	//				continue;
	//			vector<double> row(LABEL_NUM);
	//			row[0] = (lab[i][j][0] - E[c].r) * (lab[i][j][0] - E[c].r);
	//			row[1] = (lab[i][j][1] - E[c].g) * (lab[i][j][1] - E[c].g);
	//			row[2] = (lab[i][j][2] - E[c].b) * (lab[i][j][2] - E[c].b);
	//			row[3] = (i - E[c].x) * (i - E[c].x);
	//			row[4] = (j - E[c].y) * (j - E[c].y);
	//			Muti_Matrix(row,row,LABEL_NUM,1,LABEL_NUM,tmp);
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
	const double powd = pow(2*Pi,LABEL_NUM);
	vector<vector<double>> pi(k,vector<double>(sz));
	vector<vector<double>> D(k,vector<double>(LABEL_NUM2,0));
	vector<vector<double>> E(k,vector<double>(LABEL_NUM,0));
	vector<double> P(k,0);
	vector<vector<double>> k_tags(k,vector<double>(LABEL_NUM,0));
	vector<int> k_tags_num(k,0);
	//CMatrix matrix(LABEL_NUM);
	//CMatrixEx matrixEx(LABEL_NUM);
	MatrixQuick mat(LABEL_NUM);
	//�����ʼ����
	int pos=0;
	for(int i = 0 ; i < width ; i ++)
	{
		for(int j = 0 ; j < height ; j++)
		{
			int c = fenClass_c[pos];
			if(c >=k || c <0)
			{
				pos++;
				continue;
			}
			k_tags[c][0] += lab[0][pos];
			k_tags[c][1] += lab[1][pos];
			k_tags[c][2] += lab[2][pos];
#ifdef LABEL5
			k_tags[c][3] += i;
			k_tags[c][4] += j;
#endif
			k_tags_num[c] ++;
			pos++;
		}
	}
	//��ʼ��ֵ
	for(int i = 0 ; i < k ; i ++)
	{
		if( k_tags_num[i] == 0)
			continue;
		P[i] = 1.0 * k_tags_num[i] / sz;
		E[i][0] = k_tags[i][0] / k_tags_num[i] ;
		E[i][1] = k_tags[i][1] / k_tags_num[i] ;
		E[i][2] = k_tags[i][2] / k_tags_num[i] ;
#ifdef LABEL5
		E[i][3] = k_tags[i][3] / k_tags_num[i] ;
		E[i][4] = k_tags[i][4] / k_tags_num[i] ;
#endif
	}
	//��ʼЭ�������
	vector<double> et(LABEL_NUM);
	vector<double> dt(LABEL_NUM2);
	pos=0;
	for(int i = 0 ; i < width ; i ++)
	{
		for(int j = 0 ; j < height ; j ++)
		{
			int c = fenClass_c[pos];
			if(c == -1)
			{
				pos++;
				continue;
			}
			et[0] = lab[0][pos] -	E[c][0];
			et[1] = lab[1][pos] -	E[c][1];
			et[2] = lab[2][pos] -	E[c][2];
#ifdef LABEL5
			et[3] = i -				E[c][3];
			et[4] = j -				E[c][4];
#endif
			mat.Mul(&et[0],&et[0],LABEL_NUM,1,LABEL_NUM,&dt[0]);
			for(int w = 0 ; w < LABEL_NUM2 ; w ++)
			{
				D[c][w] +=dt[w];
			}
			pos++;
		}
	}
	for(int i = 0 ; i < k ; i ++)
	{
		if( k_tags_num[i] == 0)
			continue;
		for(int w = 0 ; w < LABEL_NUM2 ; w ++)
		{
			D[i][w] /= k_tags_num[i];
		}
	}

	//

	int MaxStepTime = 30;
	vector<vector<double>> D_(k,vector<double>(LABEL_NUM2));
	vector<double>dv(k);
	vector<double> subt(LABEL_NUM);
	vector<double> tmp(LABEL_NUM);
	vector<double> ans(1);
	vector<double> tmp_pi(k,1);
	vector<double> sums(sz);
	while(MaxStepTime -- )
	{
		for(int i = 0 ; i < k; i ++)
		{
			if(tmp_pi[i] == 0)
				continue;
			bool not_ok = mat.Inverse(&D[i][0],&D_[i][0]);
			if(not_ok)
			{
				dv[i] = abs(mat.Det(&D[i][0]));
			}
			if(not_ok || dv[i] < 1e-10)
			{
				for(int ii = 0 ;ii < LABEL_NUM;ii++)
				{
					D[i][ii*LABEL_NUM+ii] += 1e-10;
				}
				mat.Inverse(&D[i][0],&D_[i][0]);
				dv[i] = abs(mat.Det(&D[i][0]));
			}
		}
		//�������
		pos=0;
		for(int i=0;i<sz;i++)
		{
			sums[i] = 0;
		}
		for(int i = 0 ;i < width ; i ++)
		{
			for(int j = 0 ; j < height ; j ++)
			{
				//int pos = i * height + j;
				double sum = 0;
				for(int w = 0 ; w < k ; w ++)
				{
					if(dv[w] == 0)
					{
						continue;
					}
					subt[0] = lab[0][pos] - E[w][0];
					subt[1] = lab[1][pos] - E[w][1];
					subt[2] = lab[2][pos] - E[w][2];
#ifdef LABEL5
					subt[3] = i -			E[w][3];
					subt[4] = j -			E[w][4];
#endif
					mat.Mul(&subt[0],&D_[w][0],1,LABEL_NUM,LABEL_NUM,&tmp[0]);
					mat.Mul(&tmp[0],&subt[0],1,LABEL_NUM,1,&ans[0]);
					pi[w][pos] = P[w]/(powd * sqrt(dv[w])) * exp(-0.5 * ans[0]) ;
					sums[pos]+=pi[w][pos];
				}
				pos++;
			}
		}
		
		for(int i=0;i<sz;i++)
		{
			for(int j=0;j<k;j++)
			{
				pi[j][i]/=sums[i];
			}
		}

		//LogDebug(pi,width,height,k,"Logs",MaxStepTime);
		//��P[�������]
		double sum_nk=0;
		for(int w = 0 ; w < k; w ++)
		{
			double sum_tmp = 0;
			for(int pos = 0 ; pos < sz ;pos++)
			{
				sum_tmp += pi[w][pos];
			}
			P[w] = sum_tmp / sz;
			tmp_pi[w] = sum_tmp;
			sum_nk += sum_tmp;
		}
		/*CString str;
		str.Format("%lf %d",sum_nk,sz);
		AfxMessageBox(str);*/
		//��u
		for(int w = 0 ; w < k; w ++)
		{
			if(tmp_pi[w] < LIM0)
			{
				continue;
			}
			E[w][0] = 0;
			E[w][1] = 0;
			E[w][2] = 0;
#ifdef LABEL5
			E[w][3] = 0;
			E[w][4] = 0;
#endif
			pos=0;
			for(int i = 0 ; i < width ; i ++)
			{
				for(int j = 0 ; j < height ; j ++)
				{
					//int pos = i * height +j;
					E[w][0] += pi[w][pos] * lab[0][pos] ;
					E[w][1] += pi[w][pos] * lab[1][pos] ;
					E[w][2] += pi[w][pos] * lab[2][pos] ;
#ifdef LABEL5
					E[w][3] += pi[w][pos] * i ;
					E[w][4] += pi[w][pos] * j ;
#endif
					pos++;
				}
			}
			E[w][0] /= tmp_pi[w];
			E[w][1] /= tmp_pi[w];
			E[w][2] /= tmp_pi[w];
#ifdef LABEL5
			E[w][3] /= tmp_pi[w];
			E[w][4] /= tmp_pi[w];
#endif
		}
		//��Э�������
		for(int w = 0 ; w < k; w ++)
		{
			if(tmp_pi[w] < LIM0)
			{
				continue;
			}
			for(int i = 0 ; i < LABEL_NUM2 ; i ++)
			{
				D[w][i] = 0;
			}
			pos=0;
			for(int i = 0; i < width ; i ++)
			{
				for(int j = 0; j < height ; j++)
				{
					//int pos = i * height + j;
					subt[0] = lab[0][pos] - E[w][0];
					subt[1] = lab[1][pos] - E[w][1];
					subt[2] = lab[2][pos] - E[w][2];
#ifdef LABEL5
					subt[3] = i - E[w][3];
					subt[4] = j - E[w][4];
#endif
					mat.Mul(&subt[0],&subt[0],LABEL_NUM,1,LABEL_NUM,&dt[0]);
					for(int t = 0 ; t < LABEL_NUM2 ; t ++)
					{
						D[w][t] += pi[w][pos] * dt[t];
					}
					pos++;
				}
			}
			for(int i = 0 ; i < LABEL_NUM2 ; i ++)
			{
				D[w][i] /= tmp_pi[w];
			}
		} 

	}
	//EM�㷨����
	//FILE * file = NULL;
	//fopen_s(&file,"Logs","w");
	//for(int pos = 0 ; pos < sz ; pos ++)
	//{
	//	int max_index = 0;
	//	for(int w = 1 ; w < k ; w ++)
	//	{
	//		if(pi[max_index][pos] < pi[w][pos])
	//		{
	//			max_index = w;
	//		}
	//		
	//	}
	//	fprintf_s(file,"%10.2lf",pi[max_index][pos]);
	//	if((pos + 1) % width == 0)
	//	fprintf_s(file,"\n");
	//	fenClass_c[pos] = max_index;
	//}
	//fclose(file);
	//LogDebug(fenClass_c,width,height,"Log");
}
void GMM_Core_(	const vector<vector<float>> &lab,
				vector<int> &fenClass_c,
				int width,
				int height,
				int &k)
{
	//��������
	const int sz = width * height;
	const int kInt = sizeof(int) * k;
	const int kDouble = sizeof(double) * k;
	const int szDouble = sizeof(double) * sz;
	const int labelDouble = sizeof(double) * LABEL_NUM;
	const int label2Double = sizeof(double) * LABEL_NUM2;
	const double pow_pi = pow(2*Pi,LABEL_NUM / 2.0);
	//��ʱ��������
	int tmp;
	vector<double> sub1(LABEL_NUM);
	vector<double> sub2(LABEL_NUM2);
	double ans;
	//ר�ñ�������
	int Max_Time = 30;
	vector<vector<double>> P(k,vector<double>(sz));
	vector<double> Q(k);
	vector<vector<double>> E(k,vector<double>(LABEL_NUM));
	vector<vector<double>> D(k,vector<double>(LABEL_NUM2));
	vector<vector<double>> D_(k,vector<double>(LABEL_NUM2));
	vector<double> Det(k);
	vector<double> Nk(k);
	MatrixQuick mat(LABEL_NUM);
	
	//��ʼ������
	for(int i = 0 ; i< k ; i ++)
	{
		ZeroMemory(&P[i][0],szDouble);
	}
	for(int i = 0 ; i < sz ; i++)
	{
		tmp = fenClass_c[i];
		P[tmp][i] = 1;
	}
	while(Max_Time --)
	{
		//��0
		ZeroMemory(&Nk[0],kDouble);
		for(int i = 0; i < k ; i ++ )
		{
			ZeroMemory(&E[i][0],labelDouble);
			ZeroMemory(&D[i][0],label2Double);
		}
		//��Nk
		for(int i = 0; i <k;i++)
		{
			for(int j=0;j<sz;j++)
			{
				Nk[i]+=P[i][j];
			}
		}
		//debug sum(Nk)
		double sum_Nk=0;
		for(int i = 0 ; i < k ;i ++)
		{
			sum_Nk += Nk[i];
		}
		if(fabs(sum_Nk - sz ) > 1){
			CString str;
			str.Format("%lf %d,",sum_Nk,sz);
			AfxMessageBox(str);
		}
		//end

		//��E
		for(int i = 0 ; i < width ;i++)
		{
			for(int j = 0 ; j < height ;j++)
			{
				int pos = i*height + j ;
				for(int w = 0 ; w < k ; w++)
				{
					E[w][0] += lab[0][pos]*P[w][pos];
					E[w][1] += lab[1][pos]*P[w][pos];
					E[w][2] += lab[2][pos]*P[w][pos];
#ifdef LABEL5
					E[w][3] += i*P[w][pos];
					E[w][4] += j*P[w][pos];
#endif
				}
			}
		}
		for(int i=0;i<k;i++)
		{
			if(Nk[i] == 0)
				continue;
			for(int j = 0 ; j < LABEL_NUM;j++)
			{
				E[i][j] /=Nk[i];
			}
		}
		//��D
		for(int i = 0; i < sz; i++)
		{
			int x = i / height;
			int y = i % height;
			for(int j = 0; j < k ; j++)
			{
				sub1[0] = lab[0][i] - E[j][0];
				sub1[1] = lab[1][i] - E[j][1];
				sub1[2] = lab[2][i] - E[j][2];
#ifdef LABEL5
				sub1[3] = x - E[j][3];
				sub1[4] = y - E[j][4];
#endif
				mat.Mul(&sub1[0],&sub1[0],LABEL_NUM,1,LABEL_NUM,&sub2[0]);
				for(int w = 0 ; w < LABEL_NUM2; w++)
				{
					D[j][w]+= sub2[w]*P[j][i];
				}
			}
		}
		for(int i = 0; i < k ; i ++)
		{
			if(Nk[i] == 0)
				continue;
			for(int j = 0 ; j < LABEL_NUM2;j++)
			{
				D[i][j] /= Nk[i];
			}
		}
		//�������
		for(int i = 0 ; i < k ; i++)
		{
			Det[i] = mat.Det(&D[i][0]);
			if(Det[i] < 1e-10)
			{
				for(int ii = 0 ;ii < LABEL_NUM;ii++)
				{
					D[i][ii*LABEL_NUM + ii] += 1e-4;
				}
				Det[i] = mat.Det(&D[i][0]);
			}
			Det[i] = fabs(Det[i]);
			mat.Inverse(&D[i][0],&D_[i][0]);
		}
		//���������
		for(int i = 0 ; i < k ;i++)
		{
			Q[i] = Nk[i] / sz;
		}
		//�������
		for(int i = 0 ; i < sz ; i++)
		{
			int x = i / height;
			int y = i % height;
			for(int j = 0 ; j < k ; j++)
			{
				sub1[0] = lab[0][i] - E[j][0];
				sub1[1] = lab[1][i] - E[j][1];
				sub1[2] = lab[2][i] - E[j][2];
#ifdef LABEL5
				sub1[3] = x - E[j][3];
				sub1[4] = y - E[j][4];
#endif
				mat.Mul(&sub1[0],&D_[j][0],1,LABEL_NUM,LABEL_NUM,&sub2[0]);
				mat.Mul(&sub2[0],&sub1[0],1,LABEL_NUM,1,&ans);
				P[j][i] = Q[j] / pow_pi / sqrt(Det[j]) * exp(-0.5 * ans);
			}
		}
		//��һ��
		for(int i = 0 ; i < sz ; i ++)
		{
			double sum = 0 ;
			for(int j = 0 ; j < k ; j++)
			{
				sum+=P[j][i];
			}
			for(int j = 0; j < k ;j++)
			{
				P[j][i] /= sum;
			}
		}
	}
	for(int i = 0 ; i < sz ;i ++)
	{
		int c = 0;
		double MaxPi = P[0][i];
		for(int j = 1 ; j < k ;j ++)
		{
			if(P[j][i] > MaxPi)
			{
				MaxPi = P[j][i];
				c = j;
			}
		}
		fenClass_c[i] = c;
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
	int MaxTime = 30;
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

void KMeans_Core(const vector<vector<float>> &lab,
				vector<int> &fenClass_c,
				int width,
				int height,
				int &k,
				double M)
{
	int sz = width * height;
	int MaxTime = 30;
	vector<Tag> k_tags;
	fenClass_c = vector<int>(sz);
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
				int pos = i * height + j;
				for(int w=0;w<k;w++)
				{
					double subr = lab[0][pos]-k_tags[w].r;
					double subg = lab[1][pos]-k_tags[w].g;
					double subb = lab[2][pos]-k_tags[w].b;
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
				int pos = posX * height + posY;
				r+=lab[0][pos];
				g+=lab[1][pos];
				b+=lab[2][pos];
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
			fenClass_c[vec[i][j].x* height +vec[i][j].y] = i; 
		}
	}
}

void KMeans_Core_(const vector<vector<float>> &lab,
				vector<int> &fenClass_c,
				int width,
				int height,
				int &k,double M)

{
	int sz = width * height;
	int S = sqrt(1.0 * sz / k)+0.5;
	int MaxTime = S;
	vector<vector<double>> k_tags(5,vector<double>(k));
	vector<int>c_num(k);
	while(MaxTime --)
	{
		for(int i = 0 ; i < 5 ; i++)
		{
			ZeroMemory(&k_tags[i][0],sizeof(double)*k);
		}
		ZeroMemory(&c_num[0],sizeof(int)*k);
		for(int i = 0 ;i < width ;i ++)
		{
			for(int j = 0 ; j < height ; j++)
			{
				int pos = i * height + j;
				int c = fenClass_c[pos];
				k_tags[0][c]+= lab[0][pos];
				k_tags[1][c]+= lab[1][pos];
				k_tags[2][c]+= lab[2][pos];
				k_tags[3][c]+= i;
				k_tags[4][c]+= j;
				c_num[c]++;
			}
		}
		for(int i = 0 ; i < k ;i ++)
		{
			if(c_num[i] == 0)
				continue;
			k_tags[0][i]/=c_num[i];
			k_tags[1][i]/=c_num[i];
			k_tags[2][i]/=c_num[i];
			k_tags[3][i]/=c_num[i];
			k_tags[4][i]/=c_num[i];
		}
		double sub[5]={0};
		for(int i = 0;i<width ; i++)
		{
			for(int j = 0 ; j < height ; j++)
			{
				int pos = i * height + j;
				double MinDis = INFF;
				int c=0;
				for(int w = 0 ; w < k ; w ++)
				{
					sub[0] = lab[0][pos] - k_tags[0][w];
					sub[1] = lab[1][pos] - k_tags[1][w];
					sub[2] = lab[2][pos] - k_tags[2][w];
					sub[3] = i - k_tags[3][w];
					sub[4] = j - k_tags[4][w];
					double dis = sqrt(sub[0]*sub[0] + sub[1]*sub[1] + sub[2]*sub[2] + M*sub[3]*sub[3]+ M*sub[4]*sub[4]);
					if(dis < MinDis)
					{
						MinDis = dis;
						c = w;
					}
				}
				fenClass_c[pos] = c;
			}
		}
	}
}
void ICM_Core(const vector<vector<float>> &lab,
				vector<int> &fenClass_c,
				int width,
				int height,
				int &k)
{
	double bate = 1.5;
	int sz = width * height;
	int MaxTime = 100;
	int S = sqrt(sz / k)+0.5;
	double powpi = pow(2*Pi,LABEL_NUM/2.0);
	vector<vector<double>> E(k,vector<double>(LABEL_NUM,0));
	vector<vector<double>> D(k,vector<double>(LABEL_NUM2,0));
	vector<vector<double>> D_(k,vector<double>(LABEL_NUM2)); 
	vector<int> c_num(k,0);
	vector<double>e(LABEL_NUM);
	vector<double>t(LABEL_NUM);
	vector<double>d(LABEL_NUM2);
	vector<int> fenClass_t(sz);
	vector<double> ans(1);
	
	MatrixQuick mat(LABEL_NUM);
	int Max_Time = S*2;
	if(Max_Time > MaxTime)
		Max_Time = MaxTime;
	int Move8[][2] = {0,1,0,-1,1,0,-1,0,1,1,1,-1,-1,1,-1,-1};
	vector<int> num(k,0);
	vector<double> P(k);
	vector<double> tP(k);
	vector<double> dv(k);
	vector<double> sub(LABEL_NUM);
	vector<vector<double>> endPi(k,vector<double>(sz));
	vector<double>sumPi(sz,0);
	int s_8[8];
	int spos = 0;
	while(Max_Time -- )
	{
		//�Ƚ������ص����c_num������E������D��ֵ��0
		ZeroMemory(&c_num[0],sizeof(int)*k);
		for(int i = 0 ; i < k ; i ++)
		{
			ZeroMemory(&E[i][0],sizeof(double)*LABEL_NUM);
			ZeroMemory(&D[i][0],sizeof(double)*LABEL_NUM2);
		}
		//��ÿ��������
		for(int i = 0 ; i < width ; i ++)
		{
			for(int j = 0 ; j < height ; j++)
			{
				int pos = i * height + j;
				int c = fenClass_c[pos];
				c_num[c] ++;
				E[c][0] += lab[0][pos];
				E[c][1] += lab[1][pos];
				E[c][2] += lab[2][pos];
		#ifdef LABEL5
				E[c][3] += i;
				E[c][4] += j;
		#endif
				
			}
		}
		//���ֵ��������
		for(int i = 0 ; i < k ; i ++)
		{
			if(c_num[i] == 0)
				continue;
			E[i][0] /= c_num[i];
			E[i][1] /= c_num[i];
			E[i][2] /= c_num[i];
	#ifdef LABEL5
			E[i][3] /= c_num[i];
			E[i][4] /= c_num[i];
	#endif
		}

		//��Э�������ĺ�
		for(int i = 0 ; i < width ; i ++)
		{
			for(int j = 0 ; j < height ; j++)
			{
				int pos = i * height + j;
				int c = fenClass_c[pos];
				sub[0] = lab[0][pos]	-	E[c][0];
				sub[1] = lab[1][pos]	-	E[c][1];
				sub[2] = lab[2][pos]	-	E[c][2];
	#ifdef LABEL5
				sub[3] = i			-	E[c][3];
				sub[4] = j			-	E[c][4];
	#endif
				mat.Mul(&sub[0],&sub[0],LABEL_NUM,1,LABEL_NUM,&d[0]);
				for(int w = 0 ; w < LABEL_NUM2 ;w ++)
				{
					D[c][w] += d[w];
				}
			}
		}
		//���ֵ��
		for(int i = 0 ; i < k ; i ++)
		{
			if(c_num[i] == 0)
				continue;
			for(int w = 0 ; w < LABEL_NUM2; w++)
			{
				D[i][w] /= c_num[i];
			}
		}
		memcpy_s(&fenClass_t[0],sizeof(int) * sz,&fenClass_c[0],sizeof(int) * sz);
		for(int i =0 ; i < k ; i ++)
		{
			if(c_num[i] == 0)
			{
				continue;
			}
			dv[i] = fabs(mat.Det(&D[i][0]));
			if(dv[i] < 1e-20)
			{
				for(int ii=0;ii<LABEL_NUM;ii++)
				{
					D[i][ii*LABEL_NUM + ii]+=1e-4;
				}
				dv[i] = fabs(mat.Det(&D[i][0]));
			
			}
			mat.Inverse(&D[i][0],&D_[i][0]);
		}
		for(int i = 0 ; i < k ; i++)
		{
			if(c_num[i] == 0)
				continue;
			dv[i] = sqrt(dv[i]);
		}
		for(int i = 0 ; i < width ; i ++)
		{
			for(int j = 0 ; j < height ; j++)
			{
				int pos = i * height + j;
				int c = fenClass_t[pos];
				if(c<0 || c>=k)
				{
					continue;
				}
				//�洢pos��Χ�İ����������
				for(int w = 0 ; w < 8 ; w++)
				{
					int nx = i + Move8[w][0];
					int ny = j + Move8[w][1];
					
					if(nx < 0 || ny < 0 || nx >=width || ny >= height)
						continue;
					int npos = nx * height + ny;
					int nc = fenClass_t[npos];
					if(num[nc] == 0)
					{
						s_8[spos] = nc;
						spos ++;
					}
					num[nc] ++;
				}
				
				int tc = c;
				
				//double z = 0;
				for(int w = 0 ; w < spos ; w ++)
				{
					int tnum = num[s_8[w]];
					double b = tnum*bate;
					P[s_8[w]] = exp(b);
					//z += tP[s_8[w]];
				}
				double other=1;
				/*z+= k - spos;
				other = 1 / z;
				for(int w = 0 ; w < spos ; w ++)
				{
					P[s_8[w]] = tP[s_8[w]] / z;
				}*/
				
				double max_pi = -INFI;
				for(int w = 0 ; w < k ; w ++)
				{
					if(c_num[w] == 0)
						continue;
					double preP = other;
					if(num[w])
						preP = P[w];
					sub[0] = lab[0][pos] - E[w][0];
					sub[1] = lab[1][pos] - E[w][1];
					sub[2] = lab[2][pos] - E[w][2];
#ifdef LABEL5
					sub[3] = i - E[w][3];
					sub[4] = j - E[w][4];
#endif

					mat.Mul(&sub[0],&D_[w][0],1,LABEL_NUM,LABEL_NUM,&t[0]);
					mat.Mul(&t[0],&sub[0],1,LABEL_NUM,1,&ans[0]);
					double pi = ( preP / dv[w] * exp(-0.5 * ans[0]));
					if(pi > max_pi)
					{
						tc = w;
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
/*
�裺
E[i]:��i��ľ�ֵ
D[i]:��i��ķ���
P[i][j]:��j����i�ĸ���
Q[i][j]:���ڵ�j���㣬��i����������
Nk[i]:��i��ĵ���
S[i]����i��������ռ�
sz:���ص��ܸ���
x[i]:��i��������
Num[i][j]:��i��������j��ĸ���
�У�
E[k] = 1/Nk[k] * sum(x[i]��s[k])
D[k] = 1/Nk[k] * sum( (x[i] - E[k])^T * (x[i] - E[k]))
Q[k][j] = e^(Num[i][k]*��) / sum(e^(Num[j][k]*��))
P[k][j] = Q[j][k] * N(x[j],E[k],D[k])
*/

void ICM_Core_(const vector<vector<float>> &lab,
				vector<int> &fenClass_c,
				int width,
				int height,
				int &k)
{

	vector<int>color(k);
	RGBQUAD colorTab[256];
	for(int i =0 ; i < 256 ; i++)
	colorTab[i].rgbBlue = colorTab[i].rgbGreen = colorTab[i].rgbRed = i;
	CImage img;
	img.Create(width,height,8);
	img.SetColorTable(0,256,colorTab);
	byte *blt = (byte *)img.GetBits();
	int plt = img.GetPitch();
	int path_num=0;
	//
	int sz = width * height;
	int MaxTime = 100;
	vector<vector<double>> E(k,vector<double>(LABEL_NUM));
	vector<vector<double>> D(k,vector<double>(LABEL_NUM2));
	vector<vector<double>> D_(k,vector<double>(LABEL_NUM2));
	vector<double> det(k);
	vector<double> D2(LABEL_NUM2);
	vector<double> D1(LABEL_NUM);
	vector<double> Dt(LABEL_NUM);
	vector<int> Nk(k);
	MatrixQuick mat(LABEL_NUM);
	vector<int>fenClass_t(sz);
	int S = sqrt(1.0 * sz / k) + 0.5;
	int Max_Time = S;
	if(Max_Time > MaxTime)
		Max_Time = MaxTime;
	int Move[][2] = {0,1,0,-1,1,0,-1,0,1,1,1,-1,-1,1,-1,-1};
	int S8[8];
	vector<int>c_num(k,0);
	vector<double>Q(k);
	vector<double>Ans(1);
	const double bate = 1.5;
	srand(time(NULL));
	for(int i = 0 ; i < k;i++)
	{
		color[i] = rand() % 255;
	}
	while(Max_Time --)
	{
		ZeroMemory(&(Nk[0]),sizeof(int) * k);
		for(int i = 0 ; i < k ; i ++)
		{
			ZeroMemory(&(E[i][0]),sizeof(double) * LABEL_NUM);
			ZeroMemory(&(D[i][0]),sizeof(double) * LABEL_NUM2);
		}
		memcpy_s(&fenClass_t[0],sizeof(int)*sz,&fenClass_c[0],sizeof(int)*sz);
		//������
		for(int i = 0 ; i < sz ; i ++)
		{
			int c = fenClass_c[i];
			E[c][0] += lab[0][i];
			E[c][1] += lab[1][i];
			E[c][2] += lab[2][i];
			Nk[c] ++;
#ifdef LABEL5
			E[c][3] += i / height;
			E[c][4] += i % height;
#endif

		}
		for(int i = 0 ; i < k ; i ++)
		{
			if(Nk[i] == 0)
				continue;
			E[i][0] /= Nk[i];
			E[i][1] /= Nk[i];
			E[i][2] /= Nk[i];
#ifdef LABEL5
			E[i][3] /= Nk[i];
			E[i][4] /= Nk[i];
#endif
		}
		//����������
		//�󷽲�
		for(int i = 0 ; i < sz ; i ++)
		{
			int c = fenClass_c[i];
			D1[0] = lab[0][i] - E[c][0];
			D1[1] = lab[1][i] - E[c][1];
			D1[2] = lab[2][i] - E[c][2];
#ifdef LABEL5
			D1[3] = i / height - E[c][3];
			D1[4] = i % height - E[c][4];
#endif
			mat.Mul(&D1[0],&D1[0],LABEL_NUM,1,LABEL_NUM,&D2[0]);
			for(int j = 0 ; j < LABEL_NUM2 ; j++)
			{
				D[c][j] += D2[j];
			}
		}
		for(int i = 0 ; i < k ; i ++)
		{
			if(Nk[i] == 0)
				continue;
			for(int j = 0 ; j < LABEL_NUM2 ; j++)
			{
				D[i][j] /= Nk[i];
			}
		}
		//�󷽲����
		//�󷽲�������
		for(int i = 0 ; i < k ;i ++)
		{
			if(Nk[i] == 0)
				continue;
			det[i] = fabs(mat.Det(&D[i][0]));
			if(det[i] < 1e-20)
			{
				for(int j = 0 ; j < LABEL_NUM; j ++)
				{
					D[i][j*LABEL_NUM + j] += 1e-4;
				}
				det[i] = fabs(mat.Det(&D[i][0]));
			}
			mat.Inverse(&D[i][0],&D_[i][0]);
		}
		for(int t = 0 ; t < k ; t ++)
		{
			if(Nk[t] == 0)
				continue;
			det[t] = sqrt(det[t]);
		}
		//������������
		//�������ʲ����»�����
		for(int i = 0 ; i < width ; i++)
		{
			for(int j = 0 ; j < height ;j++)
			{
				int pos = i * height + j;
			//���������
				int cou = 0;
				//ͳ�ư�����ÿ�������Ŀ
				for(int w = 0 ; w < 8 ; w++)
				{
					int nx = i + Move[w][0];
					int ny = j + Move[w][1];
					
					if(nx < 0 || ny < 0 || nx >= width || ny >= height)
						continue;
					int npos = i * height + j;
					int c = fenClass_t[npos];
					if(c_num[c] == 0)
					{
						S8[cou] = c;
						cou ++;
					}
					c_num[c] ++;
				}
				//���������
				//double z = 0;
				
				for(int t = 0 ; t < cou ; t++)
				{
					int c = S8[t];
					int num = c_num[c];
					Q[c] = exp(num * bate);
					//z += Q[c];
				}
				double other = 1;
				/*z += k - cou;
				other = 1 / z;
				for(int t = 0 ; t < cou ;t++)
				{
					int c = S8[t]; 
					Q[c] /= z;
				}*/
				/*for(int t = 0 ; t < k ; t ++)
				{
					Q[t] = exp(c_num[t]*bate);
				}*/
				//��������
				int tc = fenClass_t[pos];
				double MaxPi = -1;
				int ansC = tc;
				double P;
				double ei ;
				double de ;
				double pi;
				
				for(int t = 0 ; t < k ;t ++)
				{
					if(Nk[t] == 0)
						continue;
					P = other;
					if(c_num[t])
						P = Q[t];
					D1[0] = lab[0][pos] - E[t][0];
					D1[1] = lab[1][pos] - E[t][1];
					D1[2] = lab[2][pos] - E[t][2];
#ifdef LABEL5
					D1[3] = i - E[t][3];
					D1[4] = j - E[t][4];
#endif
					
					mat.Mul(&D1[0],&D_[t][0],1,LABEL_NUM,LABEL_NUM,&Dt[0]);
					mat.Mul(&Dt[0],&D1[0],1,LABEL_NUM,1,&Ans[0]);
					ei = exp(-0.5*Ans[0]);
					de = det[t];
					pi = P / de * ei;
					if(pi > MaxPi)
					{
						ansC = t;
						MaxPi = pi;
					}			
				}
				
				fenClass_c[pos] = ansC;
				
				for(int t = 0; t < cou;t++)
				{
					c_num[S8[t]] = 0;
				}
			}
		}
		//�������ʽ���
		for(int i = 0 ; i < width ; i ++)
		{
			for(int j = 0 ; j < height ; j++)
			{
				int pos = i * height + j;
				*(blt + j*plt + i) = color[fenClass_c[pos]];
			}
		}
		CString str;
		str.Format("img/%d.jpg",path_num++);
		img.Save(str);
	}
}

void ICM_CoreUseEigen(const vector<vector<float>> &lab,
				vector<int> &fenClass_c,
				int width,
				int height,
				int &k)
{
	double T = 1.5;
	double bate = 1;
	int sz = width * height;
	int S = sqrt(sz / k)+0.5;
	double powpi = pow(2*Pi,LABEL_NUM);
	vector<VectorXd> E(k,VectorXd(LABEL_NUM));

	vector<MatrixXd> D(k,MatrixXd(LABEL_NUM,LABEL_NUM));
	vector<MatrixXd> D_(k,MatrixXd(LABEL_NUM,LABEL_NUM));
	vector<int> c_num(k,0);
	
	vector<int> fenClass_t(sz);
	int Max_Time = 10;
	int Move8[][2] = {0,1,0,-1,1,0,-1,0,1,1,1,-1,-1,1,-1,-1};
	double rate = 0.0001;
	vector<int> num(k,0);
	vector<double> P(k);
	vector<double> dv(k);
	VectorXd sub(LABEL_NUM);
	MatrixXd tmpMat(LABEL_NUM,LABEL_NUM);
	MatrixXd e(LABEL_NUM,LABEL_NUM);
#ifdef LABEL5
	e<< 1,0,0,0,0,
		0,1,0,0,0,
		0,0,1,0,0,
		0,0,0,1,0,
		0,0,0,0,1;
#endif
#ifdef LABEL3
	e<< 1,0,0,
		0,1,0,
		0,0,1;
#endif
	e*=rate;
	int s_8[N];
	int spos = 0;
	while(Max_Time -- )
	{
		//��ʼ���������������
		for(int i = 0 ; i < k ; i ++)
		{
			for(int j = 0 ; j < LABEL_NUM;j++)
			{
				E[i](j)=0;
			}
			for(int j = 0; j < LABEL_NUM;j++)
			{
				for(int w = 0; w < LABEL_NUM;w++)
				D[i](j,w)=0;
			}
			c_num[i]=0;
		}
		//������
		for(int i = 0 ; i < width ; i ++)
		{
			for(int j = 0 ; j < height ; j++)
			{
				int pos = i * height + j;
				int c = fenClass_c[pos];
				E[c](0)+= lab[0][pos];
				E[c](1) += lab[1][pos];
				E[c](2) += lab[2][pos];
	#ifdef LABEL5
				E[c](3) += i;
				E[c](4) += j;
	#endif
				c_num[c] ++;
			}
		}
		
		for(int i = 0 ; i < k ; i ++)
		{
			if(c_num[i] == 0)
				continue;
			E[i](0) /= c_num[i];
			E[i](1) /= c_num[i];
			E[i](2) /= c_num[i];
	#ifdef LABEL5
			E[i](3) /= c_num[i];
			E[i](4) /= c_num[i];
	#endif
		}

		//�󷽲�
		for(int i = 0 ; i < width ; i ++)
		{
			for(int j = 0 ; j < height ; j++)
			{
				int pos = i * height + j;
				int c = fenClass_c[pos];
				sub[0] = lab[0][pos]	-	E[c][0];
				sub[1] = lab[1][pos]	-	E[c][1];
				sub[2] = lab[2][pos]	-	E[c][2];
	#ifdef LABEL5
				sub[3] = i			-	E[c][3];
				sub[4] = j			-	E[c][4];
	#endif
				D[c]+= sub *  sub.transpose();

			}
		}
		for(int i = 0 ; i < k ; i ++)
		{
			if(c_num[i] == 0)
				continue;
			D[i]/=c_num[i];
		}
		for(int i = 0 ; i < sz ; i++){
			fenClass_t[i] = fenClass_c[i];		
		}	
		for(int i =0 ; i < k ; i ++)
		{
			
			if(c_num[i] == 0)
			{
				continue;
			}
			//CString str;
			/*str.Format("D=[%.5lf,%.5lf,%.5lf][%.5lf,%.5lf,%.5lf][%.5lf,%.5lf,%.5lf]",D[i](0,0),D[i](0,1),D[i](0,2),
																					 D[i](1,0),D[i](1,1),D[i](1,2),
																					 D[i](2,0),D[i](2,1),D[i](2,2));*/
			//AfxMessageBox(str);
			dv[i] = abs(D[i].determinant());
			if(dv[i] < 1e-8){	
				D[i]+=e;
			}
			D_[i] = D[i].inverse();
			dv[i] = abs(D[i].determinant());
			
		}

		//���»�����
		for(int i = 0 ; i < width ; i ++)
		{
			for(int j = 0 ; j < height ; j++)
			{
				
				int pos = i * height + j;
				int c = fenClass_t[pos];
				if(c<0 || c>=k)
				{
					continue;
				}
				//�洢pos��Χ�İ����������
				for(int w = 0 ; w < 8 ; w++)
				{
					int nx = i + Move8[w][0];
					int ny = j + Move8[w][1];
					
					if(nx < 0 || ny < 0 || nx >=width || ny >= height)
						continue;
					int npos = nx * height + ny;
					int nc = fenClass_t[npos];
					if(num[nc] == 0)
					{
						s_8[spos] = nc;
						spos ++;
					}
					num[nc] ++;
				}
				double z = 0;
				for(int w = 0 ; w < spos ; w ++)
				{
					int tnum = num[s_8[w]];
					double b = ( 8 - tnum - tnum)*bate;
					P[s_8[w]] = exp(-1.0 * b / T);
					z += P[s_8[w]];
				}
				for(int w = 0 ; w < spos ; w ++)
				{
					P[s_8[w]] /= z;
				}
				int tc = c;
				double max_pi = -INFI;
				for(int w = 0 ; w < spos ; w ++)
				{
					int c = s_8[w];
					
					if(dv[c] == 0)
					{
						continue;
					}
					sub[0] = lab[0][pos] - E[c][0];
					sub[1] = lab[1][pos] - E[c][1];
					sub[2] = lab[2][pos] - E[c][2];
#ifdef LABEL5
					sub[3] = i - E[c][3];
					sub[4] = j - E[c][4];
#endif
					/*CString str;
					str.Format("sub.t= %.5lf  %.5lf  %.5lf  D_=%.5lf  %.5lf  %.5lf %.5lf  %.5lf  %.5lf  %.5lf  %.5lf  %.5lf sub=%.5lf  %.5lf  %.5lf ",
						sub.transpose()[0],sub.transpose()[1],sub.transpose()[2],
						D_[c](0,0),D_[c](0,1),D_[c](0,2),
						D_[c](1,0),D_[c](1,1),D_[c](1,2),
						D_[c](2,0),D_[c](2,1),D_[c](2,2),
						sub[0],sub[1],sub[2]);
					AfxMessageBox(str);*/
					double ans = exp( -0.5 * (sub.transpose() * D_[c]* sub)(0,0) );
					double pi = (P[c] / powpi / sqrt(dv[c]) *  ans);  
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

void delete_empty_calss(vector<int> &fenClass_c,int width,int height,int &k)
{
	int sz = width * height;
	vector<bool> is_visit(sz,false);
	vector<int> Que(sz);
	int Qsz;
	int Qpos;
	int Move8[][2] = {0,1,0,-1,1,0,-1,0,1,1,1,-1,-1,1,-1,-1};
	vector<int> fenClass_t = fenClass_c;
	int nc = 0;
	for(int i = 0 ; i < sz ; i++)
	{
		if(is_visit[i])
			continue;
		Qsz = 1;
		Qpos = 0;
		Que[0]=i;
		int c = fenClass_t[i];
		is_visit[i] = true;
		while(Qpos < Qsz)
		{
			int pos = Que[Qpos];
			Qpos++;
			fenClass_c[pos] = nc;
			int x = pos / height;
			int y = pos % height;
			for(int j = 0 ; j < 8;j++)
			{
				int nx = x + Move8[j][0];
				int ny = y + Move8[j][1];
				if(nx < 0 || ny < 0 || nx >= width || ny >= height)
				{
					continue;
				}
				int npos = nx * height + ny;
				if(is_visit[npos] || fenClass_t[npos] != c)
					continue;
				is_visit[npos] = true;
				Que[Qsz] = npos;
				Qsz ++;
			}
		}
		nc++;
	}
	k = nc;
}
/*
w*h*k=W*H
w/W=h/H
w=h*W/H
h^2*k*W/H=W*H
h^2=H^2/k
*/
void n_avg_2_k_(vector<int> &fenClass_c,int width,int height,int &k)
{
	int sz = width * height;
	double rate = sqrt(1.0/k);
	int sw = width * rate;
	int sh = height * rate;
	int num_x = width / sw;
	int num_y = height / sh;
	k = num_x * num_y;
	fenClass_c = vector<int>(sz);
	for(int i=0;i<width;i++)
	{
		for(int j =0 ; j < height;j++)
		{
			int w = i/sw;
			int h = j/sh;
			if(w >= num_x)
				w = num_x -1;
			if(h >= num_y)
				h = num_y -1;
			fenClass_c[i*height+j]=w*num_y+h;
		}
	}
}
void n_avg_2_k(vector<int> &fenClass_c,int width,int height,int &k)
{//ת��1-k��
	int sz = width * height;
	int S = sqrt(sz / k) + 0.5;
	int swidth	= width		*	sqrt(1.0/k) + 0.5;
	int sheight = height	*	sqrt(1.0/k) + 0.5;
	int num_x = width / swidth + (width%swidth>0);
	int num_y = height / sheight + (height%sheight>0);
	k = num_x * num_y;
	fenClass_c = vector<int>(sz);
	for(int i = 0 ; i < num_x ; i++)
	{
		for(int j = 0 ; j < num_y ;j++)
		{
			int c = i * num_y + j;
			int sx = i * swidth;
			int sy = j * sheight;
			for(int ii = 0 ; ii < swidth ; ii++)
			{
				for(int jj =0; jj < sheight ; jj++)
				{
					int nx = sx + ii;
					int ny = sy + jj;
					if(nx < 0 || nx >= width || ny<0 || ny >= height)
						continue;
					int pos = nx * height + ny;
					fenClass_c[pos] = c;
				}
			}
		}
	}
	
}
void init_n_2_k(vector<int> &fenClass_c,int width,int height,int &k)
{
	int sz = width * height;
	fenClass_c = vector<int>(sz);
	int S = sqrt(1.0 * sz / k)+0.5;
	int wNum = width / S + ((width%S)?1:0);
	int hNum = height / S+((height%S)?1:0);
	k = wNum * hNum;
	for(int i = 0 ;i <width ; i ++)
	{
		for(int j = 0 ; j < height ; j++)
		{
			int row = i / S;
			int col = j / S;
			int c = row * hNum + col;
			int pos = i *height +j;
			fenClass_c[pos] = c;
		}
	}
}
void LogDebug(vector<vector<double>>&pi,int width,int height,int k,CString name,int t)
{
	
 	int sz = width * height;
	vector<CString> names(k);
	vector<FILE*> files(k);
	char buf1[20];
	char buf2[20];
	_itoa_s(t,buf1,10);
	
	for(int i =0 ; i < k ; i++)
	{
		_itoa_s(i,buf2,10);
		names[i]=name+CString("_")+CString(buf1)+CString("_")+CString(buf2);
		fopen_s(&files[i],names[i].GetBuffer(names[i].GetLength()),"w");
	}
	for(int pos = 0 ; pos < sz ; pos ++)
	{
		for(int w = 0 ; w < k ; w ++)
		{
			fprintf_s(files[w],"%10.2lf",pi[w][pos]);
			if((pos + 1) % width == 0)
				fprintf_s(files[w],"\n");
		}
	}
	for(int i =0 ; i < k;i++)
	{
		fclose(files[i]);
	}
}

void DrawBorderWidthAvgColor(CImage &des,vector<int> &fenClass_c,int width,int height)
{
	int sz = width * height;
	byte * blt = (byte *)des.GetBits();
	int plt = des.GetPitch();
	vector<bool> is_visit(sz,0);
	queue<CPoint> QUE;
	queue<CPoint> points;

#define PUSHQUE(x,y) QUE.push(CPoint(x,y));points.push(CPoint(x,y));\
	r += *(blt + plt*y+ x*3);\
	g += *(blt + plt*y+ x*3 + 1);\
	b += *(blt + plt*y+ x*3 + 2);
	//CString str;str.Format("pt=%d,(%d,%d)",plt,x,y);logs.Debug(str.GetBuffer(str.GetLength()));

#define SETCOLOR(x,y)\
	*(blt + plt*y+ x*3) = r;\
	*(blt + plt*y+ x*3 + 1) = g;\
	*(blt + plt*y+ x*3 + 2) = b;
	int Move[][2]={0,1,0,-1,1,0,-1,0,1,1,1,-1,-1,1,-1,-1};
	double r,g,b;
	for(int i = 0; i < sz ; i ++)
	{
		if(is_visit[i])
			continue;
		r=0;b=0;g=0;
		int x = i/height;
		int y = i%height;
		PUSHQUE(x,y)
		int c = fenClass_c[i];
		is_visit[i] = 1;
		while(!QUE.empty())
		{
			CPoint p = QUE.front();
			QUE.pop();
			for(int j = 0 ; j < 8;j ++)
			{
				int nx  = p.x + Move[j][0];
				int ny  = p.y + Move[j][1];
				int npos = nx* height + ny;
				if(nx < 0 || nx >= width || ny < 0 || ny >= height || is_visit[npos] || c != fenClass_c[npos])
					continue;
				PUSHQUE(nx,ny) 
				is_visit[npos] = 1;
			}
		}
		r/=points.size();
		g/=points.size();
		b/=points.size();
		while(!points.empty())
		{
			CPoint p = points.front();
			points.pop();
			SETCOLOR(p.x,p.y);
		}
	}
}

void LogDebug(string str)
{
	logs.Debug(str);
}

void SaveImg(vector<int> & fenClass_c,CImage &img,int width,int height,CString path)
{
	static int num = 1;
	CString relPath;
	DrawBorder1(img,fenClass_c,width,height);
	
	CString str;
	str.Format("%d",num);
	relPath = path+str+".jpg";
	
	img.Save(relPath.GetBuffer(relPath.GetLength()));
	num++;
}

bool ImageCopy(const CImage &srcImage, CImage &destImage)
{
    int i;//ѭ������
    if (srcImage.IsNull())
        return FALSE;
    //Դͼ�����
    BYTE* srcPtr = (BYTE*)srcImage.GetBits();
    int srcBitsCount = srcImage.GetBPP();
    int srcWidth = srcImage.GetWidth();
    int srcHeight = srcImage.GetHeight();
    int srcPitch = srcImage.GetPitch();
    //����ԭ��ͼ��
    if (!destImage.IsNull())
    {
        destImage.Destroy();
    }
    //����CImage����ͼ�񲢷����ڴ�
    if (srcBitsCount == 32)   //֧��alphaͨ��
    {
        destImage.Create(srcWidth, srcHeight, srcBitsCount, 1);
    }
    else
    {
        destImage.Create(srcWidth, srcHeight, srcBitsCount, 0);
    }
    //���ص�ɫ��
    if (srcBitsCount <= 8 && srcImage.IsIndexed())//��Ҫ��ɫ��
    {
        RGBQUAD pal[256];
        int nColors = srcImage.GetMaxColorTableEntries();
        if (nColors>0)
        {
            srcImage.GetColorTable(0, nColors, pal);
            destImage.SetColorTable(0, nColors, pal);//���Ƶ�ɫ�����
        }
    }
    //Ŀ��ͼ�����
    BYTE *destPtr = (BYTE*)destImage.GetBits();
    int destPitch = destImage.GetPitch();
    //����ͼ������
    for (i = 0; i<srcHeight; i++)
    {
        memcpy(destPtr + i*destPitch, srcPtr + i*srcPitch, abs(srcPitch));
    }

    return TRUE;
}
int ReadInt(FILE *&file,int &val){
	return fread_s(&val,sizeof(val),sizeof(val),1,file);
}
int ReadDouble(FILE *&file,double &val)
{
	return fread_s(&val,sizeof(val),sizeof(val),1,file);
}
int ReadString(FILE *&file,char *buf,int bufLen)
{
	int len;
	ReadInt(file,len);
	return fread_s(buf,bufLen,len,1,file);
}
int WriteInt(FILE *&file,int &val){
	return fwrite(&val,sizeof(val),1,file);
}
int WriteDouble(FILE *&file,double &val)
{
	return fwrite(&val,sizeof(val),1,file);
}
int WriteString(FILE *&file,char *buf)
{
	int len = strlen(buf)+1;
	WriteInt(file,len);
	return fwrite(buf,len,1,file);
}
void init_rand_2_k(vector<int> &fenClass_c,int width,int height,int &k)
{
	int sz = width * height;
	fenClass_c = vector<int>(sz);
	srand(time(NULL));
	for(int i =0;i<sz;i++)
	{
		fenClass_c[i] = rand()%k;
	}
}
