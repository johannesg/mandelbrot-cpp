#include "viewport.h"

#include <complex>

Viewport::Viewport(Point topLeft, Point bottomRight, int bufferWidth,
                   int bufferHeight)
    : topLeft(topLeft),
      bottomRight(bottomRight),
      scale((bottomRight.real() - topLeft.real()) / bufferWidth,
            (bottomRight.imag() - topLeft.imag()) / bufferHeight) {}

Point Viewport::getCoord(int x, int y) {
    return Point(scale.real() * x + topLeft.real(),
                 scale.imag() * y + topLeft.imag());
}

int calcDot(Point c) {
    Point z;
    int n = 0;
    double absz = 0.0;
    for (; n < 100 && absz < 4.0; n++) {
        z = z * z + c;
        absz = std::abs(z);
    }
    return n;
}
