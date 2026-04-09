#include <iostream>
#include "cgutilities.h"
#include "rgbimage.h"
#include "simpleraytracer.h"


int main(int argc, const char * argv[]) {
    Scene scene(20);
    RGBImage image(640,480);
    SimpleRayTracer tracer(2);
    tracer.traceScene(scene, image);
    image.saveToDisk("Prakt2aufg4.bmp");
    return 0;
}
