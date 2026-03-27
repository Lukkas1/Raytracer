#include "CGUtilities.h"
#include "SimpleRayTracer.h"

int main(int argc, const char * argv[])
{
    //Einstellungen für Aufgabe 4
    unsigned int sceneComplexity=20;
    int maxDepth=2;
    int zvalue=-8;
    int planedist=1;
    int width=1;
    int height=0.75;
    int widthInPixel=640;
    int heightInPixel=480;

    //Scene(sceneComplexity);
    Camera(zvalue, planedist, widthInPixel, heightInPixel, width, height);

}
