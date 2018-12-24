#include "stdafx.h"
#include "RGB2Lab.h"


CRGB2Lab::CRGB2Lab(void)
{
}


CRGB2Lab::~CRGB2Lab(void)
{
}
CRGB2Lab::CRGB2Lab(int r,int g,int b)
{
	this->r = r;
	this->g = g;
	this->b = b;
}
void CRGB2Lab::setRGB(int r,int g,int b)
{
	this->r = r;
	this->g = g;
	this->b = b;
}
double CRGB2Lab::getX()
{
	return 0.412453f * r + 0.357580f * g + 0.180423f * b;
}
double CRGB2Lab::getY()
{
	return 0.212671f * r + 0.715160f * g + 0.072169f * b;
}
double CRGB2Lab::getZ()
{
	return 0.019334f * r + 0.119193f * g + 0.950227f * b;
}