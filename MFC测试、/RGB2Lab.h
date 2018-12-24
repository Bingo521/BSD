#pragma once
class CRGB2Lab
{
	int r,g,b;
public:
	CRGB2Lab(void);
	~CRGB2Lab(void);
	CRGB2Lab(int r,int g,int b);
	void setRGB(int r,int g,int b);
	double getX();
	double getY();
	double getZ();
};

