//
//RGB2Lab Lab2RGB
#include "_RGB2Lab.h"
#include "stdafx.h"
const float param_13 = 1.0f / 3.0f;
const float param_16116 = 16.0f / 116.0f;
const float Xn = 0.950456f;
const float Yn = 1.0f;
const float Zn = 1.088754f;
//#define T_U8 unsigned char
//#define IMAGE_TYPE (void *)
//#define WORD unsigned short
//#define DWORD unsigned int
void RGB2XYZ(
	const int&		sR,
	const int&		sG,
	const int&		sB,
	float&			X,
	float&			Y,
	float&			Z)
{
	double R = sR/255.0;
	double G = sG/255.0;
	double B = sB/255.0;

	double r, g, b;

	if(R <= 0.04045)	r = R/12.92;
	else				r = pow((R+0.055)/1.055,2.4);
	if(G <= 0.04045)	g = G/12.92;
	else				g = pow((G+0.055)/1.055,2.4);
	if(B <= 0.04045)	b = B/12.92;
	else				b = pow((B+0.055)/1.055,2.4);

	X = r*0.4124564 + g*0.3575761 + b*0.1804375;
	Y = r*0.2126729 + g*0.7151522 + b*0.0721750;
	Z = r*0.0193339 + g*0.1191920 + b*0.9503041;
}
void RGB2LAB(const int& sR, const int& sG, const int& sB, float& lval, float& aval, float& bval)
{
#ifdef USERGB
	lval = sR;
	aval = sG;
	bval = sB;
	return ;
#endif
	//------------------------
	// sRGB to XYZ conversion
	//------------------------
	float X, Y, Z;
	RGB2XYZ(sR, sG, sB, X, Y, Z);

	//------------------------
	// XYZ to LAB conversion
	//------------------------
	double epsilon = 0.008856;	//actual CIE standard
	double kappa   = 903.3;		//actual CIE standard

	double Xr = 0.950456;	//reference white
	double Yr = 1.0;		//reference white
	double Zr = 1.088754;	//reference white

	double xr = X/Xr;
	double yr = Y/Yr;
	double zr = Z/Zr;

	double fx, fy, fz;
	if(xr > epsilon)	fx = pow(xr, 1.0/3.0);
	else				fx = (kappa*xr + 16.0)/116.0;
	if(yr > epsilon)	fy = pow(yr, 1.0/3.0);
	else				fy = (kappa*yr + 16.0)/116.0;
	if(zr > epsilon)	fz = pow(zr, 1.0/3.0);
	else				fz = (kappa*zr + 16.0)/116.0;

	lval = 116.0*fy-16.0;
	aval = 500.0*(fx-fy);
	bval = 200.0*(fy-fz);
}

#if 0
void RGB2XYZ(unsigned char R, unsigned char G, unsigned char B, float *X, float *Y, float *Z)
{
	 *X = 0.412453f * R + 0.357580f * G + 0.180423f * B;
	 *Y = 0.212671f * R + 0.715160f * G + 0.072169f * B;
	 *Z = 0.019334f * R + 0.119193f * G + 0.950227f * B;
}
 
void XYZ2Lab(float X, float Y, float Z, float *L, float *a, float *b)
{
	float fX, fY, fZ;
#if 1
	X /= (255.0*Xn);
	Y /= (255.0*Yn);
	Z /= (255.0*Zn);
#endif
 
 
	if (Y > 0.008856f)
	{
		fY = pow(Y, param_13);
		*L = 116.0f * fY -16.0f;
	}
	else
	{
		fY = 7.787f * Y + param_16116;
		*L = 903.3*fY;
	}
	
	
	*L = *L > 0.0f ? *L : 0.0f;
	
	if (X > 0.008856f)
		fX = pow(X, param_13);
	else
		fX = 7.787f * X + param_16116;
	
	if (Z > 0.008856)
		fZ = pow(Z, param_13);
	else
		fZ = 7.787f * Z + param_16116;
 
	*a = 500.0f * (fX - fY);
	*b = 200.0f * (fY - fZ);
 
 
}
#if 0
int RGB2Lab(IMAGE_TYPE * bmp_img,float *lab_img)
{
	DWORD width,height,index;
	WORD  biBitCount;
	T_U8 *dst,*bmp,R,G,B;
	float X,Y,Z,L,a,b;
	
	T_U32 line_byte;
	T_U16 i,j;
	
	BITMAPFILEHEADER bf;
	BITMAPINFOHEADER bi;
	
	memset(&bf, 0, sizeof(bf));
	memset(&bi, 0, sizeof(bi));
	
	bmp = bmp_img;
	memcpy(&bf,bmp,14);
	memcpy(&bi,&bmp[14],40);
	
	height = bi.biHeight;
	width  = bi.biWidth;
	biBitCount = bi.biBitCount;//每一个像素由24 bits表示，即RGB分量每一个分量用8 bits表示
	line_byte = WIDTHBYTES(width*bi.biBitCount);
 
	dst = bmp_img+BMPHEADSIZE;
	
	for (i = 0; i <height;i++)
	{
		for (j = 0;j < width;j++)
		{
			index = i*line_byte+3*j;
			B = dst[index];
			G = dst[index+1];
			R = dst[index+2];
 
			RGB2XYZ(R,G,B,&X,&Y,&Z);
			XYZ2Lab(X,Y,Z,&L,&a,&b);
 
			lab_img[index] = L;
			lab_img[index+1] = a;
			lab_img[index+2] = b;
		}
	}
    
	return 0;
 
}
#endif

void XYZ2RGB(float x,float y,float z,unsigned short &r,unsigned short &g,unsigned short &b)
{
	r = 3.240479*x -1.537150 * y - 0.498535*z;
	g = -0.969256*x + 1.87992 * y + 0.041556*z;
	b = 0.055648*x -0.204043 * y + 1.057311*z;
}
#endif