#include <iostream>
#include "cgutilities.h"
#include "rgbimage.h"
#include "simpleraytracer.h"


int main(int argc, const char * argv[]) {
    Scene scene(10);
    RGBImage image(320,240);
    SimpleRayTracer tracer(1);
    tracer.traceScene(scene, image);
    image.saveToDisk("PR_Aufgabe3.bmp");
    return 0;
}
