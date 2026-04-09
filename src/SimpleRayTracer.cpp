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
{}

/**
 * @param x x-Koordinate des zu durchschießenden Pixels
 * @param y y-Koordinate des zu durchschießenden Pixels
 * @return Vektor Augpunkt durch Pixel
 */
Vector Camera::generateRay( unsigned int x, unsigned int y) const
{
	/*Vector augpunkt = Position();

	unsigned int z = zvalue - planedist; //z liegt in Ebene
	Vector pixel = Vector(x,y,z);

	return (pixel - augpunkt).normalize();*/

	// Schrittweite pro Pixel
	float dx = width / (float)widthInPixel;
	float dy = height / (float)heightInPixel;

	// Pixelkoordinaten in die 3D-Szene umrechnen.
	// y-Koordinate wird invertiert, da Bild-Y nach unten und Szenen-Y nach oben steigt.
	float px = -width / 2.0f + (x + 0.5f) * dx;
	float py = height / 2.0f - (y + 0.5f) * dy;
	float pz = zvalue + planedist;

	Vector pixelPos(px, py, pz);

	// Richtung vom Augpunkt zum Pixel berechnen
	Vector dir = pixelPos - Position();
	dir.normalize(); // Strahl muss Länge eins besitzen

	return dir;
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
	Camera camera = Camera(-8, 1, 1, 0.75, Image.width(), Image.height());
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

// Nur Difuse-Komponente des Materials für Aufgabe 2
/*Color SimpleRayTracer::localIllumination( const Vector& Surface, const Vector& Eye, const Vector& N, const PointLight& Light, const Material& Mtrl )
{
	return Mtrl.getDiffuseCoeff(Surface);
}*/

/**
 * 
 * @param Surface  Punkt auf der Oberfläche (Schnittpunkt Oberfläche mit Strahl)
 * @param Eye Position des Augpunkts
 * @param N Normale des Dreiecks (übergeben Sie hier Triangle::calcNormal(...))
 * @param Light  Eine Punktlichtquelle (Position und Intensität der Lichtquelle)
 * @param Mtrl das Material des Dreiecks
 * @return 
 */
Color SimpleRayTracer::localIllumination( const Vector& Surface, const Vector& Eye, const Vector& N, const PointLight& Light, const Material& Mtrl )
{
	Color totalColor(0, 0, 0);

	// Vektor zur Lichtquelle (L)
	Vector L = Light.Position - Surface;
	L.normalize();

	// Ambiente Komponente (Vorlesung 3, S. 20) wird in trace() als Grundfarbe verwendet

	// Diffuse Komponente (Vorlesung 3, S. 17)
	// N * L aus Folie. max() für wenn die Lichtquelle hinter Dreieck dann Skalarprodukt negativ dann schwärzer als schwarz also bei 0 abschneiden
	totalColor += Light.Intensity * Mtrl.getDiffuseCoeff(Surface) * N.dot(L);

	// Spekulare Komponente (Phong) (Vorlesung 3, S. 18)
	// R = Reflexionsvektor des Lichtstrahls an der Normalen
	// R = 2N(N dot L) - L
	Vector R = N * (2.0f * N.dot(L)) - L;
	R.normalize();

	float specularFactor = pow(Eye.dot(R), Mtrl.getSpecularExp(Surface));
	totalColor += Light.Intensity * Mtrl.getSpecularCoeff(Surface) * specularFactor;

	return totalColor;
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

	/* Vorlesung:
	 * 4. Berechne ersten Auftreffpunkt p des Strahls s auf der Objektoberfläche;
	5. Prüfe, ob von Punkt p Sichtverbindung zur Lichtquelle besteht:
	5.1. Wenn JA:
		F = Berechne_Lokales_Beleuchtungsmodell(...)
	6. Berechne Reflexionsstrahl r von s im Punkt p;
	7. F = F + Raytracing(r) * Reflexionskoeffizient;
	8. Berechne Transmissionsstrahl t von s im Punkt p;
	9. F = F + Raytracing(t) * Transmissionskoeffizient;
	10. Return F;
	*/


	float min_s = 1e9f; // Startwert für die Distanzssuche (unendlich)
    int closest_triangle_idx = -1;

    // 4. Berechne ersten Auftreffpunkt p des Strahls auf der Objektoberfläche
    for (unsigned int i = 0; i < SceneModel.getTriangleCount(); ++i) {
        const Triangle& tri = SceneModel.getTriangle(i);
        float s = 0.0f;

        // Strahl-Dreieck-Kollisionstest (Vorlesung 02, Folien 17-26)
        if (o.triangleIntersection(d, tri.A, tri.B, tri.C, s)) {
            // Finde den kleinsten, nicht-negativen Wert für s (nächstgelegenes Dreieck)
            // kleines Epsilon verhindert die Kollision mit dem eigenen Startpunkt Shadow Acne
            if (s > 0.001f && s < min_s) {
                min_s = s;
                closest_triangle_idx = i;
            }
        }
    }

    // Wenn nichts getroffen wurde
    if (closest_triangle_idx == -1) {
        return Color(0, 0, 0);
    }

    // Auftreffpunkt und Normale bestimmen
    const Triangle& hit_tri = SceneModel.getTriangle(closest_triangle_idx);
    Vector hit_point = o + d * min_s; // r(o,d) = o + s*d (Vorlesung 02, Folie 21)
    Vector hit_normal = hit_tri.calcNormal(hit_point);

	//Ambiente Komponente als Grundfarbe
	Color final_color = hit_tri.pMtrl->getAmbientCoeff(hit_point);

    Vector eye_dir = (o - hit_point); // Vektor vom Schnittpunkt zurück zur Kamera
    eye_dir.normalize();

    // 5. Prüfe, ob Sichtverbindung zur Lichtquelle besteht (Vorlesung 02, Folie 21 & 27)
    for (unsigned int i = 0; i < SceneModel.getLightCount(); ++i) {
        const PointLight& light = SceneModel.getLight(i);
        Vector light_dir = light.Position - hit_point;
        float dist_to_light = light_dir.length();
        light_dir.normalize();

        bool in_shadow = false;

        // Schattenstrahl gegen alle Dreiecke testen
        for (unsigned int j = 0; j < SceneModel.getTriangleCount(); ++j) {
            // Ursprungsdreieck beim Kollisionstest ausschließen [cite: 138]
            if (j == closest_triangle_idx) continue;

            const Triangle& shadow_tri = SceneModel.getTriangle(j);
            float shadow_s = 0.0f;

            if (hit_point.triangleIntersection(light_dir, shadow_tri.A, shadow_tri.B, shadow_tri.C, shadow_s)) {
                // Wenn ein Dreieck VOR der Lichtquelle getroffen wird -> Punkt liegt im Schatten. 0.001 für
                if (shadow_s > 0.001f && shadow_s < dist_to_light) {
                    in_shadow = true;
                    break; //egal welches Objekt im weg performance
                }
            }
        }

        // 5.1 Wenn JA (kein Schatten): Berechne lokales Beleuchtungsmodell
        if (!in_shadow) {
            final_color += localIllumination(hit_point, eye_dir, hit_normal, light, *(hit_tri.pMtrl));
        }
    }

    // 6. Berechne Reflexionsstrahl r von s im Punkt p
    float reflectivity = hit_tri.pMtrl->getReflectivity(hit_point);
    if (reflectivity > 0.0f && depth > 1) {
        Vector r = d.reflection(hit_normal);
        r.normalize();

        // 7. F = F + Raytracing(r) * Reflexionskoeffizient
        Color reflection_color = trace(SceneModel, hit_point, r, depth - 1);
        final_color += reflection_color * reflectivity;
    }

    // 10. Return F
	return final_color;
}

