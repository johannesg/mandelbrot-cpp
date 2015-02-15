#pragma once
#include <complex>
typedef std::complex<double> Point;

class Viewport {
   public:
    Viewport(Point topLeft, Point bottomRight, int bufferWidth,
             int bufferHeight);

    Point getCoord(int x, int y);

   private:
    Point topLeft;
    Point bottomRight;
    Point scale;
//    int width;
//    int height;
};
