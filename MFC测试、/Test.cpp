#include "stdafx.h"

#define R(x) ((x)&0xff)
#define G(x) (((x)>>8)&0xff)
#define B(x) (((x)>>16)&0xff)
void Test(CImage &res,CImage &des,int k,CString path)
{
	des  = res;
	//获取宽和高
	int width = res.GetWidth();
	int height = res.GetHeight();
	double MAXSUB = 100*100 + 255*255 + 255*255;
	//开辟存储lab像素空间
	float *** lab = new float**[width];
	for(int i=0;i<width;i++)
	{
		lab[i] = new float*[height];
	}
	double ** maxSub = new double*[width];
	for(int i=0;i<width;i++)
	{
		maxSub[i] = new double[height];
	}
	//转换颜色空间
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
	int Move[][2] = {0,1,0,-1,1,0,-1,0,1,1,1,-1,-1,1,-1,-1};
	double Max = 0;
	for(int i=0;i<width;i++)
	{
		for(int j=0;j<height;j++)
		{
			double MaxSub = 0;
			for(int w = 0 ; w < 8;w ++)
			{
				int nx = i + Move[w][0];
				int ny = j + Move[w][1];
				if(nx < 0 || nx >= width || ny < 0 || ny >= height)
					continue;
				double sub =	(lab[i][j][0] - lab[nx][ny][0]) * (lab[i][j][0] - lab[nx][ny][0]) + 
								(lab[i][j][1] - lab[nx][ny][1]) * (lab[i][j][1] - lab[nx][ny][1]) +
								(lab[i][j][2] - lab[nx][ny][2]) * (lab[i][j][2] - lab[nx][ny][2]);
				if(sub > MaxSub)
					MaxSub = sub;
				if(sub > Max)
					Max = sub;

			}
			maxSub[i][j] = MaxSub;
		}
	}
	int M = (1<<24) - 1;
	for(int i=0;i<width;i++)
	{
		for(int j=0;j<height;j++)
		{
			maxSub[i][j] = 100*(maxSub[i][j]/Max);	
		}
	}
	unsigned short r,g,b;
	for(int i=0;i<width;i++)
	{
		for(int j=0;j<height;j++)
		{
			//XYZ2RGB((unsigned short)maxSub[i][j],0,0,r,g,b);
			//des.SetPixel(i,j,RGB(r,g,b));
		}
	}
	//释放空间
	for(int i = 0 ; i< width ;i ++)
	{
		delete [] lab[i][0];
		delete [] lab[i][1];
		delete [] lab[i][2];
		delete [] lab[i];
	}
	delete []lab;
	for(int i = 0 ; i < width ; i ++ )
	{
		delete []maxSub[i];
	}
	delete [] maxSub;
}