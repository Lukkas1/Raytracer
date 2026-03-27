#include "SimpleRayTracer.h"

#include <atomic>

#include "rgbimage.h"
#include <stdio.h>
#include <math.h>

/**
 * @param zvalue Die Position des Augpunkts auf der Z-Achse. Muss positiv sein
 * @param planedist Abstand zur Bildebene
 * @param width Breite der Bildebene in Raumeinheiten.
 * @param height Höhe der Bildebene in Raumeinheiten.
 * @param widthInPixel  Die Breite der Bildebene in Pixeln (z. B. 640 Pixel)
 * @param heightInPixel Die Höhe der Bildebene in Pixeln (z. B. 480 Pixel)
 */
Camera::Camera( float zvalue, float planedist, float width, float height, unsigned int widthInPixel, unsigned int heightInPixel )
	: zvalue(zvalue), planedist(planedist), width(width), height(height), widthInPixel(widthInPixel), heightInPixel(heightInPixel)
{
	if (zvalue < 0) return;
}

/**
 * @param x x-Koordinate des zu durchschießenden Pixels
 * @param y y-Koordinate des zu durchschießenden Pixels
 * @return Vektor Augpunkt durch Pixel
 */
Vector Camera::generateRay( unsigned int x, unsigned int y) const
{
	Vector augpunkt = Position();

	unsigned int z = zvalue - planedist; //z liegt in Ebene
	Vector pixel = Vector(x,y,z);

	return pixel - augpunkt;
}

Vector Camera::Position() const
{
	return Vector(0,0,this->zvalue);
}

SimpleRayTracer::SimpleRayTracer(unsigned int MaxDepth)
{
	// TODO: Add your code
}


void SimpleRayTracer::traceScene( const Scene& SceneModel, RGBImage& Image)
{
	// TODO: Add your code
}

Color SimpleRayTracer::localIllumination( const Vector& Surface, const Vector& Eye, const Vector& N, const PointLight& Light, const Material& Mtrl )
{
	// TODO: Add your code
	return Color(); // dummy (remove)
}

Color SimpleRayTracer::trace( const Scene& SceneModel, const Vector& o, const Vector& d, int depth)
{
	// TODO: Add your code
	return Color(); // dummy (remove)
}

