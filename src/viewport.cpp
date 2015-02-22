#include "viewport.h"

#include <complex>

Mandlebrot::Mandlebrot(Point topLeft, Point bottomRight, int bufferWidth,
                   int bufferHeight)
    : topLeft(topLeft),
      bottomRight(bottomRight),
      scale((bottomRight.real() - topLeft.real()) / bufferWidth,
            (bottomRight.imag() - topLeft.imag()) / bufferHeight) {}

Point Mandlebrot::getCoord(int x, int y) {
    return Point(scale.real() * x + topLeft.real(),
                 scale.imag() * y + topLeft.imag());
}
#define ITERATIONS 255
int Mandlebrot::calcDot(Point c) {
    Point z;
    int n = 0;
    double absz = 0.0;
    for (; n < ITERATIONS && absz < 4.0; n++) {
        z = z * z + c;
        absz = std::abs(z);
    }

    return n * 255 / ITERATIONS;
}
