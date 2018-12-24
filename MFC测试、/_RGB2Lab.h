#pragma once
#if 0
void RGB2XYZ(unsigned char R, unsigned char G, unsigned char B, float *X, float *Y, float *Z);
void XYZ2Lab(float X, float Y, float Z, float *L, float *a, float *b);
void XYZ2RGB(float x,float y,float z,unsigned short &r,unsigned short &g,unsigned short &b);
#endif
void RGB2LAB(const int& sR, const int& sG, const int& sB, float& lval, float& aval, float& bval);
void RGB2XYZ(
	const int&		sR,
	const int&		sG,
	const int&		sB,
	float&			X,
	float&			Y,
	float&			Z);