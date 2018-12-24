#include "stdafx.h"
using namespace std;
void LeftK2(CImage &res,CImage &des,int k,CString path)
{
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
	//转换颜色空间
	for(int i=0;i<width;i++)
	{
		for(int j=0;j<height;j++)
		{
			byte * colorRes = (byte *)res.GetPixelAddress(i,j);
			float * color = new float[3];
			RGB2XYZ(colorRes[0],colorRes[1],colorRes[2],color[0],color[1],color[2]);
			lab[i][j] = color;
		}
	}
	int space_len = 4*width*height - 3*(width+height)+2;
	int pixel_num = width * height;
	Value_Pos * space_all = new Value_Pos[space_len];
	int *link = new int[pixel_num];
	_Rgb * class2RGB = new _Rgb[pixel_num];
	int *pointNum = new int[pixel_num];
	int Move[][2]={0,-1,1,-1,1,0,1,1};
	int numPos = 0;

	for(int i = 0 ; i < pixel_num ; i ++)
	{
		link[i] = i;
		pointNum[i] = 1;
	}
	for(int i = 0;i<width ; i++ )
	{
		for(int j=0;j<height;j++)
		{
			int pos = i * height + j;
			class2RGB[pos].r = lab[i][j][0];
			class2RGB[pos].g = lab[i][j][1];
			class2RGB[pos].b = lab[i][j][2];
			for(int ii = 0; ii < 4; ii++)
			{
				int nx = i + Move[ii][0];
				int ny = j + Move[ii][1];
				if(nx < 0 || nx >= width || ny <0 || ny >= height)
				{
					continue;
				}
				double sr,sg,sb;
				sr = lab[i][j][0] - lab[nx][ny][0];
				sg = lab[i][j][1] - lab[nx][ny][1];
				sb = lab[i][j][2] - lab[nx][ny][2];
				double sub = sr*sr + sg*sg + sb*sb;
				Value_Pos & tmp = space_all[numPos];
				tmp.sub = sub;
				tmp.a = pos;
				tmp.b = nx*height + ny;
				numPos++;
			}
			
		}
	}
	sort(space_all,space_all+space_len);
	int left = pixel_num;
	for(int i=0 ; left > k && i < space_len; i++)
	{
		Value_Pos &tmp = space_all[i];
		int a = Find(link,tmp.a);
		int b = Find(link,tmp.b);
		if(a == b)
			continue;
		link[a] = b;
		left --;
	}
	//算法结束
	int ** fenClass = new int* [width];
	for(int i=0;i<width;i++)
	{
		fenClass[i] = new int[height];
	}
	int pos = 0;
	for(int i=0;i<width;i++)
	{
		for(int j=0;j<height;j++)
		{	
			fenClass[i][j] = Find(link,pos);
			pos++;
		}
	}
	//划线
	int Move8[][2] = {0,1,0,-1,1,0,-1,0,1,1,1,-1,-1,1,-1,-1};
	for(int i=0;i<width;i++)
	{
		for(int j=0;j<height;j++)
		{
			bool isWhite = false;
			for(int w=0;w<8;w++)
			{
				int nx = i+Move8[w][0];
				int ny = j+Move8[w][1];
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
				des.SetPixel(i ,j,RGB(255,255,255));
			}
		}
	}
	FILE * file = NULL;
	fopen_s(&file,"txt.txt","w");
	int nowPos = 0;
	for(int i=0;i<width;i++)
	{
		for(int j=0;j<height;j++)
		{
			fprintf_s(file,"%10d",link[nowPos++]);
		}
		fprintf_s(file,"\r\n");
	}
	fclose(file);
	//释放内存
	delete []space_all;
	delete []link;
	delete []class2RGB;
	delete []pointNum;
	for(int i=0;i<width;i++)
	{
		delete[]fenClass[i];
	}
	delete []fenClass;
}