#include "SimpleRayTracer.h"

#include <atomic>
#include "rgbimage.h"
#include "CGUtilities.h"

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
	: maxDepth(MaxDepth) {}


void SimpleRayTracer::traceScene( const Scene& SceneModel, RGBImage& Image)
{
	//Kamera aus Vorgabe
	Camera camera = Camera(-8, 1, 1, 0.75, 640, 480);
	Vector o = camera.Position();

	for (int x = 0; x < Image.width(); x++)
	{
		for (int y = 0; y < Image.height(); y++)
		{
			/* Für jeden Pixel
			 * 1. Setze Farbe F für Pixel (x,y) auf Schwarz;
			 * 2. Berechne Strahl s von Augpunkt zu Pixel;
			 * 3. F = Raytracing(s); // Rekursion
			 */
			Image.setPixelColor(x, y, Color(0, 0, 0));
			Vector d = camera.generateRay(x, y);
			Color f = trace(SceneModel, o, d, maxDepth);
			Image.setPixelColor(x, y, f);
		}
	}
}

/// Nur Difuse-Komponente des Materials
Color SimpleRayTracer::localIllumination( const Vector& Surface, const Vector& Eye, const Vector& N, const PointLight& Light, const Material& Mtrl )
{
	return Material.getDiffuseCoeff(SurfacePoint);
}

/**
 * @param SceneModel
 * @param o Origin/Augpunkt: Startpunkt des Strahls
 * @param d Direction/Strahlrichtung
 * @param depth verbleibende Rekursionstiefe
 * @return
 */
Color SimpleRayTracer::trace( const Scene& SceneModel, const Vector& o, const Vector& d, int depth)
{
	if (depth == 0) return Color();
	depth--;



	trace(SceneModel, o, d, depth);
}

