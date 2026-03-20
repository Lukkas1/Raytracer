#include "color.h"
#include <assert.h>

Color::Color()
{
  R = 0;
  G = 0;
  B = 0;
}

Color::Color( float r, float g, float b)
{
	R = r;
	G = g;
	B = b;
}

Color Color::operator*(const Color& c) const
{
	Color result = *this;
	result.R = this->R * c.R;
	result.G = this->G * c.G;
	result.B = this->B * c.B;
	return result;
}

Color Color::operator*(const float Factor) const
{
	Color result = *this;
	result.R = this->R * Factor;
	result.G = this->G * Factor;
	result.B = this->B * Factor;
	return result;
}

Color Color::operator+(const Color& c) const
{
	Color result = *this;
	result.R = this->R + c.R;
	result.G = this->G + c.G;
	result.B = this->B + c.B;
	return result;
}

Color& Color::operator+=(const Color& c)
{
	this->R = this->R + c.R;
	this->G = this->G + c.G;
	this->B = this->B + c.B;
	return *this;
}