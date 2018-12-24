
#include "stdafx.h"

using namespace std;



void LeftK(CImage &res,CImage &des,int k,CString path)
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
			RGB2LAB(colorRes[0],colorRes[1],colorRes[2],color[0],color[1],color[2]);
			lab[i][j] = color;
		}
	}
//算法开始
	map<double,set<Point>> value2class;
	map<Point,double>class2value;
	vector<set<int>> class2class(width*height);
	map<int,_Rgb> class2RGB;
	int *link = new int[width * height];
	int *pointNum = new int[width * height];
	double *valueC = new double[width * height];
	int left = width * height;
//初始化
	//link表初始化
	for(int i=0;i< width * height ;i++)
	{
		link [i] = i;
		pointNum[i] = 1;
		valueC[i] = 100000000000;
	}
//类点表初始化及颜色表及相邻初始化
	int Move[][2]={0,-1,1,-1,1,0,1,1};
	int numPos = 0;
	int left2 = left - k;
	for(int i = 0;i<width ; i++ )
	{
		for(int j=0;j<height;j++)
		{
			int c = numPos++;
			//初始化颜色表
			class2RGB[c].r = lab[i][j][0];
			class2RGB[c].g = lab[i][j][1];
			class2RGB[c].b = lab[i][j][2];
			//初始化相邻表及类值表
			for(int ii = 0; ii < 4; ii++)
			{
				int nx = i + Move[ii][0];
				int ny = j + Move[ii][1];
				if(nx < 0 || nx >= width || ny <0 || ny >= height)
				{
					continue;
				}
				int nc = nx*height + ny;
				class2class[c].insert(nc);
				class2class[nc].insert(c);
				double sr,sg,sb;
				sr = lab[i][j][0] - lab[nx][ny][0];
				sg = lab[i][j][1] - lab[nx][ny][1];
				sb = lab[i][j][2] - lab[nx][ny][2];
				double sub = sr*sr + sg*sg + sb*sb;
				
				value2class[sub].insert(Point(c,nc));
				class2value[Point(c,nc)] = sub;
				
			}
		}
	}
	
//初始化结束
	//开始聚类
	FILE *f = NULL;
	fopen_s(&f,"log.txt","w");
	//
	
	while(left > k && value2class.size() >0 )
	{
		fprintf(f,"%d\n",left);
		fflush(f);
		//找到第一个最相似的
		map<double,set<Point>>::iterator ite = value2class.begin();
		set<Point>& first = ite->second;
		set<Point>::iterator ite1 = first.begin();
		int ca = 0, cb = 0;
		bool isFind = false;
		while(true)
		{
			ca = Find(link,ite1->x);
			cb = Find(link,ite1->y);
			int ta = ite1->x;
			int tb = ite1->y;
			if(ca != cb)
			{
				isFind = true;
				break;
			}
			ite1 = first.erase(ite1);
			class2value.erase(Point(ta,tb));
			if(ite1 == first.end())
			{
				break;
			}
		}
		if(!isFind)
		{
			value2class.erase(ite);
			continue;
		}
		int &cNuma = pointNum[ca];
 		int &cNumb = pointNum[cb];
		//if(ca!= ite1->x ||cb != ite1->y)
		//{
		//	first.erase(ite1);
		//	if(first.size() == 0)
		//	{
		//		value2class.erase(ite);
		//	}
		//	continue;
		//}
		//_Rgb &caRGB = class2RGB[ite1->x];
		//_Rgb &cbRGB = class2RGB[ite1->y];
		
		//double aavgr = caRGB.r/cNuma;
		//double aavgg = caRGB.g/cNuma;
		//double aavgb = caRGB.b/cNuma;
		//double bavgr = cbRGB.r/cNumb;
		//double bavgg = cbRGB.g/cNumb;
		//double bavgb = cbRGB.b/cNumb;
		//double absubr = aavgr - bavgr;
		//double absubg = aavgg - bavgg;
		//double absubb = aavgb - bavgb;
		////说明这两个类中有类发生了改变
		//if(abs(absubr * absubr + absubg * absubg + absubb * absubb - ite->first) > 0.00000001)
		//{
		//	first.erase(Point(ca,cb));
		//	if(first.size() == 0)
		//	{
		//		value2class.erase(ite);
		//	}
		//	continue;
		//}
		//以上是判断是否合法
		
		set<int> &ac = class2class[ca];
		set<int> &bc = class2class[cb];
		set<int> * cac = NULL;
		set<int> * cbc = NULL;
		if(ac.size() >= bc.size())
		{
 			link[cb] = ca;
			cNuma +=cNumb;	
			cac = &ac;
			cbc = &bc;
		}
		else 
		{
			cNumb +=cNuma;	
			link[ca] = cb;
			cac = &bc;
			cbc = &ac;
			int  t = ca;
			ca = cb;
			cb = t;
		}
		//更新颜色表
		_Rgb &rgba = class2RGB[ca];
		_Rgb &rgbb = class2RGB[cb];
		rgba.r += rgbb.r;
		rgba.g += rgbb.g;
		rgba.b += rgbb.b;
		//
		for(auto it1 = cac->begin();it1!=cac->end();it1++)
		{
			int sa = ca,sb = *it1;
			P2D_ITE va = class2value.find(Point(sa,sb));
			if(va == class2value.end())
				continue;
			double val = va->second;
			Point ps = va->first;
			class2value.erase(va);
			D2SP_ITE seta = value2class.find(val);
			if(seta == value2class.end())
				continue;
			seta->second.erase(ps);
			if(seta->second.size() == 0)
				value2class.erase(seta);
		}
		for(auto it2 = cbc->begin();it2!=cbc->end();it2++)
		{
			int sa = cb,sb = *it2;
			P2D_ITE va = class2value.find(Point(sa,sb));
			if(va == class2value.end())
				continue;
			double val = va->second;
			Point ps = va->first;
			class2value.erase(va);
			D2SP_ITE seta = value2class.find(val);
			if(seta == value2class.end())
				continue;
			seta->second.erase(ps);
			if(seta->second.size() == 0)
				value2class.erase(seta);
		}
		//更新相邻表
		cac->erase(cb);
		cbc->erase(ca);
		for(auto ites1 = cac->begin();ites1!=cac->end();ites1++)
		{
			cbc->insert(Find(link,*ites1));
		}
		cac->clear();
		for(auto ites2 = cbc->begin();ites2!=cbc->end();ites2++)
		{
			cac->insert(Find(link,*ites2));
		}
		cbc->clear();	
		//更新排序表及类值表
		double avgr = rgba.r/cNuma;
		double avgg = rgba.g/cNuma;
		double avgb = rgba.b/cNuma;
		 
		for(auto ites1 = cac->begin();ites1!=cac->end();ites1++)
		{
			int nowC = Find(link,*ites1);
			if(nowC == ca)
				continue;
			_Rgb &ncrgb = class2RGB[nowC];
			double avgnr = ncrgb.r/pointNum[nowC];
			double avgng = ncrgb.g/pointNum[nowC];
			double avgnb = ncrgb.b/pointNum[nowC];
			double subr = avgr - avgnr;
			double subg = avgg - avgng;
			double subb = avgb - avgnb;
			double subrgb = subr*subr + subg*subg + subb*subb;
			
			value2class[subrgb].insert(Point(ca,nowC));
			class2value[Point(ca,nowC)] = subrgb ;
			
		}
		//消除一个类
		left--;
	}
	//结束聚类
	fclose(f);
	//建立映射矩阵
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

//算法结束，回收资源
	delete [] link;
	delete [] pointNum;
	delete [] valueC;
	for(int i=0;i<width;i++)
	{
		delete[]fenClass[i];
	}
	delete []fenClass;
//
	/*int i;
	for(i=path.GetLength();i>0;i--)
	{
		if(path[i]=='.')
		{
			break;
		}
	}
	path = path.Left(i)+TEXT("KMEAN")+path.Right(path.GetLength()-i);
	des.Save(path);
	AfxMessageBox(CString("保存完成！"));*/
}