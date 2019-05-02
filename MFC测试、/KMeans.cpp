#include "stdafx.h"
using namespace std;
#define MAXMIN	30
//#define FENKUAI
#define TXT
//#define ONLYCOLOR
#define RANDCOLOR
#define FAZHI 400
#define CUTMIN
#define ONLYONE
struct Sign
{
	double r;
	double g;
	double b;
	int x;
	int y;
	Sign(double r,double g,double b,int x,int y)
	{
		this->r=r;
		this->g=g;
		this->b=b;
		this->x=x;
		this->y=y;
	}
	Sign(Sign &sign)
	{
		this->r=sign.r;
		this->g=sign.g;
		this->b=sign.b;
		this->x=sign.x;
		this->y=sign.y;
	}
	Sign(){}
};
template<class T>
T **newMem(int width,int height)
{
	T **p=NULL;
	p = new T*[width];
	for(int i=0;i<width;i++)
		p[i] = new T[height];
	return p;
}
template<class T>
void deleteMem(T **p,int width)
{
	for(int i=0;i<width;i++)
	{
		delete []p[i];
	}
	delete []p;
}
void KMeans(CImage &res,CImage &des,int k,double rate,int step,double sublim0)
{
	//res.GetBitmapBits();
	des  = res;
	//获取宽和高
	int width = res.GetWidth();
	int height = res.GetHeight();
	//开辟存储lab像素空间
	float *** lab = new float**[width];
	for(int i=0;i<width;i++)
	{
		lab[i] = new float*[height];
	}
	
	//转换
	for(int i=0;i<width;i++)
	{
		for(int j=0;j<height;j++)
		{
			byte * colorRes = (byte *)res.GetPixelAddress(i,j);
			float * color = new float[3];
			RGB2LAB(colorRes[0],colorRes[1],colorRes[2],color[0],color[1],color[2]);
			lab[i][j] = color;
		}
	}
	
	//
	int num = (int)(sqrt((double)k)+0.5);
	k = num*num;
	int widthStep = width / num;
	int heightStep = height / num;
	int rwidth = widthStep*step;
	int rheight = heightStep*step;
	int rad = 4*k;
	int MinPixelNum = width * height / rad;
	int MaxDisX = (int)sqrt(1/rad)*width;
	int MaxDisY = (int)sqrt(1/rad)*height;
	vector<CPoint> *vec = new vector<CPoint>[k];
	Sign* cla = new Sign[k];
	Sign* pre = new Sign[k];
	CPoint **k2CPoint = new CPoint*[k];
	vector<int>numCPoint(k);
	//初始化
	int dotNum = rwidth*rheight;
	for(int i=0;i<num;i++)
	{
		for(int j=0;j<num;j++)
		{
			int startX = i*widthStep;
			int startY = j *heightStep;
			double r=0,g=0,b=0;
			int x=0,y=0;
			for(int w = startX;w<startX + rwidth && w <width;w++)
			{
				for(int v = startY;v<startY + rheight && v <height;v++)
				{
					r+=lab[w][v][0];
					g+=lab[w][v][1];
					b+=lab[w][v][2];
					x+=w;
					y+=v;
				}
			}	
			r/=dotNum;
			g/=dotNum;
			b/=dotNum;
			x/=dotNum;
			y/=dotNum;
			int cou = i*num+j;
			cla[cou] = Sign(r,g,b,x,y);
			k2CPoint[cou] = new CPoint[2];
			k2CPoint[cou][0] = CPoint(startX,startY );
			k2CPoint[cou][1] = CPoint(startX + rwidth,startY + rheight);
		}
	}

	//开始聚类

	bool isNotOk =true;
	int MaxTime = 20;
	while(isNotOk && MaxTime --)
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
#ifdef FENKUAI 
					if(i<cla[w].x - rwidth/2 || i>cla[w].x +rwidth/2 || j<cla[w].y - rheight/2 || j>cla[w].y + rheight/2)
						continue;
#endif
					double subr = lab[i][j][0]-cla[w].r;
					double subg = lab[i][j][1]-cla[w].g;
					double subb = lab[i][j][2]-cla[w].b;
					int subx = i-cla[w].x;
					int suby = j-cla[w].y;
					double nowSub = (subr*subr + subg*subg + subb*subb + subx*subx*rate + suby*suby*rate);

					if(sub >nowSub)
					{
						c = w;
						sub = nowSub;
					}
				}
				vec[c].push_back(CPoint(i,j));
			}
		}
		//计数
		for(int i=0;i<k;i++)
		{
			numCPoint[i] = vec[i].size();
		}
		//保存备份
		for(int i=0;i<k;i++)
		{
			pre[i] = cla[i];
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
			cla[i].r = r / len;
			cla[i].g = g / len;
			cla[i].b = b / len;
			cla[i].x = x / len;
			cla[i].y = y / len;
		}

		//判断是否结束
		bool stop = false;
		for(int i=0;i<k;i++)
		{
			double subr = pre[i].r - cla[i].r;
			double subg = pre[i].g - cla[i].g;
			double subb = pre[i].b - cla[i].b;
			int subx = pre[i].x - cla[i].x;
			int suby = pre[i].y - cla[i].y;
			double nowSub = sqrt(subr*subr+subg*subg+subb*subb+subx*subx*rate+suby*suby*rate);
			if(nowSub > sublim0)
			{
				stop = true;
				break;
			}

		}
		isNotOk = stop;
	/*	if(isNotOk)
		for(int i=0;i<k;i++)
		{	
#ifdef FENKUAI 
			int len = vec[i].size();
			if(len ==0){
				k2CPoint[i][0] = CPoint(-1,0);
				k2CPoint[i][1] = CPoint(-1,0);
				continue;
			}
			int maxX = vec[i][0].x;
			int maxY = vec[i][0].y;
			int minX = vec[i][0].x;
			int minY = vec[i][0].y;
			for(int j=1;j<len;j++)
			{
				if(maxX < vec[i][j].x)
				{
					maxX = vec[i][j].x;
				}
				if(maxY < vec[i][j].y)
				{
					maxY = vec[i][j].y;
				}
				if(minX > vec[i][j].x)
				{
					minX = vec[i][j].x;
				}
				if(minY > vec[i][j].x)
				{
					minY = vec[i][j].y;
				}
			}
			k2CPoint[i][0] = CPoint(minX - rwidth/2,minY - rheight/2);
			k2CPoint[i][1] = CPoint(maxX + rwidth/2,maxY + rheight/2);
#endif
			vec[i].clear();
		}*/
	}



	//处理完成
	int ** fenClass = new int* [width];
	for(int i=0;i<width;i++)
	{
		fenClass[i] = new int[height];
	}

	for(int i=0;i<width;i++)
	{
		for(int j=0;j<height;j++)
		{
			fenClass[i][j] = -1;
		}
	}
	for(int i=0;i<k;i++)
	{
		int len = vec[i].size();
		for(int j=0;j<len;j++)
		{
			int x = vec[i][j].x;
			int y = vec[i][j].y;
			fenClass[x][y] = i;
		}
	}
	int Move[][2] = {0,1,0,-1,1,0,-1,0,1,1,1,-1,-1,1,-1,-1};
	//消除小类
#ifdef CUTMIN

	bool **p = new bool*[width];
	for(int i=0;i<width;i++)
	{
		p[i] = new bool[height];
	}
	for(int i=0;i<width;i++)
	{
		for(int j=0;j<height;j++)
		{
			p[i][j] = 0;
		}
	}
	queue<CPoint> tem;
	vector<CPoint> vecArr;
	
	int Num = 0;
#ifdef ONLYONE
	for(int i=0;i<width;i++)
	{
		for(int j = 0;j<height;j++)
		{
			if(p[i][j])
				continue;
			vecArr.clear();
			int tK = fenClass[i][j];
			int wK = tK;
			tem.push(CPoint(i,j));
			vecArr.push_back(CPoint(i,j));
			p[i][j] = 1;
			int minX = i;
			int minY = j;
			int maxX = i;
			int maxY = j;
			vector<Sign> cRgb(k);
			vector<int> cNum(k);
			for(int ii = 0; ii<k ;ii++)
			{
				cRgb[ii].r = 0;
				cRgb[ii].g = 0;
				cRgb[ii].b = 0;
				cNum[ii] = 0;
			}
			cRgb[tK].r += lab[i][j][0];
			cRgb[tK].g += lab[i][j][1];
			cRgb[tK].b += lab[i][j][2];
			cNum[tK] ++;
			while(!tem.empty())
			{
				CPoint b = tem.front();
				tem.pop();
				for(int k=0;k<8;k++)
				{
					int nx = b.x + Move[k][0];
					int ny = b.y + Move[k][1];
					if( nx<0 || ny< 0 || nx >=width || ny >= height)
						continue;

					int c = fenClass[nx][ny];
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
					cRgb[c].r += lab[nx][ny][0];
					cRgb[c].g += lab[nx][ny][1];
					cRgb[c].b += lab[nx][ny][2];
					cNum[c] ++;
				}
			}
			if(maxX - minX < MaxDisX && maxY - minY < MaxDisX || vecArr.size() < MinPixelNum)
			{
				double avgR,avgG,avgB;
				double lavgR,lavgG,lavgB;
				avgR = cRgb[tK].r / cNum[tK];
				avgG = cRgb[tK].g / cNum[tK];
				avgB = cRgb[tK].b / cNum[tK];
				double preSub = INFF;
				for(int ii = 0; ii < k ;ii++)
				{
					if(ii == tK || cNum[ii] == 0)
						continue;
					lavgR = cRgb[ii].r / cNum[ii];
					lavgG = cRgb[ii].g / cNum[ii];
					lavgB = cRgb[ii].b / cNum[ii];
					double sub = (lavgR - avgR)*(lavgR - avgR) + (lavgG - avgG)*(lavgG - avgG) + (lavgB - avgB)*(lavgB - avgB);
					if(preSub > sub)
					{
						preSub = sub;
						wK = ii;
					}
				}
				int len = vecArr.size();
				for(int ii = 0; ii < len; ii++)
				{
					fenClass[vecArr[ii].x][vecArr[ii].y] = wK;
					p[vecArr[ii].x][vecArr[ii].y] = 0;
				}
			}
		}
	}
#else
	for(int i=0;i<width;i++)
	{
		for(int j = 0;j<height;j++)
		{
			if(p[i][j])
				continue;
			vecArr.clear();
			int tK = fenClass[i][j];
			int wK = tK;
			tem.push(CPoint(i,j));
			vecArr.push_back(CPoint(i,j));
			p[i][j] = 1;
			int minX = i;
			int minY = j;
			int maxX = i;
			int maxY = j;
			vector<CPoint> cNum(k);
			vector<int> cCount(k);
			vector<bool> isLink(k);
			for(int ii = 0; ii<k ;ii++)
			{
				cNum[ii].x = 0;
				cNum[ii].y = 0;
				isLink[ii] = 0;
				cCount[ii] = 0;
			}
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

					int c = fenClass[nx][ny];
					if(c == tK)
					{
						if(p[nx][ny])
							continue;
						p[nx][ny] = 1;
						vecArr.push_back(CPoint(nx,ny));
						tem.push(CPoint(nx,ny));
						if(nx >maxX)
							maxX = nx;
						else if(nx < minX)
							minX = nx;
						if(ny > maxY)
							maxY = ny;
						else if(ny < minY)
							minY = ny;
						
					}
					isLink[c] = true;
				}
			}
			if(maxX - minX <MaxDisX && maxY - minY < MaxDisX || vecArr.size() < MinPixelNum)
			{
				int nwidth = maxX - minX;
				int nheight = maxY - minY;
				int swidth = max(0,minX - nwidth);
				int sheight = max(0,minY - nheight);
				int ewidth = min(maxX + nwidth,width);
				int eheight = min(maxY + nheight,height);
				for(int ii = swidth; ii < ewidth ; ii++)
				{
					for(int jj = sheight ; jj < eheight ; jj++)
					{
						if(isLink[fenClass[ii][jj]])
						{
							cNum[fenClass[ii][jj]].x += ii;
							cNum[fenClass[ii][jj]].y += jj;
							cCount[fenClass[ii][jj]] ++;
						}
					}
				}
				int t = 0; 
				double tX = cNum[tK].x *1.0 / cCount[tK];
				double tY = cNum[tK].y *1.0 / cCount[tK];
				double sub = INFF;
				for(int ii=1;ii<k;ii++)
				{
					if(ii == tK || !isLink[ii])
						continue;
					double nX = cNum[ii].x *1.0 / cCount[ii];
					double nY = cNum[ii].y *1.0 / cCount[ii];
					double nSub = (nX - tX)*(nX - tX) + (nY - tY)*(nY - tY);
					if(nSub < sub)
					{
						sub = nSub;
						t = ii;
					}
				}
				wK = t;
				int len = vecArr.size();
				for(int ii = 0; ii < len; ii++)
				{
					fenClass[vecArr[ii].x][vecArr[ii].y] = wK;
					p[vecArr[ii].x][vecArr[ii].y] = 0;
				}
			}
		}
	}
#endif
	deleteMem<bool>(p,width);

#endif
	//重新制作
	//原图
#ifndef ONLYCOLOR
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
				if(fenClass[i][j]!=fenClass[nx][ny])
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
#else
	//纯色
#define unsigned short USHORT
	USHORT **rgbK;
	rgbK = new USHORT *[k];

	for(int i=0;i<k;i++)
	{

		rgbK[i] = new USHORT[3];
#ifndef RANDCOLOR
		int len = vec[i].size();
		double xx=0,yy=0,zz=0;
		for(int j=0;j<len;j++)
		{
			int tx = vec[i][j].x;
			int ty = vec[i][j].y;
			xx += lab[tx][ty][0];
			yy += lab[tx][ty][1];
			zz += lab[tx][ty][2];
		}
		xx/=len;
		yy/=len;
		zz/=len;
		XYZ2RGB(xx,yy,zz,rgbK[i][0],rgbK[i][1],rgbK[i][2]);
#else
		rgbK[i][0] = rand()%255;
		rgbK[i][1] = rand()%255;
		rgbK[i][2] = rand()%255;
#endif
	}
	for(int i=0;i<width;i++)
	{
		for(int j=0;j<height;j++)
		{
			int c = fenClass[i][j];
			des.SetPixel(i,j,RGB(rgbK[c][0],rgbK[c][1],rgbK[c][2]));
		}
	}
	for(int i=0;i<k;i++)
	{
		delete []rgbK[i];
	}
	delete []rgbK;
#endif

#ifdef TXT
	FILE * file = NULL;
	fopen_s(&file,"txt.txt","w");
	for(int i=0;i<width;i++)
	{
		for(int j=0;j<height;j++)
		{
			fprintf_s(file,"%5d",fenClass[i][j]);
		}
		fprintf_s(file,"\r\n");
	}
	fclose(file);
#endif
	//释放内存
	delete []cla;
	delete []pre;
	delete []vec;
	for(int i=0;i<width;i++)
	{
		delete[]fenClass[i];
	}
	delete []fenClass;
	for(int i=0;i<k;i++)
	{
		delete[]k2CPoint[i];
	}
	delete []k2CPoint;
	for(int i = 0 ; i< width ;i ++)
	{
		delete [] lab[i][0];
		delete [] lab[i][1];
		delete [] lab[i][2];
		delete [] lab[i];
	}
	delete []lab;

}

void KMeans1(CImage &res,CImage &des,int k,double rate)
{
	des = res;
	vector<vector<vector<float>>> lab;
	vector<vector<double>> grad;
	vector<vector<int>> fenClass_c;
	Image2Lab(des,lab);
	GetImageGrad(lab,grad);
	KMeans_Core(lab,grad,fenClass_c,k,rate);
	//MergeClassByColor(lab,fenClass_c,k);
	DrawBorder1(des,fenClass_c);
}