#include "vector.h"
#include <assert.h>
#include <math.h>

Vector::Vector( float x, float y, float z)
{
	this->X = x;
    this->Y = y;
    this->Z = z;
}

Vector::Vector()
{
	//x,y,z bleiben uninitialisiert
}

float Vector::dot(const Vector& v) const //Skalarprodukt
{
	float result = this->X * v.X;
    result += this->Y * v.Y;
    result += this->Z * v.Z;

    return result;
}

Vector Vector::cross(const Vector& v) const //Kreuzprodukt
{
	Vector result;

    result.X = this->Y * v.Z - this->Z * v.Y;
    result.Y = this->Z * v.X - this->X * v.Z;
    result.Z = this->X * v.Y - this->Y * v.X;

    return result;
}



Vector Vector::operator+(const Vector& v) const
{
	Vector result;

    result.X = this->X + v.X;
    result.Y = this->Y + v.Y;
    result.Z = this->Z + v.Z;

    return result;
}

Vector Vector::operator-(const Vector& v) const
{
	Vector result;

	result.X = this->X - v.X;
	result.Y = this->Y - v.Y;
	result.Z = this->Z - v.Z;

	return result;
}

Vector Vector::operator*(float c) const
{
	Vector result;

	result.X = this->X * c;
	result.Y = this->Y * c;
	result.Z = this->Z * c;

	return result;
}

Vector Vector::operator-() const
{
	Vector result;

	result.X = -this->X;
	result.Y = -this->Y;
	result.Z = -this->Z;

	return result;
}

Vector& Vector::operator+=(const Vector& v)
{
	this->X = this->X + v.X;
    this->Y = this->Y + v.Y;
    this->Z = this->Z + v.Z;
	return *this;
}



Vector& Vector::normalize()
{
    *this = *this * (1/this->length());
    return *this;

    //return (*this * (1/this->length()));
}

float Vector::length() const
{
	return sqrt(X * X + Y * Y + Z * Z);
}

float Vector::lengthSquared() const
{
	return length() * length();
}

Vector Vector::reflection( const Vector& normal) const
{
    //w = v - 2 * (v ∙ n) * n
	return *this - (normal * (2 * this->dot(normal)));

}

bool Vector::triangleIntersection( const Vector& d, const Vector& a, const Vector& b, const Vector& c, float& s) const
{
    //d ist richtung des Strahls
    //*this bzw. o ist Ursprung des Strahls
    //abc sind Eckpunkte Dreieck
	//s ist Abstand bis zum Schnittpunkt

	//normalenvektor n = (b-a) x (c-a)
	Vector n = (b - a).cross(c - a);
	n.normalize();

	// s = (a-o)*n / d*n
	float nenner = n.dot(d);
	if (fabs(nenner) < 0.0001f) return false;

	//Formel für a aus Folie:  s = d-n*o / d*n
	//Meine verwendete Formel: s = (a-o)*n / d*n

	//Herleitung:
	// 0=n*p - d => d=n*p
	// Einsetzen d in Zaehler Folienformel
	// n*p - n*o = (p-o)*n
	// => (p-o)*n
	// Unterschied zu meiner Formel p = a; warun geht das? => Ebenengleichung besagt: n*p=n*a

	s = (a - *this).dot(n) / nenner;

	//Dreieck hinter Strahlurpsrung
	if (s < 1e-5f) {
		return false;
	}

    Vector p = (*this) + d*s; // p ist Auftreffpunkt

	if (area(a,b,c) + 1e-5f >= area(a,b,p) + area(a,c,p) + area(b,c,p)) return true;
	return false;
}

float Vector::area(const Vector& u, const Vector& v, const Vector& w) const
{
    /*Vector h1 = v - u;
    Vector h2 = w - u;
    Vector h3 = h1.cross(h2);
    float result = h3.length();
    return result/2;*/

	return (v - u).cross(w-u).length()/2;
}
